
/* 
 * File:   fenv.cpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 5:21 PM
 */

#include "fenv.hpp"
#include "proc.hpp"

namespace ccl {
	using namespace std;

	fenv::fenv() {
		this->parent = NULL;
	}

	fenv::fenv(fenv* parent) {
		this->parent = parent;
	}
	
	ccl_object* fenv::get(string name) {
		if (vars.find(name) == vars.end()) {
			return parent ? parent->get(name) : constants::b_false();
		}
		return vars[name];
	}
	
	void fenv::put(string name, ccl_object* value) {
		fenv* f = this;
		while (f) {
			if (f->vars.find(name) != f->vars.end()) {
				f->set(name, value);
				return;
			}
			f = f->parent;
		}
		set(name, value);
	}
	
	void fenv::set(string name, ccl_object* value) {
		vars[name] = value;
	}
	
	bool fenv::has(string name) {
		if (vars.find(name) == vars.end()) {
			return parent ? parent->has(name) : false;
		}
		return true;
	}

	fenv::~fenv() {
		
	}
	
	fenv* make_base_fenv() {
		fenv* f = new fenv();
		
		map<string,proc*>* reg = procs::get_proc_registry();
		for(map<string,proc*>::iterator i = reg->begin(); i != reg->end(); i++) {
			string key = i->first;
			proc* value = i->second;
			f->vars[key] = new ccl_object(types::proc(), (void*) value);
		}
		
		return f;
	}
}

