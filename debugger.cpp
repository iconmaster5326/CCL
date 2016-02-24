
/* 
 * File:   debugger.cpp
 * Author: iconmaster
 *
 * Created on February 20, 2016, 8:55 PM
 */

#include "debugger.hpp"

#include <iostream>
#include <sstream>

namespace ccl {
	using namespace std;
	
	static const char* tt_names[] = {"NONE", "WORD", "NUM", "VAR", "OPT", "STR", "STREX", "LB", "RB", "LCB", "RCB", "PIPE", "EACH", "SEMI"};
	static const char* op_names[] = {";", "|", "&"};
	static const char* e_names[] = {"NONE", "CODE", "EXPR", "STR", "STREX", "NUM", "VAR", "TRUE"};
	static const char* cmd_names[] = {"NOP", "RESET", "CALL", "EMIT", "LOAD", "PUSHF", "ADDF", "POPF", "CONCAT", "PICK", "ECODE", "DROP"};

	static string input;
	static machine* m;
	
	static source_range get_range() {
		executor* e = m->callstack.front();
		list<operation>::const_iterator ii = e->pos;
		while ((*ii).op != CMD_CALL) {
			if (ii == e->p->code.cend()) {
				list<operation>::const_reverse_iterator jj = e->p->code.crbegin();
				
				while ((*jj).op != CMD_CALL) {
					if (jj == e->p->code.crend()) {
						return source_range("<unknown>", -1);
					}
					
					jj++;
				}
				
				return (*jj).range;
			}
			
			ii++;
		}
		
		return (*ii).range;
	}
	
	static string debug_string(ccl_object* arg) {
		return string()+"["+arg->type->name+":"+arg->type->to_string(arg)+"]";
	}
	
	static string op_string(program* p, operation& op) {
		ostringstream strs;
		strs << cmd_names[op.op];
		switch (op.op) {
			case CMD_ECODE:
			case CMD_EMIT: {
				strs << " " << debug_string(p->consts[op.n]);
				break;
			}
			case CMD_CALL: {
				strs << " " << op.s << " " << op.n;
				break;
			}
			case CMD_PICK: {
				strs << " " << op.n;
				break;
			}
			case CMD_ADDF:
			case CMD_LOAD: {
				strs << " " << op.s;
				break;
			}
		}
		return strs.str();
	}
	
	void begin_debugger(string _input, machine* _m) {
		input = _input; m = _m;
		string brk;
		
		while (1) {
			source_range r = get_range();
			cout << r.file << ":" << r.line << "> ";
			string s;
			getline(cin, s);
			if (s.find("l") == 0) {
				executor* e = m->callstack.front();
				program* p = e->p;
				for (list<operation>::const_iterator i = p->code.begin(); i != p->code.end(); i++) {
					operation op = *i;
					cout << (e->pos == i ? ">>> " : "    ") << op_string(p, op) << endl;
				}
			} else if (s.find("i") == 0) {
				executor* e = m->callstack.front();
				
				cout << "STACK: ";
				for (list<ccl_object*>::iterator ii = e->stack.begin(); ii != e->stack.end(); ii++) {
					cout << debug_string(*ii) << " ";
				}
				cout << endl;
				
				cout << "FLAGS: ";
				for (map<string,ccl_object*>::iterator ii = e->flags.begin(); ii != e->flags.end(); ii++) {
					cout << ii->first << "=" << debug_string(ii->second) << " ";
				}
				cout << endl;
				
				cout << "HOOK? " << (e->hook ? "yes" : "no") << endl;
			} else if (s.find("s") == 0) {
				try {
					if (m->step()) {
						cout << "program ended" << endl;
					}
				} catch (exception& e) {
					cerr << "ERROR: " << e.what() << endl;
				}
			} else if (s.find("b") == 0) {
				brk = s.substr(2);
			} else if (s.find("r") == 0) {
				try {
					while (1) {
						if (m->step()) {
							cout << "program ended" << endl;
							break;
						}

						if (m->callstack.front()->pos != m->callstack.front()->p->code.end()) {
							operation op = (*m->callstack.front()->pos);
							if (op.op == CMD_CALL && op.s.compare(brk) == 0) {
								cout << "breakpoint reached" << endl;
								break;
							}
						}
					}
				} catch (exception& e) {
					cerr << "ERROR: " << e.what() << endl;
				}
			} else if (s.find("q") == 0) {
				return;
			} 
		}
	}
}