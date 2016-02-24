
/* 
 * File:   token.cpp
 * Author: iconmaster
 * 
 * Created on January 20, 2016, 3:13 PM
 */

#include "token.hpp"

namespace ccl {
	using namespace std;
	
	token::token(string file, int begin) {
		range = source_range(file, begin);
	}
	
	token::token(token_type type, string data, source_range range) : type{type}, data{data}, range{range} {}

	token::~token() {
		
	}
}