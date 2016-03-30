
/* 
 * File:   ccl_type_list.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "ccl_type_repeat.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_repeat::ccl_type_repeat() : ccl_type("recur") {}
	
	string ccl_type_repeat::to_string(ccl_object* obj) {
		vector<ccl_object*>* items = (vector<ccl_object*>*) obj->value;
		ostringstream strs;
		strs << "(recur: ";
		bool first = true;
		for (vector<ccl_object*>::iterator ii = items->begin(); ii != items->end(); ii++) {
			if (!first) {
				strs << ", ";
			} else {
				first = false;
			}
			
			ccl_object* item = *ii;
			strs << item->type->to_string(item);
		}
		strs << ")";
		return strs.str();
	}
	
	ccl_type_repeat::~ccl_type_repeat() {
		
	}
}