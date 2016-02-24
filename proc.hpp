
/* 
 * File:   proc.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 6:27 PM
 */

#ifndef PROC_HPP
#define PROC_HPP

#include <string>
#include <list>
#include <map>
#include <vector>

#include "object.hpp"
#include "executor.hpp"
#include "proc_hook.hpp"

namespace ccl {
	using namespace std;
	
	class proc {
		public:
			proc_hooks hooks = proc_hooks();
			ccl_object* call(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec);
			virtual ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) = 0;
	};
	
	vector<ccl_object*>* map_args(list<string> names, list<ccl_object*>* args, map<string, ccl_object*>* flags);
	vector<ccl_object*>* map_args(int nnames, string names[], list<ccl_object*>* args, map<string, ccl_object*>* flags);
	
	namespace procs {
		void register_proc(string name, proc* p);
		map<string,proc*>* get_proc_registry();
		proc* get_proc(string name);
		void register_base_procs();
	}
}

#endif /* PROC_HPP */

