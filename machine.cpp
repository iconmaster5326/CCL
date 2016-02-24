
/* 
 * File:   machine.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 5:56 PM
 */

#include <stdexcept>
#include <iostream>

#include "machine.hpp"

namespace ccl {
	using namespace std;
	
	machine::machine(program* p, fenv* root_fenv) : root_fenv{root_fenv} {
		if (p) {
			load(p);
		}
	}
	
	void machine::load(program* p, fenv* f, ccl_object* input) {
		callstack.push_front(new executor(this, p, f ? f : root_fenv, input ? input : constants::b_false()));
	}
	
	bool machine::step() {
		if (callstack.empty()) {
			return true;
		}
		bool res = false;
		
		try {
			res = callstack.front()->step();
		} catch (exception& e) {
			if (traps.empty()) {
				throw runtime_error(e.what());
			} else {
				bool good = false;
				for (list<machine_trap*>::iterator ii = traps.begin(); ii != traps.end(); ii++) {
					machine_trap* trap = *ii;
					if (trap->enabled) {
						trap->enabled = false;
						if (trap->trap(this, (new string(e.what()))->c_str())) {
							good = true;
							ii++;
							traps.erase(traps.begin(), ii);
							break;
						}
					}
				}
				
				for (list<machine_trap*>::iterator ii = traps.begin(); ii != traps.end(); ii++) {
					machine_trap* trap = *ii;
					trap->enabled = true;
				}
				
				if (!good) {
					throw runtime_error(e.what());
				}
			}
		}
		
		if (res) {
			executor* e = callstack.front();
			callstack.pop_front();
			if (callstack.empty()) {
				return true;
			}
			callstack.front()->output = e->output;
		}
		return false;
	}
	
	void machine::run() {
		while (!step()) {}
	}
	
	machine::~machine() {
		
	}
}