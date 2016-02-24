
/* 
 * File:   parser.cpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 3:27 PM
 */

#include <cctype>
#include <sstream>
#include <stdexcept>

#include "parser.hpp"

namespace ccl {
	using namespace std;
	
	list<token*>* parse(string input) {
		return parse("?", input);
	}
	
	list<token*>* parse(string file, string input) {
		list<token*>* a = new list<token*>();
		int input_len = input.size();
		token* t = new token(file, 1);
		int line = 1;
		
		for (int i = 0; i < input_len; i++) {
			char c = input[i];
			if (c == '\n') line++;
			token_type to_add = TT_NONE;
			
			switch (c) {
				case '\'':
				case '\"': {
					if (!t->data.empty()) {
						a->push_back(t);
						t = new token(file, line);
					}
					i++; if (input[i] == '\n') line++;
					
					while (i < input_len && input[i] != c) {
						if (input[i] == '\\') {
							switch (input[i]) {
								case 'n': {
									t->data = t->data + '\n';
									break;
								}
								case 't': {
									t->data = t->data + '\t';
									break;
								}
								default: {
									t->data = t->data + input[i];
									break;
								}
							}
						} else {
							t->data = t->data + input[i];
						}
						i++; if (input[i] == '\n') line++;
					}
					
					if (i >= input_len) {
						ostringstream strs;
						strs << "File " << t->range.file << ", line " << t->range.line << ": String not closed";
						throw runtime_error(strs.str());
					}
					
					t->type = c == '\'' ? TT_STR : TT_STREX;
					a->push_back(t);
					t = new token(file, line);
					break;
				}
				case '|': {
					to_add = TT_PIPE;
					break;
				}
				case '&': {
					to_add = TT_EACH;
					break;
				}
				case ';': {
					to_add = TT_SEMI;
					break;
				}
				case '[': {
					to_add = TT_LB;
					break;
				}
				case ']': {
					to_add = TT_RB;
					break;
				}
				case '{': {
					to_add = TT_LCB;
					break;
				}
				case '}': {
					to_add = TT_RCB;
					break;
				}
				default: {
					if (isspace(c)) {
						if (!t->data.empty()) {
							a->push_back(t);
							t = new token(file, line);
						}
					} else if ((t->type == TT_NONE || t->type == TT_NUM) && (isdigit(c) || c == '.')) {
						t->type = TT_NUM;
						t->data = t->data + c;
					} else if (t->type == TT_NONE && c == '-') {
						if (i != input.size() - 1 && (isdigit(input[i+1]) || input[i+1] == '.')) {
							t->type = TT_NUM;
							t->data += c;
						} else {
							t->type = TT_OPT;
						}
					} else if (t->type == TT_NONE && c == '$') {
						t->type = TT_VAR;
					} else if (c == '/') {
						if (i < input_len - 1 && input[i+1] == '/') {
							i+=2;
							while (i < input_len) {
								if (input[i] == '\n') {
									line++;
									break;
								}
								i++;
							}
						} else {
							t->data = t->data + c;
						}
					} else if (t->type == TT_NONE || t->type == TT_WORD) {
						t->type = TT_WORD;
						t->data = t->data + c;
					} else {
						t->data = t->data + c;
					}
					break;
				}
			}
			
			if (to_add != TT_NONE) {
				if (!t->data.empty()) {
					a->push_back(t);
					t = new token(file, line);
				}
				
				t->type = to_add;
				a->push_back(t);
				t = new token(file, line);
			}
		}
		
		if (!t->data.empty()) {
			a->push_back(t);
		}
		
		return a;
	}
}