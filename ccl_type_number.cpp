
/* 
 * File:   ccl_type_number.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */

#include <sstream>
#include <stdexcept>

#include "ccl_type_number.hpp"
#include "proc.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_number::ccl_type_number() : ccl_type("number") {
		
	}
	
	string ccl_type_number::to_string(ccl_object* obj) {
		ostringstream strs;
		strs << obj->num_val();
		return strs.str();
	}
	
	bool ccl_type_number::equals(ccl_object* arg1, ccl_object* arg2) {
		if (arg2->type == types::num()) {
			return arg1->num_val() == arg2->num_val();
		}
		return false;
	}
	
	bool ccl_type_number::compare(ccl_object* arg1, ccl_object* arg2, int& result) {
		if (arg2->type != types::num()) {
			return false;
		}
		
		result = arg1->num_val() - arg2->num_val();
		return true;
	}
	
	void ccl_type_number::register_proc_hooks() {
		
	}

	ccl_type_number::~ccl_type_number() {
		
	}
}