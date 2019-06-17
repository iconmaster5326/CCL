/*
 * context.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CONTEXT_HPP_
#define INCLUDE_CCL_CONTEXT_HPP_

#include "ccl/types.hpp"

namespace ccl {
	class _Context : public GcAble {
	public:
	};
	
	class _Execution : public GcAble {
	public:
		Context ctx;
	};
}

#endif /* INCLUDE_CCL_CONTEXT_HPP_ */
