
/* 
 * File:   ccl_type_arg.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */

#include <sstream>

#include "ccl_type_arg.hpp"

namespace ccl {
	using namespace std;
	
	arg_spec::arg_spec() {}
	arg_spec::arg_spec(string name, ccl_object* default_value, bool flag) : name(name), default_value(default_value), flag(flag) {}
	
	ccl_type_arg::ccl_type_arg() : ccl_type("arg") {}
	
	string ccl_type_arg::to_string(ccl_object* obj) {
		arg_spec* spec = (arg_spec*) obj->value;
		return "(arg: " + spec->name + ")";
	}

	ccl_type_arg::~ccl_type_arg() {
		
	}

}