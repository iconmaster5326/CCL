
/* 
 * File:   ccl_type_key.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */

#include <sstream>

#include "ccl_type_key.hpp"

namespace ccl {
	using namespace std;
	
	key_spec::key_spec() {}
	key_spec::key_spec(ccl_object* key, ccl_object* value) : key{key}, value{value} {}

	
	ccl_type_key::ccl_type_key() : ccl_type("key") {}
	
	string ccl_type_key::to_string(ccl_object* obj) {
		key_spec* spec = (key_spec*) obj->value;
		return string() + "(" + spec->key->type->to_string(spec->key) + " : " + spec->value->type->to_string(spec->value) + ")";
	}

	ccl_type_key::~ccl_type_key() {
		
	}

}