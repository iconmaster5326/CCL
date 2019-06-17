/*
 * object.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_OBJECT_HPP_
#define INCLUDE_CCL_OBJECT_HPP_

#include <boost/uuid/uuid.hpp>

#include "ccl/types.hpp"
#include "ccl/class.hpp"

namespace ccl {
	class _Object : public GcAble {
	public:
		Class clazz;
		boost::uuids::uuid id;
		Execution owner;
		Mutex mutex;
		union {
			void* asPtr;
			double asDouble;
			uint64_t asInt;
		} value;
		
		// lifetime handlers
		_Object(Execution& owner, Class& clazz, void* value);
		_Object(Execution& owner, Class& clazz, double value);
		_Object(Execution& owner, Class& clazz, uint64_t value);
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
