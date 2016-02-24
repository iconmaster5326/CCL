
/* 
 * File:   ccl_type_bool.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */

#include <sstream>

#include "ccl_type_bool.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_true::ccl_type_true() : ccl_type("true") {}
	
	string ccl_type_true::to_string(ccl_object* obj) {
		return "true";
	}

	ccl_type_true::~ccl_type_true() {
		
	}

	ccl_type_false::ccl_type_false() : ccl_type("false") {}
	
	string ccl_type_false::to_string(ccl_object* obj) {
		return "false";
	}

	ccl_type_false::~ccl_type_false() {
		
	}
}