
/* 
 * File:   parser.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 3:27 PM
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>

#include "token.hpp"

namespace ccl {
	using namespace std;
	
	list<token*>* parse(string input);
	list<token*>* parse(string file, string input);
}

#endif /* PARSER_HPP */

