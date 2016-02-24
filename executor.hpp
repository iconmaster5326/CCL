
/* 
 * File:   executor.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 5:57 PM
 */

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <list>
#include <map>
#include <string>

#include "operation.hpp"
#include "program.hpp"
#include "fenv.hpp"
#include "pc_hook.hpp"

namespace ccl {
	using namespace std;
	
	class machine;
	
	class executor {
		public:
			program* p;
			list<ccl_object*> stack;
			list<map<string,ccl_object*>> flag_stack;
			map<string,ccl_object*> flags;
			list<operation>::const_iterator pos;
			fenv* this_fenv;
			ccl_object* input;
			ccl_object* output;
			machine* m;
			pc_hook* hook;
			operation last_op = operation(CMD_NOP);
			executor(machine* m, program* p, fenv* this_fenv, ccl_object* input);
			bool step();
			~executor();
	};
}

#endif /* EXECUTOR_HPP */

