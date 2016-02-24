
/* 
 * File:   lexer.cpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 4:24 PM
 */

#include <cstdlib>
#include <sstream>
#include <stdexcept>

#include "lexer.hpp"

namespace ccl {
	using namespace std;
	
	list<instruction*>* lex(list<token*>* tokens) {
		list<instruction*>* a = new list<instruction*>();
		instruction* inst = new instruction();
		bool null_inst = true;
		bool has_opt = false; string opt; 
		
		for (list<token*>::const_iterator i = tokens->begin(); i != tokens->end(); i++) {
			token* t = *i;
			switch (t->type) {
				case TT_EACH: {
					if (!null_inst) {
						if (has_opt) {
							inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), new expression(E_TRUE)));
							inst->nflags++;
							has_opt = false;
						}
						a->push_back(inst);
						inst = new instruction();
						null_inst = true;
					}
					
					inst->type = OP_EACH; inst->range = t->range;
					null_inst = false;
					break;
				}
				case TT_PIPE: {
					if (!null_inst) {
						if (has_opt) {
							inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), new expression(E_TRUE)));
							inst->nflags++;
							has_opt = false;
						}
						a->push_back(inst);
						inst = new instruction();
						null_inst = true;
					}
					
					inst->type = OP_PIPE; inst->range = t->range;
					break;
				}
				case TT_SEMI: {
					if (!null_inst) {
						if (has_opt) {
							inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), new expression(E_TRUE)));
							inst->nflags++;
							has_opt = false;
						}
						a->push_back(inst);
						inst = new instruction();
						null_inst = true;
					}
					
					inst->type = OP_CLEAR; inst->range = t->range;
					null_inst = false;
					break;
				}
				case TT_WORD: {
					if (inst->cmd.empty()) {
						inst->cmd = t->data;
						inst->range = t->range;
					} else {
						expression* e = new expression(E_STR, t->data);
						if (has_opt) {
							inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
							inst->nflags++;
							has_opt = false;
						} else {
							inst->args.push_back(e);
							inst->nargs++;
						}
					}
					null_inst = false;
					break;
				}
				case TT_OPT: {
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), new expression(E_TRUE)));
						inst->nflags++;
					}
					opt = t->data;
					has_opt = true;
					null_inst = false;
					if (inst->cmd.empty()) inst->range = t->range;
					break;
				}
				case TT_STR: {
					expression* e = new expression(E_STR, t->data);
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = t->range;
					break;
				}
				case TT_STREX: {
					expression* e = new expression(E_STREX, t->data);
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = t->range;
					break;
				}
				case TT_NUM: {
					expression* e = new expression(E_NUM, strtod(t->data.c_str(), NULL));
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = t->range;
					break;
				}
				case TT_VAR: {
					expression* e = new expression(E_VAR, t->data);
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = t->range;
					break;
				}
				case TT_LB: {
					int depth = 0;
					list<token*> ts = list<token*>();
					i++;
					
					token* ot = t;
					
					while (i != tokens->end()) {
						t = *i;
						if (t->type == TT_LB) {
							depth++;
							ts.push_back(t);
						} else if (t->type == TT_RB) {
							depth--;
							if (depth == -1) {
								break;
							} else {
								ts.push_back(t);
							}
						} else {
							ts.push_back(t);
						}
						i++;
					}
					
					if (i == tokens->end()) {
						ostringstream strs;
						strs << "File " << ot->range.file << ", line " << ot->range.line << ": Opening square bracket unmatched";
						throw runtime_error(strs.str());
					}
					
					expression* e = new expression(E_EXPR, (list<void*>*) (lex(&ts)));
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = ot->range;
					
					break;
				}
				case TT_LCB: {
					int depth = 0;
					list<token*> ts = list<token*>();
					i++;
					
					token* ot = t;
					
					while (i != tokens->end()) {
						t = *i;
						if (t->type == TT_LCB) {
							depth++;
							ts.push_back(t);
						} else if (t->type == TT_RCB) {
							depth--;
							if (depth == -1) {
								break;
							} else {
								ts.push_back(t);
							}
						} else {
							ts.push_back(t);
						}
						i++;
					}
					
					if (i == tokens->end()) {
						ostringstream strs;
						strs << "File " << ot->range.file << ", line " << ot->range.line << ": Opening curly bracket unmatched";
						throw runtime_error(strs.str());
					}
					
					expression* e = new expression(E_CODE, (list<void*>*) (lex(&ts)));
					if (has_opt) {
						inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), e));
						inst->nflags++;
						has_opt = false;
					} else {
						inst->args.push_back(e);
						inst->nargs++;
					}
					null_inst = false;
					if (inst->cmd.empty()) inst->range = ot->range;
					
					break;
				}
				case TT_RB: {
					ostringstream strs;
					strs << "File " << t->range.file << ", line " << t->range.line << ": Closing square bracket unmatched";
					throw runtime_error(strs.str());
				}
				case TT_RCB: {
					ostringstream strs;
					strs << "File " << t->range.file << ", line " << t->range.line << ": Closing curly bracket unmatched";
					throw runtime_error(strs.str());
				}
			}
		}
		
		if (!null_inst) {
			if (has_opt) {
				inst->args.push_back(new expression(E_FLAG, (char*) opt.c_str(), new expression(E_TRUE)));
				inst->nflags++;
				has_opt = false;
			}
			a->push_back(inst);
			inst = new instruction();
			null_inst = true;
		}
		
		return a;
	}
}
