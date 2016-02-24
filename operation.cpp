
/* 
 * File:   operation.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 2:37 PM
 */

#include "operation.hpp"

namespace ccl {
	using namespace std;

	operation::operation(opcode op, unsigned short n, string s, source_range range) : op{op}, n{n}, s{s}, range{range} {}

	operation::~operation() {
		
	}

}