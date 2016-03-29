
/* 
 * File:   object.cpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 5:24 PM
 */

#include <cstdlib>

#include "object.hpp"
#include "ccl_type_string.hpp"
#include "ccl_type_number.hpp"
#include "ccl_type_arg.hpp"
#include "ccl_type_code.hpp"
#include "ccl_type_bool.hpp"
#include "ccl_type_list.hpp"
#include "ccl_type_map.hpp"
#include "ccl_type_key.hpp"
#include "ccl_type_repeat.hpp"

namespace ccl {
	using namespace std;
	
	bool ccl_compare(ccl_object* a,ccl_object* b) {
		int res;
		if (a->type->compare(a, b, res)) {
			return res < 0;
		} else {
			return ((char*) a->value) - ((char*) b->value);
		}
	}
	
	bool ccl_object_compare_class::operator()(const ccl_object* lhs, const ccl_object* rhs) {
		return ccl_compare((ccl_object*) lhs, (ccl_object*) rhs);
	}
	
	ccl_type::ccl_type(char* name) : name{name} {}
	
	string ccl_type::to_string(ccl_object* obj) {
		return "(" + string(obj->type->name) + ")";
	}
	
	bool ccl_type::equals(ccl_object* arg1, ccl_object* arg2) {
		return arg1->type == arg2->type && arg1->value == arg2->value;
	}
	
	ccl_object* ccl_type::copy(ccl_object* arg) {
		return new ccl_object(arg->type, arg->value);
	}
	
	bool ccl_type::compare(ccl_object* arg1, ccl_object* arg2, int& result) {
		return false;
	}
	
	ccl_iterator* ccl_type::iterator(ccl_object* arg) {
		return NULL;
	}
	
	void ccl_type::register_proc_hooks() {}

	ccl_type::~ccl_type() {
		
	}
	
	ccl_object::ccl_object(ccl_type* type, void* value) : type{type}, value{value} {}
	
	ccl_object::ccl_object(ccl_type* type, double value) : type{type} {
		this->value = *((void**)&value);
	}
	
	double ccl_object::num_val() {
		return *((double*)&value);
	}
	
	string ccl_object::str_val() {
		return string((char*)value);
	}
	
	ccl_object::~ccl_object() {
		
	}
	
	/* 
	 * =========
	 * TYPES
	 * =========
	 */
	
	namespace types {
		static ccl_type_string _STR = ccl_type_string();
		ccl_type* str() {
			return &_STR;
		}
		static ccl_type_number _NUM = ccl_type_number();
		ccl_type* num() {
			return &_NUM;
		}
		static ccl_type_list _LIST = ccl_type_list();
		ccl_type* list() {
			return &_LIST;
		}
		static ccl_type_map _MAP = ccl_type_map();
		ccl_type* map() {
			return &_MAP;
		}
		static ccl_type_true _TRUE = ccl_type_true();
		ccl_type* b_true() {
			return &_TRUE;
		}
		static ccl_type_false _FALSE = ccl_type_false();
		ccl_type* b_false() {
			return &_FALSE;
		}
		static ccl_type_code _CODE = ccl_type_code();
		ccl_type* code() {
			return &_CODE;
		}
		static ccl_type _PROC = ccl_type("proc");
		ccl_type* proc() {
			return &_PROC;
		}
		static ccl_type_arg _ARG = ccl_type_arg();
		ccl_type* arg() {
			return &_ARG;
		}
		static ccl_type_key _KEY = ccl_type_key();
		ccl_type* key() {
			return &_KEY;
		}
		static ccl_type_repeat _REPEAT = ccl_type_repeat();
		ccl_type* repeat() {
			return &_REPEAT;
		}
		static ccl_type _ADDED_HOOK = ccl_type("_added_hook");
		ccl_type* _added_hook() {
			return &_ADDED_HOOK;
		}
	}
	
	namespace constants {
		static ccl_object _TRUE = ccl_object(types::b_true(), (void*) NULL);
		ccl_object* b_true() {
			return &_TRUE;
		}
		static ccl_object _FALSE = ccl_object(types::b_false(), (void*) NULL);
		ccl_object* b_false() {
			return &_FALSE;
		}
		static ccl_object _EMPTY_CODE = ccl_object(types::code(), (void*) new code_spec(new program(), new fenv()));
		ccl_object* empty_code() {
			return &_EMPTY_CODE;
		}
	}
}


