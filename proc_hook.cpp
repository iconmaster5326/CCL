#include "proc_hook.hpp"
#include "object.hpp"
#include "executor.hpp"

namespace ccl {
	using namespace std;
	
	proc_hooks::proc_hooks() : list<proc_hook>() {}
	
	void proc_hooks::connect(proc_hook hook) {
		this->push_front(hook);
	}
	
	ccl_object* proc_hooks::fire(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		for (proc_hooks::iterator ii = this->begin(); ii != this->end(); ii++) {
			proc_hook hook = *ii;
			ccl_object* result = hook(input, args, flags, exec);
			if (result) {
				return result;
			}
		}
		return NULL;
	}

}
