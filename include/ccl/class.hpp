/*
 * class.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CLASS_HPP_
#define INCLUDE_CCL_CLASS_HPP_

#include <deque>
#include <unordered_map>

#include "ccl/types.hpp"

namespace ccl {
	class _Class : public GcAble {
	public:
		// lifetime handlers
		virtual ~_Class();
		virtual void gcGetChildren(_Object& self, std::function<void(GcNodeRef)> addChild);
		virtual void gcFree(_Object& self);
		
		// C++ STL overrides
		virtual size_t hash(const _Object& self) const;
		virtual bool equal(const _Object& self, const Object other) const;
		virtual bool lessThan(const _Object& self, const Object other) const;
		
		// interface to objects
		virtual std::string name();
		virtual std::string toString(_Object& self);
		virtual Object execute(_Object& self, Thread& thread, Object& input, std::deque<Object>& args, std::unordered_map<std::string, Object>& flags);
	};
}

#endif /* INCLUDE_CCL_CLASS_HPP_ */
