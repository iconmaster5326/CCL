
/* 
 * File:   ccl_type_list.cpp
 * Author: iconmaster
 * 
 * Created on January 21, 2016, 4:52 PM
 */
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "ccl_type_list.hpp"
#include "proc_hook.hpp"
#include "proc.hpp"
#include "machine.hpp"
#include "ccl_type_code.hpp"
#include "ccl_type_key.hpp"

namespace ccl {
	using namespace std;
	
	ccl_type_list::ccl_type_list() : ccl_type("list") {}
	
	string ccl_type_list::to_string(ccl_object* obj) {
		vector<ccl_object*>* items = (vector<ccl_object*>*) obj->value;
		ostringstream strs;
		strs << "[";
		bool first = true;
		for (vector<ccl_object*>::iterator ii = items->begin(); ii != items->end(); ii++) {
			if (!first) {
				strs << ", ";
			} else {
				first = false;
			}
			
			ccl_object* item = *ii;
			strs << item->type->to_string(item);
		}
		strs << "]";
		return strs.str();
	}
	
	ccl_object* ccl_type_list::copy(ccl_object* arg) {
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) arg->value);
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		*v2 = *v1;
		return new ccl_object(types::list(), v2);
	}
	
	ccl_iterator* ccl_type_list::iterator(ccl_object* arg) {
		return new list_iterator(((vector<ccl_object*>*) arg->value)->begin(), ((vector<ccl_object*>*) arg->value)->end());
	}
	
	ccl_type_list::~ccl_type_list() {
		
	}

	/*
	 * ======================
	 * ITERATOR
	 * ======================
	 */
	

	list_iterator::list_iterator(vector<ccl_object*>::iterator ii, vector<ccl_object*>::iterator end) : ii{ii}, end{end} {}
	
	void list_iterator::next() {
		ii++;
	}
	
	bool list_iterator::done() {
		return ii == end;
	}
	
	ccl_object* list_iterator::value() {
		return *ii;
	}
	
	/*
	 * ======================
	 * PROC HOOKS
	 * ======================
	 */
	
	static ccl_object* on_append(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		if (operand->type != types::list()) {
			throw runtime_error(string()+"Argument must be of type 'list', got type '"+operand->type->name+"'");
		}
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v2 = ((vector<ccl_object*>*) operand->value);
		vector<ccl_object*>* v3 = new vector<ccl_object*>();
		
		*v3 = *v1;
		v3->insert( v3->end(), v2->begin(), v2->end() );
		
		return new ccl_object(types::list(), v3);
	}
	
	static ccl_object* on_append_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		if (operand->type != types::list()) {
			throw runtime_error(string()+"Argument must be of type 'list', got type '"+operand->type->name+"'");
		}
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v2 = ((vector<ccl_object*>*) operand->value);
		
		v1->insert( v1->end(), v2->begin(), v2->end() );
		
		return input;
	}
	
	static ccl_object* on_prepend_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		if (operand->type != types::list()) {
			throw runtime_error(string()+"Argument must be of type 'list', got type '"+operand->type->name+"'");
		}
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v2 = ((vector<ccl_object*>*) operand->value);
		
		v1->insert( v1->begin(), v2->begin(), v2->end() );
		
		return input;
	}
	
	static ccl_object* on_add(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v3 = new vector<ccl_object*>();
		
		*v3 = *v1;
		v3->push_back(operand);
		
		return new ccl_object(types::list(), v3);
	}
	
	static ccl_object* on_add_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		ccl_object* operand = args->empty() ? constants::b_false() : args->front();
		
		vector<ccl_object*>* v1 = ((vector<ccl_object*>*) input->value);
		v1->push_back(operand);
		
		return input;
	}
	
	static ccl_object* on_size(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		return new ccl_object(types::num(), (double) v->size());
	}
	
	static ccl_object* on_get(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "or"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* or_else = (*got)[1] ? (*got)[1] : constants::b_false();
		
		if (index->type != types::num()) {
			throw runtime_error(string()+"Index into list must be of type 'num', got type '"+index->type->name+"'");
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n = (int) index->num_val();
		if (n < 0 || n >= v->size()) {
			ostringstream strs;
			strs << "Index into list out of bounds (size is " << v->size() << ", index was " << n << ")";
			throw runtime_error(strs.str());
		}
		
		return (*v)[n];
	}
	
	static ccl_object* on_set(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(2, new string[2] {"i", "v"}, args, flags);
		ccl_object* index = (*got)[0] ? (*got)[0] : constants::b_false();
		ccl_object* value = (*got)[1] ? (*got)[1] : constants::b_false();
		
		if (index->type != types::num()) {
			throw runtime_error(string()+"Index into list must be of type 'num', got type '"+index->type->name+"'");
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n = (int) index->num_val();
		if (n < 0 || n >= v->size()) {
			ostringstream strs;
			strs << "Index into list out of bounds (size is " << v->size() << ", index was " << n << ")";
			throw runtime_error(strs.str());
		}
		
		(*v)[n] = value;
		
		return input;
	}
	
	static ccl_object* on_find(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"e"}, args, flags);
		ccl_object* elem = (*got)[0] ? (*got)[0] : constants::b_false();
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		for (int i = 0; i < v->size(); i++) {
			if (elem->type->equals(elem, (*v)[i])) {
				return new ccl_object(types::num(), (double) i);
			}
		}
		return constants::b_false();
	}
	
	static ccl_object* on_copy(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(2, new string[2] {"b", "e"}, args, flags);
		ccl_object* begin = (*got)[0];
		ccl_object* end = (*got)[1];
		
		if (!begin && !end) {
			return input->type->copy(input);
		}
		
		if (end && end->type != types::num()) {
			throw runtime_error(string()+"End index into list must be of type 'number', got type '"+end->type->name+"'");
		}
		
		if (begin && begin->type != types::num()) {
			throw runtime_error(string()+"Begin index into list must be of type 'number', got type '"+begin->type->name+"'");
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int b = begin ? min((int) v->size(), max((int) 0, (int) begin->num_val())) : 0;
		int e = end ? max(0, min((int) v->size() - b,(int) end->num_val())) : max(0, (int) v->size() - b);
		
		if (begin->num_val() < 0) {
			e += begin->num_val();
		}
		
		vector<ccl_object*>* v3 = new vector<ccl_object*>();
		
		if (e) {
			v3->insert(v3->begin(), v->begin() + b, v->begin() + b + e);
		}
		
		return new ccl_object(types::list(), v3);
	}
	
	class proc_list_sort_hook : public pc_hook {
		public:
			bool first = true;
			vector<ccl_object*>* v = new vector<ccl_object*>();
			code_spec* cmp;
			int pos = 1;
			proc_list_sort_hook(vector<ccl_object*>* v, code_spec* cmp) : v{v}, cmp{cmp} {}
			void call(executor* e) override {
				if (pos >= v->size()) {
					e->stack.push_front(new ccl_object(types::list(), v));
					e->hook = NULL;
					return;
				}
				
				if (!first) {
					if (e->output->type == types::b_false()) {
						ccl_object* swap = (*v)[pos];
						(*v)[pos] = (*v)[pos-1];
						(*v)[pos-1] = swap;
						
						if (pos > 1) {
							pos--;
						}
					} else {
						pos++;
						
						if (pos >= v->size()) {
							e->stack.push_front(new ccl_object(types::list(), v));
							e->hook = NULL;
							return;
						}
					}
				} else {
					first = false;
				}
				
				ccl_object* arg1 = (*v)[pos];
				ccl_object* arg2 = (*v)[pos-1];
				fenv* f2 = new fenv(cmp->f);
				e->m->load(cmp->p, f2, new ccl_object(types::key(), new key_spec(arg1, arg2)));
			}
	};
	
	static ccl_object* on_sort(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"compare"}, args, flags);
		ccl_object* arg_cmp = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* sorted = new vector<ccl_object*>();
		*sorted = *v;
		
		if (arg_cmp) {
			if (arg_cmp->type != types::code()) {
				throw runtime_error(string()+"Argument 'compare' must be of type 'code', got type '"+arg_cmp->type->name+"'");
			}
			exec->hook = new proc_list_sort_hook(sorted, (code_spec*) arg_cmp->value);
			return new ccl_object(types::_added_hook(), (void*) NULL);
		}
		
		sort(sorted->begin(), sorted->end(), ccl_compare);
		
		return new ccl_object(types::list(), sorted);
	}
	
	static ccl_object* on_sort_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* got = map_args(1, new string[1] {"compare"}, args, flags);
		ccl_object* arg_cmp = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		if (arg_cmp) {
			if (arg_cmp->type != types::code()) {
				throw runtime_error(string()+"Argument 'compare' must be of type 'code', got type '"+arg_cmp->type->name+"'");
			}
			exec->hook = new proc_list_sort_hook(v, (code_spec*) arg_cmp->value);
			return new ccl_object(types::_added_hook(), (void*) NULL);
		}
		
		sort(v->begin(), v->end(), ccl_compare);
		
		return input;
	}
	
	static ccl_object* on_empty(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		return v->empty() ? constants::b_true() : constants::b_false();
	}
	
	static ccl_object* on_reverse(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* reversed = new vector<ccl_object*>();
		
		reversed->insert(reversed->begin(), v->rbegin(), v->rend());
		
		return new ccl_object(types::list(), reversed);
	}
	
	static ccl_object* on_reverse_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		*v2 = *v; v->clear();
		
		v->insert(v->begin(), v2->rbegin(), v2->rend());
		delete v2;
		
		return input;
	}
	
	static ccl_object* on_rep(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0] ? (*got)[0] : constants::b_false();
		
		if (times->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
		}
		
		int n = (int) times->num_val();
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		
		for (int i = 0; i < n; i++) {
			v2->insert(v2->begin(), v->begin(), v->end());
		}
		
		return new ccl_object(types::list(), v2);
	}
	
	static ccl_object* on_head(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		if (!times) {
			return v->empty() ? constants::b_false() : (*v)[0];
		}
		
		if (times->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
		}
		
		int n = min((int)v->size(),max(0,(int) times->num_val()));
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		
		v2->insert(v2->begin(), v->begin(), v->begin() + n);
		
		return new ccl_object(types::list(), v2);
	}
	
	static ccl_object* on_tail(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		if (!times) {
			return v->empty() ? constants::b_false() : (*v)[v->size()-1];
		}
		
		if (times->type != types::num()) {
			throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
		}
		
		int n = min((int)v->size(),max(0,(int) times->num_val()));
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		
		v2->insert(v2->begin(), v->end() - n, v->end());
		
		return new ccl_object(types::list(), v2);
	}
	
	static ccl_object* on_behead(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n;
		if (!times) {
			n = min((int)v->size(),1);
		} else {
			if (times->type != types::num()) {
				throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
			}
			
			n = min((int)v->size(),max(0,(int) times->num_val()));
		}
		
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		
		v2->insert(v2->begin(), v->begin() + n, v->end());
		
		return new ccl_object(types::list(), v2);
	}
	
	static ccl_object* on_curtail(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n;
		if (!times) {
			n = min((int)v->size(),1);
		} else {
			if (times->type != types::num()) {
				throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
			}
			
			n = min((int)v->size(),max(0,(int) times->num_val()));
		}
		
		vector<ccl_object*>* v2 = new vector<ccl_object*>();
		
		v2->insert(v2->begin(), v->begin(), v->end() - n);
		
		return new ccl_object(types::list(), v2);
	}
	
	static ccl_object* on_behead_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n;
		if (!times) {
			ccl_object* o2;
			n = min((int)v->size(),1);
			o2 = v->empty() ? constants::b_false() : (*v)[0];
			
			v->erase(v->begin(), v->begin() + n);
			
			return o2;
		} else {
			if (times->type != types::num()) {
				throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
			}
			
			n = min((int)v->size(),max(0,(int) times->num_val()));
			
			vector<ccl_object*>* v2 = new vector<ccl_object*>();
			v2->insert(v2->begin(), v->begin(), v->begin() + n);
			
			v->erase(v->begin(), v->begin() + n);
			
			return new ccl_object(types::list(), v2);
		}
	}
	
	static ccl_object* on_curtail_bang(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) {
		if (input->type != types::list()) {
			return NULL;
		}
		vector<ccl_object*>* got = map_args(1, new string[1] {"n"}, args, flags);
		ccl_object* times = (*got)[0];
		
		vector<ccl_object*>* v = ((vector<ccl_object*>*) input->value);
		
		int n;
		if (!times) {
			ccl_object* o2;
			n = min((int)v->size(),1);
			o2 = v->empty() ? constants::b_false() : (*v)[v->size() - 1];
			
			v->erase(v->end() - n, v->end());
			
			return o2;
		} else {
			if (times->type != types::num()) {
				throw runtime_error(string()+"Argument must be of type 'number', got type '"+times->type->name+"'");
			}
			
			n = min((int)v->size(),max(0,(int) times->num_val()));
			
			vector<ccl_object*>* v2 = new vector<ccl_object*>();
			v2->insert(v2->begin(), v->end() - n, v->end());
			
			v->erase(v->end() - n, v->end());
			
			return new ccl_object(types::list(), v2);
		}
	}
	
	void ccl_type_list::register_proc_hooks() {
		procs::get_proc("append")->hooks.connect(on_append);
		procs::get_proc("append!")->hooks.connect(on_append_bang);
		procs::get_proc("prepend!")->hooks.connect(on_prepend_bang);
		procs::get_proc("add")->hooks.connect(on_add);
		procs::get_proc("add!")->hooks.connect(on_add_bang);
		procs::get_proc("size")->hooks.connect(on_size);
		procs::get_proc("empty")->hooks.connect(on_empty);
		procs::get_proc("get")->hooks.connect(on_get);
		procs::get_proc("set")->hooks.connect(on_set);
		procs::get_proc("find")->hooks.connect(on_find);
		procs::get_proc("copy")->hooks.connect(on_copy);
		procs::get_proc("sort")->hooks.connect(on_sort);
		procs::get_proc("sort!")->hooks.connect(on_sort_bang);
		procs::get_proc("reverse")->hooks.connect(on_reverse);
		procs::get_proc("reverse!")->hooks.connect(on_reverse_bang);
		procs::get_proc("mul")->hooks.connect(on_rep);
		procs::get_proc("head")->hooks.connect(on_head);
		procs::get_proc("tail")->hooks.connect(on_tail);
		procs::get_proc("behead")->hooks.connect(on_behead);
		procs::get_proc("curtail")->hooks.connect(on_curtail);
		procs::get_proc("behead!")->hooks.connect(on_behead_bang);
		procs::get_proc("curtail!")->hooks.connect(on_curtail_bang);
	}
}