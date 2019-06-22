/*
 * lexer.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_LEXER_HPP_
#define INCLUDE_CCL_LEXER_HPP_

#include <iostream>
#include <string>

#include "ccl/error.hpp"

namespace ccl {
	class Token {
	public:
		enum class Type {
			NONE,
			WORD,
			STRING, // '...'
			EX_STRING, // "..."
			VAR, // $...
			FLAG, // -...
			PIPE, // |
			SEMICOLON, // ;
			AND, // &
			LPAREN, // ()
			RPAREN,
			LBRACE, // {}
			RBRACE,
			LBRACKET, // []
			RBRACKET,
			ELLIPSES, // a literal set of three dots
		};
		
		Type type;
		std::string value;
		Source source;
	};
	
	class Lexer {
	public:
		Lexer(const std::string& location, std::istream& input);
		Lexer(const std::string& location, const std::string& input);
		~Lexer();
		
		Token next();
		Token& peek();
		bool done();
	private:
		const std::string& location;
		bool inputOwned, peeked;
		std::istream* input;
		int line = 1;
		int col = 1;
		Token tokenPeeked;
		
		void advance(char c);
		void toNextWS();
	};
}

#endif /* INCLUDE_CCL_LEXER_HPP_ */
