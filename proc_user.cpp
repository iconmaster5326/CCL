
/* 
 * File:   proc_user.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 7:25 PM
 */

#include "proc_user.hpp"
#include "machine.hpp"
#include "executor.hpp"
#include "pc_hook.hpp"

#include <vector>

namespace ccl {
	using namespace std;
	
	class proc_user_hook : public pc_hook {
		public:
			void call(executor* e) override {
				e->stack.push_front(e->output);
				e->hook = NULL;
			}
	};
	
	proc_user::proc_user(program* p, fenv* f, list<arg_spec>* specs) : p{p}, f{f}, specs{specs} {}

	ccl_object* proc_user::call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		fenv* f2 = new fenv(f);
		
		for (list<arg_spec>::iterator ii = specs->begin(); ii != specs->end(); ii++) {
			arg_spec spec = *ii;
			
			if (spec.flag) {
				if (flags->find(spec.name) == flags->end()) {
					f2->set(spec.name, spec.default_value);
				} else {
					f2->set(spec.name, (*flags)[spec.name]);
				}
			} else {
				if (flags->find(spec.name) != flags->end()) {
					f2->set(spec.name, (*flags)[spec.name]);
				} else if (args->empty()) {
					f2->set(spec.name, spec.default_value);
				} else {
					f2->set(spec.name, args->front());
					args->pop_front();
				}
			}
		}
		
		exec->m->load(p, f2, input);
		exec->hook = new proc_user_hook();
		return NULL;
	}

	proc_user::~proc_user() {

	}
}