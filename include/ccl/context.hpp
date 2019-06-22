/*
 * context.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CONTEXT_HPP_
#define INCLUDE_CCL_CONTEXT_HPP_

#include <deque>

#include "ccl/types.hpp"

namespace ccl {
	class _Context : public GcAble {
	public:
		std::deque<Thread> threads;
	};
	
	class _Thread : public GcAble {
	public:
		Context ctx;
	};
}

#endif /* INCLUDE_CCL_CONTEXT_HPP_ */
