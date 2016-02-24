
/* 
 * File:   lexer.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 4:24 PM
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <list>
#include <map>
#include <string>

#include "token.hpp"
#include "expression.hpp"
#include "instruction.hpp"

namespace ccl {
	using namespace std;
	
	list<instruction*>* lex(list<token*>* tokens);
}

#endif /* LEXER_HPP */

