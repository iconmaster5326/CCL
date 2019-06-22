/*
 * classes.hpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CLASSES_HPP_
#define INCLUDE_CCL_CLASSES_HPP_

#include "ccl/class.hpp"

namespace ccl {
	CCL_DECL_GC_CLASS(ClassNil) : public _Class {
	public:
		static ClassNil CLASS;
		static Object NIL;
	};
}

#endif /* INCLUDE_CCL_CLASSES_HPP_ */
