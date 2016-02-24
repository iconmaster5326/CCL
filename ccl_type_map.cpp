
/* 
 * File:   ccl_type_map.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "ccl_type_map.hpp"
#include "proc_hook.hpp"
#include "proc.hpp"
#include "ccl_type_key.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_map::ccl_type_map() : ccl_type("map") {}
	
	string ccl_type_map::to_string(ccl_object* arg) {
		ccl_map_type* m = (ccl_map_type*) arg->value;
		ostringstream strs;
		strs << "{";
		bool first = true;
		for (ccl_map_type::iterator ii = m->begin(); ii != m->end(); ii++) {
			if (!first) {
				strs << ", ";
			} else {
				first = false;
			}
			
			strs << ii->first->type->to_string(ii->first) << " : " << ii->second->type->to_string(ii->second);
		}
		strs << "}";
		return strs.str();
	}
	
	ccl_object* ccl_type_map::copy(ccl_object* arg) {
		ccl_map_type* m = (ccl_map_type*) arg->value;
		ccl_map_type* m2 = new ccl_map_type();
		*m2 = *m;
		return new ccl_object(types::map(), m2);
	}
	
	ccl_iterator* ccl_type_map::iterator(ccl_object* arg) {
		ccl_map_type* m = (ccl_map_type*) arg->value;
		return new map_iterator(m->begin(), m->end());
	}
	
	ccl_type_map::~ccl_type_map() {
		
	}

	/*
	 * ======================
	 * ITERATOR
	 * ======================
	 */
	


	map_iterator::map_iterator(ccl_map_type::iterator ii, ccl_map_type::iterator end) : ii{ii}, end{end} {}

	
	void map_iterator::next() {
		ii++;
	}
	
	bool map_iterator::done() {
		return ii == end;
	}
	
	ccl_object* map_iterator::value() {
		return new ccl_object(types::key(), new key_spec(ii->first, ii->second));
	}
	
	/*
	 * ======================
	 * PROC HOOKS
	 * ======================
	 */
	
	static ccl_object* on_add(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"key"}, args, flags);
		ccl_object* key = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (key->type != types::key()) {
			throw runtime_error(string()+"Argument must be of type 'key', got type '"+key->type->name+"'");
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		key_spec* spec = (key_spec*) key->value;
		ccl_map_type* m3 = new ccl_map_type();
		
		for (ccl_map_type::iterator ii = m->begin(); ii != m->end(); ii++) {
			(*m3)[ii->first] = ii->second;
		}
		
		(*m3)[spec->key] = spec->value;
		
		return new ccl_object(types::map(), m3);
	}
	
	static ccl_object* on_add_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"key"}, args, flags);
		ccl_object* key = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (key->type != types::key()) {
			throw runtime_error(string()+"Argument must be of type 'key', got type '"+key->type->name+"'");
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		key_spec* spec = (key_spec*) key->value;
		
		(*m)[spec->key] = spec->value;
		
		return input;
	}
	
	static ccl_object* on_size(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		return new ccl_object(types::num(), (double) m->size());
	}
	
	static ccl_object* on_has(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"i"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		return m->find(index) != m->end() ? constants::b_true() : constants::b_false();
	}
	
	static ccl_object* on_empty(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		return m->empty() ? constants::b_true() : constants::b_false();
	}
	
	static ccl_object* on_get(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "or"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* default_value = (*got)[1] ? (*got)[1] : constants::b_false();
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		return m->find(index) == m->end() ? default_value : (*m)[index];
	}
	
	static ccl_object* on_append(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"map"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (index->type != types::map()) {
			throw runtime_error(string()+"Argument must be of type 'map', got type '"+index->type->name+"'");
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		ccl_map_type* m2 = (ccl_map_type*) index->value;
		ccl_map_type* m3 = new ccl_map_type();
		
		for (ccl_map_type::iterator ii = m->begin(); ii != m->end(); ii++) {
			(*m3)[ii->first] = ii->second;
		}
		
		for (ccl_map_type::iterator ii = m2->begin(); ii != m2->end(); ii++) {
			(*m3)[ii->first] = ii->second;
		}
		
		return new ccl_object(types::map(), m3);
	}
	
	static ccl_object* on_append_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"map"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (index->type != types::map()) {
			throw runtime_error(string()+"Argument must be of type 'map', got type '"+index->type->name+"'");
		}
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		ccl_map_type* m2 = (ccl_map_type*) index->value;
		
		for (ccl_map_type::iterator ii = m2->begin(); ii != m2->end(); ii++) {
			(*m)[ii->first] = ii->second;
		}
		
		return input;
	}
	
	static ccl_object* on_find(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"v"}, args, flags);
		ccl_object* value = (*got)[0] ? (*got)[0] : constants::b_false();
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		for (ccl_map_type::iterator ii = m->begin(); ii != m->end(); ii++) {
			if (!ccl_compare(ii->second, value) && !ccl_compare(value, ii->second)) {
				return ii->first;
			}
		}
		
		return constants::b_false();
	}
	
	static ccl_object* on_set(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::map()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "v"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* value = (*got)[1] ? (*got)[1] : constants::b_false();
		
		ccl_map_type* m = (ccl_map_type*) input->value;
		
		(*m)[index] = value;
		return input;
	}
	
	void ccl_type_map::register_proc_hooks() {
		procs::get_proc("add")->hooks.connect(on_add);
		procs::get_proc("add!")->hooks.connect(on_add_bang);
		procs::get_proc("size")->hooks.connect(on_size);
		procs::get_proc("empty")->hooks.connect(on_empty);
		procs::get_proc("get")->hooks.connect(on_get);
		procs::get_proc("set")->hooks.connect(on_set);
		procs::get_proc("has")->hooks.connect(on_has);
		procs::get_proc("append")->hooks.connect(on_append);
		procs::get_proc("append!")->hooks.connect(on_append_bang);
		procs::get_proc("find")->hooks.connect(on_find);
	}
}