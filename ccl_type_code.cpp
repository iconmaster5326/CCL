
/* 
 * File:   ccl_type_code.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */

#include <sstream>

#include "ccl_type_code.hpp"

namespace ccl {
	using namespace std;
	
	code_spec::code_spec(program* p, fenv* f) : p{p}, f{f} {}
	
	ccl_type_code::ccl_type_code() : ccl_type("code") {}
	
	string ccl_type_code::to_string(ccl_object* obj) {
		return "(code)";
	}

	ccl_type_code::~ccl_type_code() {
		
	}

}