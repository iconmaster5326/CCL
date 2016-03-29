
/* 
 * File:   base_procs.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 6:27 PM
 */

#include "proc.hpp"
#include "proc_user.hpp"
#include "ccl_type_arg.hpp"
#include "ccl_type_code.hpp"
#include "machine.hpp"
#include "ccl_type_key.hpp"
#include "ccl_type_map.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <algorithm>

namespace ccl {
	using namespace std;
	
	namespace procs {
		
		static void dump(fenv* f) {
			for(map<string, ccl_object*>::iterator i = f->vars.begin(); i != f->vars.end(); i++) {
				string key = i->first;
				ccl_object* value = i->second;

				cout << "[DUMP] " <<key << " = " << value->type->to_string(value) << endl;
			}
			if (f->parent) {
				cout << "[DUMP] PARENT:" << endl;
				dump(f->parent);
			}
		}
		
		class proc_dump : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					dump(exec->this_fenv);
					return input;
				}
		};
		
		class push_output_hook : public pc_hook {
		public:
				void call(executor* e) override {
					e->stack.push_front(e->output);
					e->hook = NULL;
				}
		};
		
		class proc_undef : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					throw runtime_error(string("Operation undefined for type '")+input->type->name+"'");
				}
		};
		
		class proc_print : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					cout << input->type->to_string(input) << endl;
					return input;
				}
		};
		
		class proc_input : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					string* s = new string();
					getline(cin, *s);
					return new ccl_object(types::str(), (void*) s->c_str());
				}
		};
		
		class proc_proc : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* arg;
					char* f_name = NULL;
					list<arg_spec>* f_args = new list<arg_spec>();
					code_spec* f_code = NULL;
					
					if (args->empty()) {
						throw runtime_error("expected one argument of type 'code'; got zero");
					}
					
					if (flags->find("name") != flags->end()) {
						f_name = (char*) (*flags)["name"]->value;
					}
					
					if (flags->find("code") != flags->end()) {
						f_code = (code_spec*) (*flags)["code"]->value;
					}
					
					while (!args->empty()) {
						arg = args->front();
						args->pop_front();
						
						if (arg->type == types::str()) {
							if (f_name) {
								f_args->push_back(arg_spec(arg->str_val(), constants::b_false(), false));
							} else {
								f_name = (char*) arg->value;
							}
						} else if (arg->type == types::arg()) {
							f_args->push_back(*((arg_spec*) arg->value));
						} else if (arg->type == types::code()) {
							if (f_code) {
								throw runtime_error("expected one argument of type 'code'; got multiple");
							}
							f_code = (code_spec*) arg->value;
						} else {
							throw runtime_error(string("expected argument of type 'string'/'arg'/'code'; got type '")+arg->type->name+"'");
						}
					}
					
					if (!f_code) {
						throw runtime_error("expected one argument of type 'code'; got zero");
					}
					proc_user* pr = new proc_user(f_code->p, f_code->f, f_args);
					ccl_object* pro = new ccl_object(types::proc(), pr);
					if (f_name) {
						exec->this_fenv->set(f_name, pro);
					}
					return pro;
				}
		};
		
		class proc_arg : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					arg_spec* spec = new arg_spec();
					spec->flag = false;
					
					vector<ccl_object*>* got = map_args(2, new string[2] {"name", "or"}, args, flags);
					
					ccl_object* name = (*got)[0];
					if (!name) {
						throw runtime_error(string("Argument 'name' is required"));
					}
					if (name->type != types::str()) {
						throw runtime_error(string("expected argument of type 'string'; got type '")+name->type->name+"'");
					}
					spec->name = string((char*) name->value);
					
					ccl_object* value = (*got)[1];
					if (value) {
						spec->default_value = value;
					} else {
						spec->default_value = constants::b_false();
					}
					
					return new ccl_object(types::arg(), (void*) spec);
				}
		};
		
		class proc_flag : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					arg_spec* spec = new arg_spec();
					spec->flag = true;
					
					vector<ccl_object*>* got = map_args(2, new string[2] {"name", "or"}, args, flags);
					
					ccl_object* name = (*got)[0];
					if (!name) {
						throw runtime_error(string("Argument 'name' is required"));
					}
					if (name->type != types::str()) {
						throw runtime_error(string("expected argument of type 'string'; got type '")+name->type->name+"'");
					}
					spec->name = string((char*) name->value);
					
					ccl_object* value = (*got)[1];
					if (value) {
						spec->default_value = value;
					} else {
						spec->default_value = constants::b_false();
					}
					
					return new ccl_object(types::arg(), (void*) spec);
				}
		};
		
		class proc_var : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (args->empty()) {
						throw runtime_error("expected one argument of type 'string'; got zero");
					}
					ccl_object* nameo = args->front();
					if (nameo->type != types::str()) {
						throw runtime_error(string("expected argument of type 'string'; got type '")+nameo->type->name+"'");
					}
					char* name = (char*) nameo->value;
					exec->this_fenv->put(name, input);
					return input;
				}
		};
		
		class proc_local : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (args->empty()) {
						throw runtime_error("expected one argument of type 'string'; got zero");
					}
					ccl_object* nameo = args->front();
					if (nameo->type != types::str()) {
						throw runtime_error(string("expected argument of type 'string'; got type '")+nameo->type->name+"'");
						return input;
					}
					char* name = (char*) nameo->value;
					exec->this_fenv->set(name, input);
					return input;
				}
		};
		
		class proc_add : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::num() && operand->type == types::num()) {
						return new ccl_object(types::num(), input->num_val() + operand->num_val());
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_sub : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::num() && operand->type == types::num()) {
						return new ccl_object(types::num(), input->num_val() - operand->num_val());
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_mul : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::num() && operand->type == types::num()) {
						return new ccl_object(types::num(), input->num_val() * operand->num_val());
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_div : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::num() && operand->type == types::num()) {
						return new ccl_object(types::num(), input->num_val() / operand->num_val());
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_mod : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::num() && operand->type == types::num()) {
						return new ccl_object(types::num(), (double) (((int) input->num_val()) % ((int) operand->num_val())));
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_eq : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type->equals(input, operand)) {
						return constants::b_true();
					}
					return constants::b_false();
				}
		};
		
		class proc_neq : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type->equals(input, operand)) {
						return constants::b_false();
					}
					return constants::b_true();
				}
		};
		
		class proc_lt : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					int comp;
					bool result = input->type->compare(input, operand, comp);
					if (result) {
						return comp < 0 ? constants::b_true() : constants::b_false();
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_le : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					int comp;
					bool result = input->type->compare(input, operand, comp);
					if (result) {
						return comp <= 0 ? constants::b_true() : constants::b_false();
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_gt : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					int comp;
					bool result = input->type->compare(input, operand, comp);
					if (result) {
						return comp > 0 ? constants::b_true() : constants::b_false();
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_ge : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					int comp;
					bool result = input->type->compare(input, operand, comp);
					if (result) {
						return comp >= 0 ? constants::b_true() : constants::b_false();
					}
					throw runtime_error(string("Operation undefined between types '")+input->type->name+"' and '"+operand->type->name+"'");
				}
		};
		
		class proc_and : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::b_false()) {
						return input;
					}
					return operand;
				}
		};
		
		class proc_or : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* operand = args->empty() ? constants::b_false() : args->front();
					if (input->type == types::b_false()) {
						return operand;
					}
					return input;
				}
		};
		
		class proc_not : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type == types::b_false()) {
						return constants::b_true();
					}
					return constants::b_false();
				}
		};
		
		class proc_prepend : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* temp = input;
					input = args->front();
					args->pop_front();
					args->push_front(temp);
					return procs::get_proc("append")->call(input, args, flags, exec);
				}
		};
		
		class proc_prepend_bang : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* temp = input;
					input = args->front();
					args->pop_front();
					args->push_front(temp);
					return procs::get_proc("append!")->call(input, args, flags, exec);
				}
		};
		
		class proc_copy : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return input->type->copy(input);
				}
		};
		
		class proc_true : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return constants::b_true();
				}
		};
		
		class proc_false : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return constants::b_false();
				}
		};
		
		class proc_pass : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return input;
				}
		};
		
		class proc_each_hook : public pc_hook {
			public:
				bool first = true;
				vector<ccl_object*>* output = new vector<ccl_object*>();
				code_spec* spec;
				ccl_iterator* iter;
				proc_each_hook(code_spec* spec, ccl_iterator* iter) : spec{spec}, iter{iter} {}
				void call(executor* e) override {
					if (!first) {
						output->push_back(e->output);
					} else {
						first = false;
					}
					
					if (iter->done()) {
						e->stack.push_front(new ccl_object(types::list(), output));
						e->hook = NULL;
						return;
					}
					
					ccl_object* value = iter->value();
					fenv* f2 = new fenv(spec->f);
					e->m->load(spec->p, f2, value);
					
					iter->next();
				}
		};
		
		class proc_each : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					ccl_object* arg = args->empty() ? constants::empty_code() : args->front();
					
					if (arg->type == types::proc()) {
						proc* pr = (proc*) arg->value;
						list<ccl_object*> arglist;
						map<string, ccl_object*> flaglist;
						vector<ccl_object*>* out = new vector<ccl_object*>();
						while (!iter->done()) {
							ccl_object* item = iter->value();
							out->push_back(pr->call(item, &arglist, &flaglist, exec));
							iter->next();
						}
						return new ccl_object(types::list(), out);
					}
					
					if (arg->type != types::code()) {
						throw runtime_error(string()+"Argument of type '"+input->type->name+"' is not executable");
					}
					
					code_spec* spec = (code_spec*) arg->value;
					exec->hook = new proc_each_hook(spec, iter);
					return NULL;
				}
		};
		
		class proc_pairs_hook : public pc_hook {
			public:
				bool first = true;
				vector<ccl_object*>* output = new vector<ccl_object*>();
				code_spec* spec;
				ccl_iterator* iter;
				int i = 0;
				proc_pairs_hook(code_spec* spec, ccl_iterator* iter) : spec{spec}, iter{iter} {}
				void call(executor* e) override {
					if (!first) {
						output->push_back(e->output);
					} else {
						first = false;
					}
					
					if (iter->done()) {
						e->stack.push_front(new ccl_object(types::list(), output));
						e->hook = NULL;
						return;
					}
					
					ccl_object* value = iter->value();
					fenv* f2 = new fenv(spec->f);
					e->m->load(spec->p, f2, new ccl_object(types::key(), new key_spec(new ccl_object(types::num(), (double) i), value)));
					
					iter->next(); i++;
				}
		};
		
		class proc_pairs : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					ccl_object* arg = args->empty() ? constants::empty_code() : args->front();
					
					if (arg->type == types::proc()) {
						proc* pr = (proc*) arg->value;
						list<ccl_object*> arglist;
						map<string, ccl_object*> flaglist;
						vector<ccl_object*>* out = new vector<ccl_object*>();
						while (!iter->done()) {
							ccl_object* item = iter->value();
							out->push_back(pr->call(item, &arglist, &flaglist, exec));
							iter->next();
						}
						return new ccl_object(types::list(), out);
					}
					
					if (arg->type != types::code()) {
						throw runtime_error(string()+"Argument of type '"+input->type->name+"' is not executable");
					}
					
					code_spec* spec = (code_spec*) arg->value;
					exec->hook = new proc_pairs_hook(spec, iter);
					return NULL;
				}
		};
		
		class proc_list : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* vec = new vector<ccl_object*>(args->size());
					int i = 0;
					for (list<ccl_object*>::iterator ii = args->begin(); ii != args->end(); ii++) {
						(*vec)[i] = *ii;
						i++;
					}
					return new ccl_object(types::list(), vec);
				}
		};
		
		class proc_map : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_map_type* m = new ccl_map_type();
					for (list<ccl_object*>::iterator ii = args->begin(); ii != args->end(); ii++) {
						if ((*ii)->type != types::key()) {
							throw runtime_error(string()+"argument expects type 'key', got type '"+(*ii)->type->name+"'");
						}
						key_spec* spec = (key_spec*) (*ii)->value;
						(*m)[spec->key] = spec->value;
					}
					return new ccl_object(types::map(), m);
				}
		};
		
		class proc_key : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* got = map_args(2, new string[2] {"k", "v"}, args, flags);
					ccl_object* key = (*got)[0] ? (*got)[0] : constants::b_false();
					ccl_object* value = (*got)[1] ? (*got)[1] : constants::b_false();
					
					return new ccl_object(types::key(), new key_spec(key, value));
				}
		};
		
		class proc_fold_hook : public pc_hook {
			public:
				bool first = true;
				ccl_iterator* iter;
				ccl_object* acc;
				string name;
				code_spec* code;
				proc_fold_hook(ccl_iterator* iter, ccl_object* acc, string name, code_spec* code) : iter{iter}, acc{acc}, name{name}, code{code} {}
				void call(executor* e) override {
					if (!first) {
						acc = e->output;
					} else {
						first = false;
					}
					
					if (iter->done()) {
						e->stack.push_front(acc);
						e->hook = NULL;
						return;
					}
					
					ccl_object* value = iter->value();
					fenv* f2 = new fenv(code->f);
					f2->set(name, acc);
					e->m->load(code->p, f2, value);
					
					iter->next();
				}
		};
		
		class proc_fold : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					
					vector<ccl_object*>* got = map_args(2, new string[2] {"v", "f"}, args, flags);
					ccl_object* arg = (*got)[0] ? (*got)[0] : constants::b_false();
					ccl_object* code = (*got)[1] ? (*got)[1] : constants::b_false();
					
					if (code->type == types::proc()) {
						proc* pr = (proc*) code->value;
						list<ccl_object*> arglist;
						map<string, ccl_object*> flaglist;
						while (!iter->done()) {
							ccl_object* item = iter->value();
							arglist.push_front(arg);
							arg = pr->call(item, &arglist, &flaglist, exec);
							arglist.clear();
							iter->next();
						}
						return arg;
					} else {
						if (arg->type != types::arg() && arg->type != types::str()) {
							throw runtime_error(string()+"argument 'v' expects type 'arg', got type '"+arg->type->name+"'");
						}
						if (code->type != types::code()) {
							throw runtime_error(string()+"argument 'f' expects type 'code', got type '"+code->type->name+"'");
						}

						arg_spec* aspec = arg->type == types::arg() ? (arg_spec*) arg->value : new arg_spec(arg->str_val(), constants::b_false(), false);
						code_spec* cspec = (code_spec*) code->value;

						exec->hook = new proc_fold_hook(iter, aspec->default_value ? aspec->default_value : constants::b_false(), aspec->name, cspec);
					}
					
					return NULL;
				}
		};
		
		class proc_let : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					list<arg_spec*> specs;
					
					for (list<ccl_object*>::iterator ii = args->begin(); ii != args->end(); ii++) {
						ccl_object* arg = *ii;
						
						if (arg->type == types::arg()) {
							specs.push_back((arg_spec*) arg->value);
						} else if (arg->type == types::str()) {
							specs.push_back(new arg_spec(arg->str_val(), input, false));
						} else if (arg->type == types::code()) {
							code_spec* code = (code_spec*) arg->value;
							fenv* f2 = new fenv(code->f);
							for (list<arg_spec*>::iterator jj = specs.begin(); jj != specs.end(); jj++) {
								arg_spec* spec = *jj;
								f2->set(spec->name, spec->default_value);
							}
							exec->m->load(code->p, f2, input);
							exec->hook = new push_output_hook();
							return NULL;
						} else {
							throw runtime_error(string()+"Expected argument of type 'code', 'arg', or 'string'; got type '"+arg->type->name+"'");
						}
					}
					
					throw runtime_error(string()+"Expected one argument of type 'code'; got zero");
				}
		};
		
		class proc_if : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* got = map_args(3, new string[3] {"cond", "then", "else"}, args, flags);
					ccl_object* arg_cond = (*got)[0] ? (*got)[0] : constants::b_false();
					ccl_object* arg_then = (*got)[1] ? (*got)[1] : constants::empty_code();
					ccl_object* arg_else = (*got)[2] ? (*got)[2] : constants::empty_code();
					
					if (arg_cond->type == types::b_false()) {
						code_spec* code = (code_spec*) arg_else->value;
						fenv* f2 = new fenv(code->f);
						exec->m->load(code->p, f2, input);
						exec->hook = new push_output_hook();
					} else {
						code_spec* code = (code_spec*) arg_then->value;
						fenv* f2 = new fenv(code->f);
						exec->m->load(code->p, f2, input);
						exec->hook = new push_output_hook();
					}
					
					return NULL;
				}
		};
		
		class case_hook : public pc_hook {
		public:
				bool first = true;
				bool test_out = false;
				ccl_object* input;
				list<ccl_object*>* args;
				case_hook(ccl_object* input, list<ccl_object*>* args) :input(input), args(args) {}
				
				void call(executor* exec) override {
					if (args->empty()) {
						if (first || test_out) {
							exec->stack.push_front(input);
						} else {
							exec->stack.push_front(exec->output);
						}
						exec->hook = NULL;
						return;
					}
					
					if (test_out) {
						if (exec->output->type != types::b_false()) {
							ccl_object* arg = args->front();
							if (arg->type != types::code()) {
								throw runtime_error(string()+"Expected argument of type 'code'; got type '"+arg->type->name+"'");
							}
							code_spec* code = (code_spec*) arg->value;
							fenv* f2 = new fenv(code->f);
							exec->m->load(code->p, f2, input);
							
							exec->hook = new push_output_hook();
							return;
						}
						args->pop_front();
					} else {
						ccl_object* arg = args->front();
						args->pop_front();
						if (arg->type != types::code()) {
							throw runtime_error(string()+"Expected argument of type 'code'; got type '"+arg->type->name+"'");
						}
						code_spec* code = (code_spec*) arg->value;
						fenv* f2 = new fenv(code->f);
						exec->m->load(code->p, f2, input);
						
						if (args->empty()) {
							test_out = !test_out;
						}
					}
					
					test_out = !test_out;
					first = false;
				}
		};
		
		class proc_case : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					exec->hook = new case_hook(input, new list<ccl_object*>(*args));
					return NULL;
				}
		};
		
		class proc_loop_hook : public pc_hook {
			public:
				bool first = true;
				vector<ccl_object*>* output = new vector<ccl_object*>();
				code_spec* spec;
				int i = 0;
				int goal;
				proc_loop_hook(code_spec* spec, int goal) : spec{spec}, goal{goal} {}
				void call(executor* e) override {
					if (!first) {
						output->push_back(e->output);
					} else {
						first = false;
					}
					
					if (i >= goal) {
						e->stack.push_front(new ccl_object(types::list(), output));
						e->hook = NULL;
						return;
					}
					
					ccl_object* value = new ccl_object(types::num(), (double) i);
					fenv* f2 = new fenv(spec->f);
					e->m->load(spec->p, f2, value);
					
					i++;
				}
		};
		
		class proc_loop : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::num()) {
						throw runtime_error(string()+"Expected input of type 'number'; got type '"+input->type->name+"'");
					}
					ccl_object* arg = args->empty() ? constants::empty_code() : args->front();
					
					if (arg->type != types::code()) {
						throw runtime_error(string()+"Argument of type '"+input->type->name+"' is not executable");
					}
					
					code_spec* spec = (code_spec*) arg->value;
					exec->hook = new proc_loop_hook(spec, (int) input->num_val());
					return NULL;
				}
		};
		
		class proc_rand : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					
					if (input->type != types::num()) {
						throw runtime_error(string()+"Expected input of type 'number'; got type '"+input->type->name+"'");
					}
					if (input->num_val() <= 0) {
						return new ccl_object(types::num(), 0.0);
					}
					if (flags->find("real") != flags->end()) {
						return new ccl_object(types::num(), ((double)rand()/(double)RAND_MAX) * (input->num_val() - 1));
					}
					return new ccl_object(types::num(), (double) (rand() % (int)input->num_val()));
				}
		};
		
		class proc_filter_hook : public pc_hook {
			public:
				bool first = true;
				vector<ccl_object*>* output = new vector<ccl_object*>();
				code_spec* spec;
				ccl_iterator* iter;
				ccl_object* last_item = NULL;
				proc_filter_hook(code_spec* spec, ccl_iterator* iter) : spec{spec}, iter{iter} {}
				void call(executor* e) override {
					if (!first) {
						if (e->output->type != types::b_false()) {
							output->push_back(last_item);
						}
					} else {
						first = false;
					}
					
					if (iter->done()) {
						e->stack.push_front(new ccl_object(types::list(), output));
						e->hook = NULL;
						return;
					}
					
					ccl_object* value = iter->value();
					fenv* f2 = new fenv(spec->f);
					e->m->load(spec->p, f2, value);
					
					iter->next();
					last_item = value;
				}
		};
		
		class proc_filter : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					ccl_object* arg = args->empty() ? constants::empty_code() : args->front();
					
					if (arg->type == types::proc()) {
						proc* pr = (proc*) arg->value;
						list<ccl_object*> arglist;
						map<string, ccl_object*> flaglist;
						vector<ccl_object*>* out = new vector<ccl_object*>();
						while (!iter->done()) {
							ccl_object* item = iter->value();
							out->push_back(pr->call(item, &arglist, &flaglist, exec));
							iter->next();
						}
						return new ccl_object(types::list(), out);
					}
					
					if (arg->type != types::code()) {
						throw runtime_error(string()+"Argument of type '"+input->type->name+"' is not executable");
					}
					
					code_spec* spec = (code_spec*) arg->value;
					exec->hook = new proc_filter_hook(spec, iter);
					return NULL;
				}
		};
		
		class proc_get_key : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::key()) {
						throw runtime_error(string()+"Expected input of type 'key'; got type '"+input->type->name+"'");
					}
					key_spec* spec = (key_spec*) input->value;
					return spec->key;
				}
		};
		
		class proc_get_value : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::key()) {
						throw runtime_error(string()+"Expected input of type 'key'; got type '"+input->type->name+"'");
					}
					key_spec* spec = (key_spec*) input->value;
					return spec->value;
				}
		};
		
		class proc_join : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					
					vector<ccl_object*>* got = map_args(1, new string[1] {"sep"}, args, flags);
					ccl_object* sep = (*got)[0];
					
					string s;
					if (sep) {
						s = sep->str_val();
					}
					
					ostringstream* strs = new ostringstream();
					bool first = true;
					
					while (!iter->done()) {
						if (first) {
							first = false;
						} else {
							*strs << s;
						}
						ccl_object* item = iter->value();
						*strs << item->type->to_string(item);
						
						iter->next();
					}
					
					return new ccl_object(types::str(), (void*) strs->str().c_str());
				}
		};
		
		class proc_split : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::str()) {
						throw runtime_error(string()+"Expected input of type 'string'; got type '"+input->type->name+"'");
					}
					vector<ccl_object*>* got = map_args(1, new string[1] {"sep"}, args, flags);
					ccl_object* arg_sep = (*got)[0];
					string s = input->str_val();
					vector<ccl_object*>* v = new vector<ccl_object*>();
					
					string sep;
					if (arg_sep) {
						if (arg_sep->type != types::str()) {
							throw runtime_error(string()+"Expected argument of type 'string'; got type '"+arg_sep->type->name+"'");
						}
						
						
						sep = arg_sep->str_val();
					}
					
					if (sep.empty()) {
						for (int i = 0; i < s.size(); i++) {
							v->push_back(new ccl_object(types::str(), (void*) (new string(s.substr(i, 1)))->c_str()));
						}
					} else {
						int i = 0;
						while (1) {
							int j = s.find(sep, i);
							if (j == s.npos) {
								v->push_back(new ccl_object(types::str(), (void*) (new string(s.substr(i)))->c_str()));
								break;
							}
							v->push_back(new ccl_object(types::str(), (void*) (new string(s.substr(i, j-i)))->c_str()));
							i = j + sep.size();
						}
					}
					
					return new ccl_object(types::list(), v);
				}
		};
		
		class proc_call : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* got = map_args(3, new string[3] {"proc", "args", "flags"}, args, flags);
					ccl_object* arg_proc = (*got)[0] ? (*got)[0] : constants::b_false();
					ccl_object* arg_args = (*got)[1];
					ccl_object* arg_flags = (*got)[2];
					
					if (arg_proc->type != types::proc()) {
						throw runtime_error(string()+"argument 'proc' expects type 'proc', got type '"+arg_proc->type->name+"'");
					}
					
					if (arg_args && arg_args->type != types::list()) {
						throw runtime_error(string()+"argument 'args' expects type 'list', got type '"+arg_proc->type->name+"'");
					}
					
					if (arg_flags && arg_flags->type != types::map()) {
						throw runtime_error(string()+"argument 'flags' expects type 'map', got type '"+arg_proc->type->name+"'");
					}
					
					list<ccl_object*> nargs;
					map<string, ccl_object*> nflags;
					
					if (arg_args) {
						vector<ccl_object*>* v = (vector<ccl_object*>*) arg_args->value;
						nargs.insert(nargs.begin(), v->begin(), v->end());
					}
					
					if (arg_flags) {
						ccl_iterator* iter = arg_flags->type->iterator(arg_flags);
						while (!iter->done()) {
							key_spec* spec = (key_spec*) iter->value()->value;
							if (spec->key->type != types::str()) {
								throw runtime_error(string()+"argument 'flags' expects keys of 'string', got key of type '"+spec->key->type->name+"'");
							}
							string s = spec->key->str_val();
							nflags[s] = spec->value;
							
							iter->next();
						}
					}
					
					proc* pr = (proc*) arg_proc->value;
					return pr->call(input, &nargs, &nflags, exec);
				}
		};
		
		class proc_zip : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_iterator* iter = input->type->iterator(input);
					if (!iter) {
						throw runtime_error(string()+"Input of type '"+input->type->name+"' is not iterable");
					}
					
					list<ccl_iterator*> iters;
					
					while (!iter->done()) {
						ccl_object* item = iter->value();
						ccl_iterator* ii = item->type->iterator(item);
						if (!ii) {
							throw runtime_error(string()+"Input element of type '"+input->type->name+"' is not iterable");
						}
						iters.push_back(ii);
						iter->next();
					}
					
					vector<ccl_object*>* output = new vector<ccl_object*>();
					vector<ccl_object*>* sublist = new vector<ccl_object*>();
					bool all_end;
					do {
						all_end = true;
						
						for (list<ccl_iterator*>::iterator ii = iters.begin(); ii != iters.end(); ii++) {
							ccl_iterator* jj = *ii;
							if (jj->done()) {
								sublist->push_back(constants::b_false());
							} else {
								sublist->push_back(jj->value());
								jj->next();
								all_end = false;
							}
						}
						
						if (!all_end) {
							output->push_back(new ccl_object(types::list(), sublist));
							sublist = new vector<ccl_object*>();
						}
					} while (!all_end);
					
					return new ccl_object(types::list(), output);
				}
		};
		
		class proc_lower : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::str()) {
						throw runtime_error(string()+"Expected input of type 'string'; got type '"+input->type->name+"'");
					}
					string* s = new string(input->str_val());
					transform(s->begin(), s->end(), s->begin(), ::tolower);
					return new ccl_object(types::str(), (void*) s->c_str());
				}
		};
		
		class proc_upper : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::str()) {
						throw runtime_error(string()+"Expected input of type 'string'; got type '"+input->type->name+"'");
					}
					string* s = new string(input->str_val());
					transform(s->begin(), s->end(), s->begin(), ::toupper);
					return new ccl_object(types::str(), (void*) s->c_str());
				}
		};
		
		class proc_try_trap : public machine_trap {
			public:
				code_spec* catch_spec;
				code_spec* finally_spec;
				executor* original_exec;
				proc_try_trap(executor* original_exec, code_spec* catch_spec, code_spec* finally_spec) : original_exec(original_exec), catch_spec(catch_spec), finally_spec(finally_spec) {}
				bool trap(machine* m, string msg) override {
					list<executor*> calls = m->callstack;
					for (list<executor*>::iterator ii = calls.begin(); ii != calls.end(); ii++) {
						if (*ii == original_exec) {
							break;
						}
						m->callstack.pop_front();
					}
					
					fenv* f2 = new fenv(catch_spec->f);
					if (!m->callstack.empty()) m->callstack.front()->hook = new push_output_hook();
					m->load(catch_spec->p, f2, new ccl_object(types::str(), (void*) msg.c_str()));
					
					return true;
				}
		};
		
		class proc_try : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* got = map_args(3, new string[3] {"try", "catch", "finally"}, args, flags);
					ccl_object* arg_try = (*got)[0] ? (*got)[0] : constants::empty_code();
					ccl_object* arg_catch = (*got)[1] ? (*got)[1] : constants::empty_code();
					ccl_object* arg_finally = (*got)[2] ? (*got)[2] : constants::empty_code();
					
					if (arg_try->type != types::code()) {
						throw runtime_error(string()+"argument 'try' expects type 'code', got type '"+arg_try->type->name+"'");
					}
					if (arg_catch->type != types::code()) {
						throw runtime_error(string()+"argument 'catch' expects type 'code', got type '"+arg_catch->type->name+"'");
					}
					if (arg_finally->type != types::code()) {
						throw runtime_error(string()+"argument 'finally' expects type 'code', got type '"+arg_finally->type->name+"'");
					}
					
					code_spec* try_spec = (code_spec*) arg_try->value;
					code_spec* catch_spec = (code_spec*) arg_catch->value;
					code_spec* finally_spec = (code_spec*) arg_finally->value;
					
					fenv* f2 = new fenv(try_spec->f);
					exec->m->load(try_spec->p, f2, input);
					exec->hook = new push_output_hook();
					exec->m->traps.push_front(new proc_try_trap(exec, catch_spec, finally_spec));
					return NULL;
				}
		};
		
		class proc_throw : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					throw runtime_error(input->type->to_string(input));
				}
		};
		
		class proc_to_str : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return new ccl_object(types::str(), (void*) input->type->to_string(input).c_str());
				}
		};
		
		class proc_to_num : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					const char* s = input->type->to_string(input).c_str();
					char* end;
					double d = strtod(s, &end);
					if (end == s) {
						return constants::b_false();
					}
					return new ccl_object(types::num(), d);
				}
		};
		
		class proc_type : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					return new ccl_object(types::str(), (void*) input->type->name);
				}
		};
		
		class proc_replace : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					if (input->type != types::str()) {
						throw runtime_error(string()+"Expected input of type 'string'; got type '"+input->type->name+"'");
					}
					
					vector<ccl_object*>* got = map_args(2, new string[2] {"from", "to"}, args, flags);
					ccl_object* arg_from = (*got)[0] ? (*got)[0] : constants::b_false();
					ccl_object* arg_to = (*got)[1] ? (*got)[1] : constants::b_false();
					
					if (arg_from->type != types::str()) {
						throw runtime_error(string()+"Expected argument 'from' to be type 'string'; got type '"+arg_from->type->name+"'");
					}
					if (arg_to->type != types::str()) {
						throw runtime_error(string()+"Expected argument 'to' to be type 'string'; got type '"+arg_to->type->name+"'");
					}
					
					string s = input->str_val();
					string from = arg_from->str_val();
					string to = arg_to->str_val();
					list<string*> v;
					
					if (from.empty()) {
						return input;
					}
					
					int i = 0;
					while (1) {
						int j = s.find(from, i);
						if (j == s.npos) {
							v.push_back(new string(s.substr(i)));
							break;
						}
						v.push_back(new string(s.substr(i, j-i)));
						i = j + from.size();
					}
					
					ostringstream strs;
					bool first = true;
					for (list<string*>::iterator ii = v.begin(); ii != v.end(); ii++) {
						if (first) {
							first = false;
						} else {
							strs << to;
						}
						
						strs << **ii;
					}
					return new ccl_object(types::str(), (void*) (new string(strs.str()))->c_str());
				}
		};
		
		class proc_repeat_hook : public pc_hook {
		public:
				list<arg_spec>* init_args;
				list<ccl_object*> args;
				code_spec* code;
				ccl_object* input;
				bool first = true;
				
				proc_repeat_hook(list<arg_spec>* init_args, code_spec* code, ccl_object* input) : init_args(init_args), code(code), input(input) {
					for (list<arg_spec>::iterator ii = init_args->begin(); ii != init_args->end(); ii++) {
						args.push_back(ii->default_value);
					}
				}
				
				void call(executor* e) override {
					
					if (!first) {
						if (e->output->type == types::repeat()) {
							args.clear();
							vector<ccl_object*>* items = (vector<ccl_object*>*) e->output->value;
							args.insert(args.begin(), items->begin(), items->end());
						} else {
							e->stack.push_front(e->output);
							e->hook = NULL;
							return;
						}
					} else {
						first = false;
					}
					
					fenv* f2 = new fenv(code->f);
					list<ccl_object*>::iterator jj = args.begin();
					for (list<arg_spec>::iterator ii = init_args->begin(); ii != init_args->end(); ii++) {
						if (jj != args.end()) {
							f2->set(ii->name, *jj);
							jj++;
						} else {
							break;
						}
					}
					e->m->load(code->p, f2, input);
				}
		};
		
		class proc_repeat : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					ccl_object* arg;
					list<arg_spec>* f_args = new list<arg_spec>();
					code_spec* f_code = NULL;
					
					if (args->empty()) {
						throw runtime_error("expected one argument of type 'code'; got zero");
					}
					
					if (flags->find("do") != flags->end()) {
						f_code = (code_spec*) (*flags)["do"]->value;
					}
					
					while (!args->empty()) {
						arg = args->front();
						args->pop_front();
						
						if (arg->type == types::str()) {
							f_args->push_back(arg_spec(arg->str_val(), constants::b_false(), false));
						} else if (arg->type == types::arg()) {
							f_args->push_back(*((arg_spec*) arg->value));
						} else if (arg->type == types::code()) {
							if (f_code) {
								throw runtime_error("expected one argument of type 'code'; got multiple");
							}
							f_code = (code_spec*) arg->value;
						} else {
							throw runtime_error(string("expected argument of type 'string'/'arg'/'code'; got type '")+arg->type->name+"'");
						}
					}
					
					if (!f_code) {
						throw runtime_error("expected one argument of type 'code'; got zero");
					}
					
					exec->hook = new proc_repeat_hook(f_args, f_code, input);
					return NULL;
				}
		};
		
		class proc_recur : public proc {
			public:
				ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override {
					vector<ccl_object*>* vec = new vector<ccl_object*>(args->size());
					int i = 0;
					for (list<ccl_object*>::iterator ii = args->begin(); ii != args->end(); ii++) {
						(*vec)[i] = *ii;
						i++;
					}
					return new ccl_object(types::repeat(), vec);
				}
		};
		
		void register_base_procs() {
			register_proc("dump", new proc_dump());
			
			register_proc("pass", new proc_pass());
			register_proc("print", new proc_print());
			register_proc("input", new proc_input());
			register_proc("proc", new proc_proc());
			register_proc("arg", new proc_arg());
			register_proc("flag", new proc_flag());
			register_proc("var", new proc_var());
			register_proc("local", new proc_local());
			register_proc("let", new proc_let());
			register_proc("if", new proc_if());
			register_proc("case", new proc_case());
			register_proc("call", new proc_call());
			register_proc("try", new proc_try());
			register_proc("throw", new proc_throw());
			register_proc("repeat", new proc_repeat());
			register_proc("recur", new proc_recur());
			
			register_proc("add", new proc_add());
			register_proc("sub", new proc_sub());
			register_proc("mul", new proc_mul());
			register_proc("div", new proc_div());
			register_proc("mod", new proc_mod());
			
			register_proc("eq", new proc_eq());
			register_proc("neq", new proc_neq());
			register_proc("lt", new proc_lt());
			register_proc("le", new proc_le());
			register_proc("gt", new proc_gt());
			register_proc("ge", new proc_ge());
			
			register_proc("and", new proc_and());
			register_proc("or", new proc_or());
			register_proc("not", new proc_not());
			
			register_proc("rand", new proc_rand());
			
			register_proc("true", new proc_true());
			register_proc("false", new proc_false());
			register_proc("list", new proc_list());
			register_proc("map", new proc_map());
			register_proc("key", new proc_key());
			
			register_proc("type", new proc_type());
			register_proc("to-num", new proc_to_num());
			
			register_proc("get", new proc_undef());
			register_proc("set", new proc_undef());
			register_proc("has", new proc_undef());
			register_proc("get-key", new proc_get_key());
			register_proc("get-value", new proc_get_value());
			
			register_proc("size", new proc_undef());
			register_proc("empty", new proc_undef());
			register_proc("append", new proc_undef());
			register_proc("prepend", new proc_prepend());
			register_proc("find", new proc_undef());
			register_proc("copy", new proc_copy());
			register_proc("sort", new proc_undef());
			register_proc("sort!", new proc_undef());
			register_proc("add!", new proc_undef());
			register_proc("append!", new proc_undef());
			register_proc("prepend!", new proc_prepend_bang());
			register_proc("reverse", new proc_undef());
			register_proc("reverse!", new proc_undef());
			
			register_proc("join", new proc_join());
			register_proc("split", new proc_split());
			register_proc("lower", new proc_lower());
			register_proc("upper", new proc_upper());
			register_proc("replace", new proc_replace());
			
			register_proc("head", new proc_undef());
			register_proc("tail", new proc_undef());
			register_proc("behead", new proc_undef());
			register_proc("curtail", new proc_undef());
			register_proc("behead!", new proc_undef());
			register_proc("curtail!", new proc_undef());
			
			register_proc("each", new proc_each());
			register_proc("pairs", new proc_pairs());
			register_proc("fold", new proc_fold());
			register_proc("loop", new proc_loop());
			register_proc("filter", new proc_filter());
			register_proc("zip", new proc_zip());
		}
	}
}