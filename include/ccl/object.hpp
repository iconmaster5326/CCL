/*
 * object.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_OBJECT_HPP_
#define INCLUDE_CCL_OBJECT_HPP_

#include "ccl/types.hpp"
#include "ccl/class.hpp"

namespace ccl {
	class _Object : public GcAble {
	public:
		Class clazz;
		union {
			void* asPtr;
			double asDouble;
			uint64_t asInt;
		} value;
		
		// lifetime handlers
		_Object(Class& clazz, void* value);
		_Object(Class& clazz, double value);
		_Object(Class& clazz, uint64_t value);
		virtual ~_Object();
		void gcGetChildren(std::function<void(GcNodeRef)> addChild);
		
		// C++ STL overrides
		inline size_t hash() const {
			return clazz->hash(*this);
		}
		inline bool equal(const Object other) const {
			return clazz->equal(*this, other);
		}
		inline bool lessThan(const Object other) const {
			return clazz->lessThan(*this, other);
		}
		
		// interface to classes
		Object execute(Thread& thread, Object& input, std::deque<Object>& args, std::unordered_map<std::string, Object>& flags) {
			return clazz->execute(*this, thread, input, args, flags);
		}
	};
}

namespace std {
	template<> struct hash<ccl::Object> {
		inline size_t operator()(const ccl::Object& a) const noexcept {
			return a->hash();
		}
	};
	
	template<> struct equal_to<ccl::Object> {
		inline bool operator()(const ccl::Object& a, const ccl::Object& b) const {
			return a->equal(b);
		};
	};
	
	template<> struct less<ccl::Object> {
		inline bool operator()(const ccl::Object& a, const ccl::Object& b) const {
			return a->lessThan(b);
		};
	};
}

#endif /* INCLUDE_CCL_OBJECT_HPP_ */
