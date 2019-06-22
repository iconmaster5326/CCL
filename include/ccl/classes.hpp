/*
 * classes.hpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_CLASSES_HPP_
#define INCLUDE_CCL_CLASSES_HPP_

#include "ccl/object.hpp"

namespace ccl {
	CCL_DECL_GC_CLASS(ClassNil) : public _Class {
	public:
		static ClassNil CLASS;
		static Object NIL;
	};
	
	CCL_DECL_GC_CLASS(ClassString) : public _Class {
	public:
		using Instance = std::string;
		static ClassString CLASS;
		static Object create(const Instance& value) {
			Instance* p = new Instance(value);
			return Object((Class) CLASS, p);
		}
		static Instance& value(const Object& self) {
			return * (Instance*) self->value.asPtr;
		}
	};
}

#endif /* INCLUDE_CCL_CLASSES_HPP_ */
