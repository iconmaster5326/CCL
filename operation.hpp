
/* 
 * File:   operation.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 2:37 PM
 */

#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <cstdlib>
#include <string>

#include "source_range.hpp"

namespace ccl {
	using namespace std;
	
	typedef enum : unsigned char {
		CMD_NOP, CMD_RESET, CMD_CALL, CMD_EMIT, CMD_LOAD, CMD_PUSHF, CMD_ADDF, CMD_POPF, CMD_CONCAT, CMD_PICK, CMD_ECODE, CMD_DROP
	} opcode;
	
	class operation {
		public:
			opcode op;
			unsigned short n;
			string s;
			source_range range;
			
			operation(opcode op, unsigned short n = 0, string s = string(), source_range range = source_range("?", -1));
			~operation();
		private:

	};
}

#endif /* OPERATION_HPP */

