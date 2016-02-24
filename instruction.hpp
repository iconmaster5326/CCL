
/* 
 * File:   instruction.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 4:18 PM
 */

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <list>
#include <string>
#include <map>

#include "expression.hpp"
#include "source_range.hpp"

namespace ccl {
	using namespace std;
	
	typedef enum {
		OP_CLEAR, OP_PIPE, OP_EACH
	} op_type;
	
	class instruction {
		public:
			op_type type;
			string cmd;
			list<expression*> args;
			int nflags;
			int nargs;
			source_range range = source_range("?", -1);
			
			instruction();
			~instruction();
		private:
			
	};
}

#endif /* INSTRUCTION_HPP */

