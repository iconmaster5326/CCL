
/* 
 * File:   machine.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 5:56 PM
 */

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "executor.hpp"
#include "fenv.hpp"
#include "program.hpp"
#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class machine_trap {
		public:
			bool enabled = true;
			virtual bool trap(machine* m, string msg) = 0;
	};
	
	class machine {
		public:
			list<executor*> callstack;
			fenv* root_fenv;
			list<machine_trap*> traps;
			machine(program* p = NULL, fenv* root_fenv = make_base_fenv());
			void load(program* p, fenv* f = NULL, ccl_object* input = NULL);
			bool step();
			void run();
			~machine();
	};
}

#endif /* MACHINE_HPP */

