
/* 
 * File:   compiler.cpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 2:57 PM
 */

#include <cctype>
#include <sstream>
#include <stdexcept>

#include "compiler.hpp"
#include "parser.hpp"
#include "lexer.hpp"

namespace ccl {
	using namespace std;
	
	static void compile_inst(list<instruction*>* code, program* p);
	
	static bool compile_expr(expression* e, program* p, int pick_value) {
		switch (e->type) {
			case E_CODE: {
				int cid = p->consts.size();
				program* p2 = compile((list<instruction*>*) e->value.as_list);
				p->consts.push_back(new ccl_object(types::code(), (void*) p2));
				
				p->code.push_back(operation(CMD_ECODE, cid));
				break;
			}
			case E_EXPR: {
				p->code.push_back(operation(CMD_PICK, pick_value));
				compile_inst((list<instruction*>*) e->value.as_list, p);
				break;
			}
			case E_NUM: {
				int cid = p->consts.size();
				p->consts.push_back(new ccl_object(types::num(), (void*) e->value.as_str));
				p->code.push_back(operation(CMD_EMIT, cid));
				break;
			}
			case E_STR: {
				int cid = p->consts.size();
				p->consts.push_back(new ccl_object(types::str(), (void*) e->value.as_str));
				p->code.push_back(operation(CMD_EMIT, cid));
				break;
			}
			case E_STREX: {
				
				string str = string(e->value.as_str);
				if (str.empty()) {
					int cid = p->consts.size();
					p->consts.push_back(new ccl_object(types::str(), (void*) ""));
					p->code.push_back(operation(CMD_EMIT, cid));
					break;
				}
				string* buf = new string();
				bool first = true;
				int pos = 0;
				for (int i = 0; i < str.size(); i++) {
					char c = str[i];
					if (c == '$') {
						i++;
						if (i >= str.size()) {
							ostringstream strs;
							strs << "File " << "<string>" << ", line " << "1" << ": Unexpected end of format string";
							throw runtime_error(strs.str());
						}
						c = str[i];
						if (c == '$') {
							*buf += c;
						} else {
							if (!buf->empty()) {
								int cid = p->consts.size();
								p->consts.push_back(new ccl_object(types::str(), (void*) buf->c_str()));
								p->code.push_back(operation(CMD_EMIT, cid));
								buf = new string(); pos++;
								if (first) {
									first = false;
								} else {
									p->code.push_back(operation(CMD_CONCAT));
									pos--;
								}
							}
							
							if (c == '[') {
								p->code.push_back(operation(CMD_PICK, pick_value + pos));
								int depth = 0;
								i++;
								
								while (i < str.size()) {
									c = str[i];
									
									if (c == '[') {
										depth++;
									} else if (c == ']') {
										depth--;
										if (depth == -1) {
											break;
										}
									}

									*buf += c;

									i++;
								}
								
								if (i >= str.size()) {
									ostringstream strs;
									strs << "File " << "<string>" << ", line " << "1" << ": Unclosed square bracket in format string";
									throw runtime_error(strs.str());
								}
								
								compile_inst(lex(parse("<string>", *buf)), p);
								buf = new string(); pos++;
								if (first) {
									first = false;
								} else {
									p->code.push_back(operation(CMD_CONCAT));
									pos--;
								}
							} else if (c == '{') {
								i++;
								
								while (i < str.size()) {
									c = str[i];
									
									if (c == '}') {
										break;
									}

									*buf += c;

									i++;
								}
								
								if (i >= str.size()) {
									ostringstream strs;
									strs << "File " << "<string>" << ", line " << "1" << ": Unclosed curly bracket in format string";
									throw runtime_error(strs.str());
								}
								
								p->code.push_back(operation(CMD_LOAD, 0, buf->c_str()));
								buf = new string(); pos++;
								if (first) {
									first = false;
								} else {
									p->code.push_back(operation(CMD_CONCAT));
									pos--;
								}
							} else {
								while (i < str.size()) {
									if (isspace(c)) {
										i--;
										break;
									}

									*buf += c;

									i++; c = str[i];
								}
								
								p->code.push_back(operation(CMD_LOAD, 0, buf->c_str()));
								buf = new string(); pos++;
								if (first) {
									first = false;
								} else {
									p->code.push_back(operation(CMD_CONCAT));
									pos--;
								}
							}
						}
					} else {
						*buf += c;
					}
				}
				
				if (!buf->empty()) {
					int cid = p->consts.size();
					p->consts.push_back(new ccl_object(types::str(), (void*) buf->c_str()));
					p->code.push_back(operation(CMD_EMIT, cid));
					buf = new string(); pos++;
					if (first) {
						first = false;
					} else {
						p->code.push_back(operation(CMD_CONCAT));
						pos--;
					}
				}
				
						
				break;
			}
			case E_TRUE: {
				int cid = p->consts.size();
				p->consts.push_back(new ccl_object(types::b_true(), (void*) NULL));
				p->code.push_back(operation(CMD_EMIT, cid));
				break;
			}
			case E_VAR: {
				p->code.push_back(operation(CMD_LOAD, 0, e->value.as_str));
				break;
			}
			case E_FLAG: {
				compile_expr(e->value.as_fp.expr, p, pick_value);
				p->code.push_back(operation(CMD_ADDF, 0, e->value.as_fp.name));
				return false;
			}
		}
		
		return true;
	}
	
	static void compile_inst(list<instruction*>* code, program* p) {
		
		for (list<instruction*>::const_iterator i = code->begin(); i != code->end(); i++) {
			instruction* inst = *i;
			
			if (inst->cmd.empty() && inst->args.size() > 1) {
				ostringstream strs;
				strs << "File " << inst->range.file << ", line " << inst->range.line << ": Cannot emit more than 1 constant value";
				throw runtime_error(strs.str());
			}
			
			if (inst->type == OP_EACH) {
				inst->type = OP_PIPE;
				int cid = p->consts.size();
				list<instruction*>* code2 = new list<instruction*>();
				code2->push_front(inst);
				program* p2 = compile(code2);
				p->consts.push_back(new ccl_object(types::code(), (void*) p2));
				p->code.push_back(operation(CMD_ECODE, cid));
				p->code.push_back(operation(CMD_CALL, 1, "each", inst->range));
				inst->type = OP_EACH;
			} else if (inst->cmd.empty()) {
				if (inst->type == OP_CLEAR) {
					p->code.push_back(operation(CMD_RESET));
				}
				
				if (inst->args.size() == 1) {
					expression* arg = inst->args.front();
					if (arg->type != E_EXPR && arg->type != E_STREX) {
						p->code.push_back(operation(CMD_DROP));
					}
					compile_expr(arg, p, 0);
				}
			} else {
				if (inst->type == OP_CLEAR && !p->code.empty()) {
					p->code.push_back(operation(CMD_RESET));
				}
				
				if (inst->nflags) {
					p->code.push_back(operation(CMD_PUSHF));
				}

				int pos = 0;
				
				for (list<expression*>::const_iterator ii = inst->args.begin(); ii != inst->args.end(); ii++) {
					if (compile_expr(*ii, p, pos))
						pos++;
				}
				
				p->code.push_back(operation(CMD_CALL, inst->nargs, inst->cmd.c_str(), inst->range));
				
				if (inst->nflags) {
					p->code.push_back(operation(CMD_POPF));
				}
			}
		}
	}
	
	program* compile(list<instruction*>* code) {
		program* p = new program();
		compile_inst(code, p);
		return p;
	}
}