
/* 
 * File:   instruction.cpp
 * Author: iconmaster
 * 
 * Created on January 20, 2016, 4:18 PM
 */

#include "instruction.hpp"

namespace ccl {
	using namespace std;

	instruction::instruction() {
		type = OP_PIPE;
		cmd = string("");
		args = list<expression*>();
		nflags = 0; nargs = 0;
	}
	
	instruction::~instruction() {
		
	}

}