/*
 * types.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_TYPES_HPP_
#define INCLUDE_CCL_TYPES_HPP_

#include "ccl/gc.hpp"

#define CCL_DECL_GC_CLASS(name) class _##name; using name = Gc<_##name>; class _##name

namespace ccl {
	CCL_DECL_GC_CLASS(Object);
	CCL_DECL_GC_CLASS(Thread);
	CCL_DECL_GC_CLASS(Context);
	CCL_DECL_GC_CLASS(Scope);
	CCL_DECL_GC_CLASS(Program);
	CCL_DECL_GC_CLASS(Class);
}

#endif /* INCLUDE_CCL_TYPES_HPP_ */
