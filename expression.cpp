
/* 
 * File:   expression.cpp
 * Author: iconmaster
 * 
 * Created on January 20, 2016, 4:10 PM
 */

#include "expression.hpp"

namespace ccl {
	using namespace std;
	
	expression::expression() {
		type = E_NONE;
	}
	
	expression::expression(expr_type type) {
		this->type = type;
	}
	
	expression::expression(expr_type type, string value) {
		this->type = type;
		this->value.as_str = (char*) value.c_str();
	}
	
	expression::expression(expr_type type, char* value) {
		this->type = type;
		this->value.as_str = value;
	}
	
	expression::expression(expr_type type, double value) {
		this->type = type;
		this->value.as_num = value;
	}
	
	expression::expression(expr_type type, list<void*>* value) {
		this->type = type;
		this->value.as_list = value;
	}
	
	expression::expression(expr_type type, char* name, expression* value) {
		this->type = type;
		this->value.as_fp.name = name;
		this->value.as_fp.expr = value;
	}

	expression::~expression() {
		
	}
}