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
#include <shared_mutex>
#include <iostream>

#ifdef CCL_GC_DEBUG
#define CCL_GC_PRINT(x) {\
	Lock lock{gcMutex};\
	std::cout << "[GC] " << x << std::endl;\
}
#else
#define CCL_GC_PRINT(x)
#endif

namespace ccl {
	using Mutex = std::recursive_mutex;
	using Lock = std::lock_guard<Mutex>;
	
	extern size_t gcSoftLimit, gcHardLimit;
	
	struct GcNode; struct GcAble;
	
	extern Mutex gcMutex;
	extern size_t gcBytesAllocated;
	extern std::list<GcNode> gcNodes;
	using GcNodeRef = std::list<GcNode>::iterator;
	
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
			return gcNodes.begin();
		}
		
		inline GcNode(GcAble* memory, size_t bytes) : memory{memory}, size{bytes} {}
	};
	
	template<typename T>
	struct Gc {
		GcNodeRef node;
		
		Gc(const T& other) {
			Lock lock{gcMutex};
			
			for (GcNodeRef node = gcNodes.begin(); node != gcNodes.end(); node++) {
				if (&other == node->memory) {
					this->node = node;
					node->refs++;
					break;
				}
			}
			
			throw GcInternalError();
		}
		
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
