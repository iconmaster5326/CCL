
/* 
 * File:   executor.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 5:57 PM
 */

#include "executor.hpp"
#include "proc.hpp"
#include "proc_user.hpp"
#include "ccl_type_code.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>

namespace ccl {
	using namespace std;
	
	executor::executor(machine* m, program* p, fenv* this_fenv, ccl_object* input) : p{p}, m{m}, this_fenv{this_fenv}, input{input} {
		output = NULL;
		stack = list<ccl_object*>();
		flag_stack = list<map<string,ccl_object*>>();
		flags = map<string,ccl_object*>();
		pos = p->code.begin();
		stack.push_front(input);
		hook = NULL;
	}
	
	bool executor::step() {
		if (hook) {
			try {
				hook->call(this);
			} catch (exception& e) {
				ostringstream strs;
				strs << "in file " << last_op.range.file << ", line " << last_op.range.line << ": in proc '" << last_op.s << "': " << e.what();
				throw runtime_error(strs.str());
			}
			return false;
		}
		
		if (pos == p->code.end()) {
			output = stack.front();
			return true;
		}
		operation op = *pos;
		
		switch (op.op) {
			case CMD_EMIT: {
				stack.push_front(p->consts[op.n]);
				break;
			}
			case CMD_ECODE: {
				stack.push_front(new ccl_object(types::code(), (void*) new code_spec((program*) p->consts[op.n]->value, this_fenv)));
				break;
			}
			case CMD_CALL: {
				if (op.n + 1 > stack.size()) {
					ostringstream strs;
					strs << "Internal error: Attempt to CALL " << op.s << " " << op.n << " from a stack of size " << stack.size();
					throw runtime_error(strs.str());
				}
				
				list<ccl_object*> args = list<ccl_object*>();
				for (int i = 0; i < op.n; i++) {
					ccl_object* arg = stack.front();
					stack.pop_front();
					args.push_front(arg);
				}
				ccl_object* proc_in = stack.front();
				stack.pop_front();
				
				if (this_fenv->has(op.s)) {
					ccl_object* to_call = this_fenv->get(op.s);
					if (to_call->type != types::proc()) {
						ostringstream strs;
						strs << "in file " << op.range.file << ", line " << op.range.line << ": ";
						strs << "Attempt to call object '" << op.s << "' of type '" << to_call->type->name << "' (object must be of type 'proc')";
						throw runtime_error(strs.str());
					}
					proc* pr = (proc*) to_call->value;
					try {
						ccl_object* outval = pr->call(proc_in, &args, &flags, this);
						if (outval) {
							stack.push_front(outval);
						}
					} catch (exception& e) {
						ostringstream strs;
						strs << "in file " << op.range.file << ", line " << op.range.line << ": in proc '" << op.s << "': " << e.what();
						throw runtime_error(strs.str());
					}
					flags = map<string,ccl_object*>();
				} else {
					ostringstream strs;
					strs << "in file " << op.range.file << ", line " << op.range.line << ": ";
					strs << "Attempt to call nonexistent proc '" << op.s << "'";
					throw runtime_error(strs.str());
				}
				
				break;
			}
			case CMD_LOAD: {
				stack.push_front(this_fenv->get(op.s));
				break;
			}
			case CMD_PUSHF: {
				flag_stack.push_front(flags);
				flags = map<string,ccl_object*>();
				break;
			}
			case CMD_ADDF: {
				if (stack.empty()) {
					throw runtime_error("Internal error: Attempt to ADDF from empty stack");
				}
				flags[op.s] = stack.front();
				stack.pop_front();
				break;
			}
			case CMD_POPF: {
				if (flag_stack.empty()) {
					throw runtime_error("Internal error: Attempt to POPF from empty flag stack");
				}
				flags = flag_stack.front();
				flag_stack.pop_front();
				break;
			}
			case CMD_CONCAT: {
				if (2 > stack.size()) {
					ostringstream strs;
					strs << "Internal error: Attempt to CONCAT from a stack of size " << stack.size();
					throw runtime_error(strs.str());
				}
				ccl_object* op1 = stack.front();
				stack.pop_front();
				ccl_object* op2 = stack.front();
				stack.pop_front();
				string s1 = op1->type->to_string(op1);
				string s2 = op2->type->to_string(op2);
				stack.push_front(new ccl_object(types::str(), (void*) (new string(s2 + s1))->c_str()));
				break;
			}
			case CMD_PICK: {
				int n = 0;
				if (op.n >= stack.size()) {
					ostringstream strs;
					strs << "Internal error: Attempt to PICK " << op.n << " from a stack of size " << stack.size();
					throw runtime_error(strs.str());
				}
				list<ccl_object*>::const_iterator i;
				for (i = stack.begin(); i != stack.end(); i++) {
					if (n == op.n) {break;}
					n++;
				}
				stack.push_front(*i);
				break;
			}
			case CMD_RESET: {
				if (stack.empty()) {
					throw runtime_error("Internal error: Attempt to RESET from empty stack");
				}
				stack.pop_front();
				stack.push_front(input);
				break;
			}
			case CMD_DROP: {
				if (stack.empty()) {
					throw runtime_error("Internal error: Attempt to DROP from empty stack");
				}
				stack.pop_front();
				break;
			}
		}
		
		pos++; last_op = op;
		return false;
	}
	
	executor::~executor() {
		
	}
}