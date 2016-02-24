
/* 
 * File:   token.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 3:13 PM
 */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

#include "source_range.hpp"

namespace ccl {
	using namespace std;

	typedef enum {
		TT_NONE, TT_WORD, TT_NUM, TT_VAR, TT_OPT, TT_STR, TT_STREX, TT_LB, TT_RB, TT_LCB, TT_RCB, TT_PIPE, TT_EACH, TT_SEMI
	} token_type;

	class token {
		public:
			token_type type = TT_NONE;
			string data = string("");
			source_range range = source_range("?", -1);
			
			token(string file, int begin);
			token(token_type type, string data, source_range range);
			~token();
		private:

	};
}

#endif /* TOKEN_HPP */

