
/* 
 * File:   ccl_type_string.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "ccl_type_string.hpp"
#include "proc_hook.hpp"
#include "proc.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_string::ccl_type_string() : ccl_type("string") {
		
	}
	
	string ccl_type_string::to_string(ccl_object* obj) {
		return obj->str_val();
	}
	
	bool ccl_type_string::equals(ccl_object* arg1, ccl_object* arg2) {
		if (arg2->type == types::str()) {
			return arg1->str_val().compare(arg2->str_val()) == 0;
		}
		return false;
	}
	
	ccl_object* ccl_type_string::copy(ccl_object* arg) {
		return new ccl_object(arg->type, (void*) (new string((char*) arg->value))->c_str());
	}
	
	bool ccl_type_string::compare(ccl_object* arg1, ccl_object* arg2, int& result) {
		if (arg2->type != types::str()) {
			return false;
		}
		result = arg1->str_val().compare(arg2->str_val());
		return true;
	}
	
	ccl_iterator* ccl_type_string::iterator(ccl_object* arg) {
		return new string_iterator(arg->str_val());
	}

	ccl_type_string::~ccl_type_string() {
		
	}
	
	/*
	 * ======================
	 * ITERATOR
	 * ======================
	 */
	

	string_iterator::string_iterator(string s) : s{s} {}
	
	bool string_iterator::done() {
		return n >= s.size();
	}
	
	void string_iterator::next() {
		n++;
	}
	
	ccl_object* string_iterator::value() {
		return new ccl_object(types::num(), (double) s[n]);
	}
	
	/*
	 * ======================
	 * PROC HOOKS
	 * ======================
	 */
	
	static ccl_object* on_append(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		if (input->type != types::str()) {
			return NULL;
		}
		string s1 = input->str_val();
		string s2 = string(operand->type->to_string(operand));
		return new ccl_object(types::str(), (void*) (new string(s1 + s2))->c_str());
	}
	
	static ccl_object* on_prepend(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		if (input->type != types::str()) {
			return NULL;
		}
		string s1 = input->str_val();
		string s2 = string(operand->type->to_string(operand));
		return new ccl_object(types::str(), (void*) (new string(s2 + s1))->c_str());
	}
	
	static ccl_object* on_get(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "or"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* or_else = (*got)[1] ? (*got)[1] : constants::b_false();
		
		if (index->type != types::num()) {
			throw runtime_error(string()+"Index into string must be of type 'num', got type '"+index->type->name+"'");
		}
		
		int n = (int) index->num_val();
		string s = input->str_val();
		if (n < 0 || n >= s.size()) {
			ostringstream strs;
			strs << "Index into string out of bounds (size is " << s.size() << ", index was " << n << ")";
			throw runtime_error(strs.str());
		}
		
		return new ccl_object(types::num(), (double) s[n]);
	}
	
	static ccl_object* on_set(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "v"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* value = (*got)[1] ? (*got)[1] : constants::b_false();
		
		if (index->type != types::num()) {
			throw runtime_error(string()+"Index into string must be of type 'number', got type '"+index->type->name+"'");
		}
		
		if (value->type != types::num()) {
			throw runtime_error(string()+"Character to set must be of type 'number', got type '"+index->type->name+"'");
		}
		
		int n = (int) index->num_val();
		int v = (int) value->num_val();
		string s = input->str_val();
		if (n < 0 || n >= s.size()) {
			ostringstream strs;
			strs << "Index into string out of bounds (size is " << s.size() << ", index was " << n << ")";
			throw runtime_error(strs.str());
		}
		
		return new ccl_object(types::str(), (void*) (s.substr(0, n) + ((char) v) + s.substr(n+1)).c_str());
	}
	
	static ccl_object* on_copy(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"b", "e"}, args, flags);
		ccl_object* begin = (*got)[0];
		ccl_object* end = (*got)[1];
		
		if (!begin && !end) {
			return input->type->copy(input);
		}
		
		if (end && end->type != types::num()) {
			throw runtime_error(string()+"End index into string must be of type 'number', got type '"+end->type->name+"'");
		}
		
		if (begin && begin->type != types::num()) {
			throw runtime_error(string()+"Begin index into string must be of type 'number', got type '"+begin->type->name+"'");
		}
		
		string s = input->str_val();
		int b = begin ? min((int) s.size(), max((int) 0,(int) begin->num_val())) : 0;
		int e = end ? min((int) s.size() - b,(int) end->num_val()) : max(0, (int) s.size() - b);
		
		if (begin->num_val() < 0) {
			e += begin->num_val();
		}
		
		return new ccl_object(types::str(), (void*) s.substr(b,e).c_str());
	}
	
	static ccl_object* on_find(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"s"}, args, flags);
		ccl_object* substr = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (substr->type != types::str()) {
			throw runtime_error(string()+"Search string must be of type 'string', got type '"+substr->type->name+"'");
		}
		
		string s = input->str_val();
		string s2 = substr->str_val();
		
		int result = s.find(s2);
		return result == s.npos ? constants::b_false() : new ccl_object(types::num(), (double) result);
	}
	
	static ccl_object* on_size(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		
		string s = input->str_val();
		
		return new ccl_object(types::num(), (double) s.size());
	}
	
	static ccl_object* on_empty(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		
		string s = input->str_val();
		
		return s.empty() ? constants::b_true() : constants::b_false();
	}
	
	static ccl_object* on_reverse(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		
		string s = input->str_val();
		ostringstream strs;
		
		for (int i = s.size() - 1; i >= 0; i--) {
			strs << s[i];
		}
		
		return new ccl_object(types::str(), (void*) strs.str().c_str());
	}
	
	static ccl_object* on_rep(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (times->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
		}
		
		int n = (int) times->num_val();
		string s = input->str_val();
		ostringstream* strs = new ostringstream();
		
		for (int i = 0; i < n; i++) {
			(*strs) << s;
		}
		
		return new ccl_object(types::str(), (void*) strs->str().c_str());
	}
	
	static ccl_object* on_head(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* count = (*got)[0];
		
		string s = input->str_val();
		
		if (!count) {
			if (s.empty()) {
				return constants::b_false();
			}
			
			return new ccl_object(types::num(), (double) s[0]);
		}
		
		if (count->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+count->type->name+"'");
		}
		
		int n = max(min((int) count->num_val(), (int) s.size()), 0);
		
		
		return new ccl_object(types::str(), (void*) s.substr(0, n).c_str());
	}
	
	static ccl_object* on_tail(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* count = (*got)[0];
		
		string s = input->str_val();
		
		if (!count) {
			if (s.empty()) {
				return constants::b_false();
			}
			
			return new ccl_object(types::num(), (double) s[s.size()-1]);
		}
		
		if (count->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+count->type->name+"'");
		}
		
		int n = max(min((int) count->num_val(), (int) s.size()), 0);
		
		
		return new ccl_object(types::str(), (void*) s.substr(s.size()-n, n).c_str());
	}
	
	static ccl_object* on_behead(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* count = (*got)[0] ? (*got)[0] : new ccl_object(types::num(), 1.0);
		
		string s = input->str_val();
		
		if (count->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+count->type->name+"'");
		}
		
		int n = max(min((int) count->num_val(), (int) s.size()), 0);
		
		return new ccl_object(types::str(), (void*) s.substr(n, s.size()-n).c_str());
	}
	
	static ccl_object* on_curtail(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::str()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* count = (*got)[0] ? (*got)[0] : new ccl_object(types::num(), 1.0);
		
		string s = input->str_val();
		
		if (count->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+count->type->name+"'");
		}
		
		int n = max(min((int) count->num_val(), (int) s.size()), 0);
		
		return new ccl_object(types::str(), (void*) s.substr(0, s.size()-n).c_str());
	}
	
	void ccl_type_string::register_proc_hooks() {
		procs::get_proc("add")->hooks.connect(on_append);
		procs::get_proc("append")->hooks.connect(on_append);
		procs::get_proc("prepend")->hooks.connect(on_prepend);
		procs::get_proc("get")->hooks.connect(on_get);
		procs::get_proc("set")->hooks.connect(on_set);
		procs::get_proc("copy")->hooks.connect(on_copy);
		procs::get_proc("find")->hooks.connect(on_find);
		procs::get_proc("size")->hooks.connect(on_size);
		procs::get_proc("empty")->hooks.connect(on_empty);
		procs::get_proc("reverse")->hooks.connect(on_reverse);
		procs::get_proc("mul")->hooks.connect(on_rep);
		procs::get_proc("head")->hooks.connect(on_head);
		procs::get_proc("tail")->hooks.connect(on_tail);
		procs::get_proc("behead")->hooks.connect(on_behead);
		procs::get_proc("curtail")->hooks.connect(on_curtail);
	}
}