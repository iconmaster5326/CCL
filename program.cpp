
/* 
 * File:   program.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 2:48 PM
 */

#include "program.hpp"

namespace ccl {
	using namespace std;
	program::program() {
		code = list<operation>();
		consts = vector<ccl_object*>();
	}

	program::~program() {
		
	}
}
