
/* 
 * File:   proc.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 6:27 PM
 */

#include "proc.hpp"

namespace ccl {
	using namespace std;
	
	vector<ccl_object*>* map_args(list<string> names, list<ccl_object*>* args, map<string, ccl_object*>* flags) {
		vector<ccl_object*>* a = new vector<ccl_object*>(names.size(), NULL);
		list<ccl_object*>::iterator argii = args->begin();
		int i = 0;
		for (list<string>::iterator ii = names.begin(); ii != names.end(); ii++) {
			string name = *ii;
			if (flags->find(name) == flags->end()) {
				if (argii != args->end()) {
					(*a)[i] = *argii;
					argii++;
				}
			} else {
				(*a)[i] = (*flags)[name];
			}
			i++;
		}
		
		return a;
	}
	
	vector<ccl_object*>* map_args(int nnames, string names[], list<ccl_object*>* args, map<string, ccl_object*>* flags) {
		list<string> a = list<string>();
		
		for (int i = 0; i < nnames; i++) {
			a.push_back(names[i]);
		}
		
		return map_args(a, args, flags);
	}
	
	namespace procs {
		static map<string,proc*> registry = map<string,proc*>();
		
		void register_proc(string name, proc* p) {
			registry[name] = p;
		}
		
		map<string,proc*>* get_proc_registry() {
			return &registry;
		}
		
		proc* get_proc(string name) {
			return registry[name];
		}
	}
	
	ccl_object* proc::call(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		ccl_object* result = this->hooks.fire(input, args, flags, exec);
		if (result) {
			if (result->type == types::_added_hook()) {
				return NULL;
			}
			return result;
		}
		return this->call_impl(input, args, flags, exec);
	}

}