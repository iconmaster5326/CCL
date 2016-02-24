
/* 
 * File:   program.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 2:48 PM
 */

#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <list>
#include <vector>

#include "operation.hpp"
#include "instruction.hpp"
#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class ccl_type; class ccl_object; 

	class program {
		public:
			list<operation> code;
			vector<ccl_object*> consts;
			program();
			~program();
		private:

	};
}

#endif /* PROGRAM_HPP */

