/*
 * class.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CLASS_HPP_
#define INCLUDE_CCL_CLASS_HPP_

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
	};
}

#endif /* INCLUDE_CCL_CLASS_HPP_ */
