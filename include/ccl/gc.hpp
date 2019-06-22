/*
 * gc.hpp
 *
 *  Created on: Jun 14, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_GC_HPP_
#define INCLUDE_CCL_GC_HPP_

#include <functional>
#include <list>
#include <mutex>
#include <unordered_set>
#include <iostream>
#include <type_traits>

#ifdef CCL_GC_DEBUG
#define CCL_GC_PRINT(x) {\
	Lock lock{gcMutex};\
	std::cout << "[GC] " << x << std::endl;\
}
#else
#define CCL_GC_PRINT(x)
#endif

namespace ccl {
	struct GcNode; struct GcAble;
	
	using Mutex = std::recursive_mutex;
	using Lock = std::lock_guard<Mutex>;
	using GcNodeRef = std::list<GcNode>::iterator;
	
	struct GcNodeRefHash {
		size_t operator()(const GcNodeRef& node) const {
			return (size_t) (void*) &*node;
		}
	};
	
	extern size_t gcSoftLimit, gcHardLimit;
	extern Mutex gcMutex;
	extern size_t gcBytesAllocated;
	extern std::list<GcNode> gcNodes;
	extern std::unordered_set<GcNodeRef,GcNodeRefHash>* gcValidNodes;
	extern std::list<GcNode>* gcStaticallyAllocatedNodes;
	
	struct GcOutOfMemoryError {};
	struct GcInternalError {};
	
	struct GcNode {
		bool collectable = false;
		size_t refs = 1;
		size_t size;
		GcAble* memory;
		
		static void runMinorGC();
		static void runMajorGC();
		static bool valid(GcNodeRef node);
		
		template<typename T, typename... Args>
		static GcNodeRef alloc(Args... args) {
			T* mem = new T(args...);
			Lock lock{gcMutex};
			
			gcBytesAllocated += sizeof(T);
			CCL_GC_PRINT("New GC heap size: " << gcBytesAllocated);
			
			if (gcBytesAllocated > gcSoftLimit) {
				CCL_GC_PRINT("Running minor GC (soft limit: " << gcSoftLimit << ")");
				runMinorGC();
				
				if (gcBytesAllocated > gcSoftLimit) {
					CCL_GC_PRINT("Running major GC (soft limit: " << gcSoftLimit << ")");
					runMajorGC();
					
					if (gcBytesAllocated > gcHardLimit) {
						CCL_GC_PRINT("out of memory (hard limit: " << gcHardLimit << ")");
						throw GcOutOfMemoryError();
					} else if (gcBytesAllocated > gcSoftLimit) {
						gcSoftLimit *= 2;
						CCL_GC_PRINT("Expanding soft limit (soft limit now: " << gcSoftLimit << ")");
					}
				}
			}
			
			gcNodes.emplace_front(mem, sizeof(T));
			if (!gcValidNodes) gcValidNodes = new std::unordered_set<GcNodeRef,GcNodeRefHash>();
			gcValidNodes->insert(gcNodes.begin());
			return gcNodes.begin();
		}
		
		template<typename T, typename... Args>
		static GcNodeRef staticAlloc(Args... args) {
			T* mem = new T(args...);
			Lock lock{gcMutex};
			
			if (!gcStaticallyAllocatedNodes) gcStaticallyAllocatedNodes = new std::list<GcNode>();
			if (!gcValidNodes) gcValidNodes = new std::unordered_set<GcNodeRef,GcNodeRefHash>();
			
			gcStaticallyAllocatedNodes->emplace_front(mem, sizeof(T));
			gcValidNodes->insert(gcStaticallyAllocatedNodes->begin());
			return gcStaticallyAllocatedNodes->begin();
		}
		
		inline GcNode(GcAble* memory, size_t bytes) : memory{memory}, size{bytes} {}
	};
	
	struct GcStaticAlloc {};
	constexpr GcStaticAlloc gcStaticAlloc{};
	
	template<typename T>
	struct Gc {
		GcNodeRef node;
		
		Gc(GcNodeRef other) : node{other} {
			Lock lock{gcMutex};
			node->refs++;
		}
		
		Gc(const Gc<T>& other) : node{other.node} {
			Lock lock{gcMutex};
			node->refs++;
		}
		
		Gc(Gc<T>&& other) : node{other.node} {
			
		}
		
		template<typename U, typename = typename std::enable_if<std::is_convertible<U&,T&>::value>::type>
		Gc(const Gc<U>& other) : node{other.node} {
			Lock lock{gcMutex};
			node->refs++;
		}
		
		Gc<T>& operator=(const Gc<T>& other) {
			if (&other == this) return *this;
			this->~Gc();
			node = other.node;
			return *this;
		}
		
		Gc<T>& operator=(Gc<T>&& other) {
			if (&other == this) return *this;
			this->~Gc();
			node = other.node;
			return *this;
		}
		
		template<typename... Args>
		Gc(const GcStaticAlloc gcStaticAlloc, Args... args) : node{GcNode::staticAlloc<T, Args...>(args...)} {}
		
		template<typename... Args>
		Gc(Args... args) : node{GcNode::alloc<T, Args...>(args...)} {}
		
		~Gc() {
			Lock lock{gcMutex};
			if (!GcNode::valid(node)) return;
			
			node->refs--;
			if (node->refs == 0) {
				node->collectable = true;
			}
		}
		
		T& operator*() const {
			return *(T*)(node->memory);
		}
		
		T* operator->() const {
			return (T*)(node->memory);
		}
		
		bool operator==(const Gc<T>& other) const {
			return node == other.node;
		}
		
		operator GcNodeRef() const {
			return node;
		}
		
		bool valid() {
			return GcNode::valid(node);
		}
	};
	
	struct GcAble {
		virtual ~GcAble();
		virtual void gcGetChildren(std::function<void(GcNodeRef)> addChild);
	};
}

#endif /* INCLUDE_CCL_GC_HPP_ */
