/*
 * lexer.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include <cctype>
#include <sstream>

#include "ccl/lexer.hpp"

using namespace std;
using namespace ccl;

static const string SPECIAL_CHARS = "$|&;(){}[]\"\'";

ccl::Lexer::Lexer(const std::string& location, std::istream& input) : location{location}, inputOwned(false), input(&input) {
	toNextWS();
}

ccl::Lexer::Lexer(const std::string& location, const std::string& input) : location{location}, inputOwned(true), input(new std::istringstream(input)) {
	toNextWS();
}

ccl::Lexer::~Lexer() {
	if (inputOwned) delete input;
}

void Lexer::advance(char c) {
	if (c == '\n') {
		line++;
		col = 1;
	} else {
		col++;
	}
}

Token ccl::Lexer::next() {
	if (done()) return {Token::Type::NONE, "", {location, line, col}};
	
	noskipws(*input);
	Token result;
	char c; *input >> c;
	
	switch (c) {
	case '|': result = {Token::Type::PIPE, "|", {location, line, col}}; break;
	case '&': result = {Token::Type::AND, "&", {location, line, col}}; break;
	case ';': result = {Token::Type::SEMICOLON, ";", {location, line, col}}; break;
	case '(': result = {Token::Type::LPAREN, "(", {location, line, col}}; break;
	case ')': result = {Token::Type::RPAREN, ")", {location, line, col}}; break;
	case '{': result = {Token::Type::LBRACE, "{", {location, line, col}}; break;
	case '}': result = {Token::Type::RBRACE, "}", {location, line, col}}; break;
	case '[': result = {Token::Type::LBRACKET, "[", {location, line, col}}; break;
	case ']': result = {Token::Type::RBRACKET, "]", {location, line, col}}; break;
	case '-': {
		ostringstream word;
		char c = input->peek();
		
		while (!done() && !isspace(c) && SPECIAL_CHARS.find(c) == string::npos) {
			word << c;
			
			*input >> c; advance(c);
			c = input->peek();
		}
		
		string r = word.str();
		if (r.empty())
			result = {Token::Type::WORD, "-", {location, line, col}};
		else {
			try {
				stod(r);
				result = {Token::Type::WORD, "-"+r, {location, line, col}}; // really bad hack for negative number constants
			} catch (const invalid_argument& ex) {
				result = {Token::Type::FLAG, r, {location, line, col}};
			} catch (const out_of_range& ex) {
				throw exception();
			}
		}
	} break;
	case '$': {
		ostringstream word;
		char c = input->peek();
		
		while (!done() && !isspace(c) && SPECIAL_CHARS.find(c) == string::npos) {
			word << c;
			
			*input >> c; advance(c);
			c = input->peek();
		}
		
		string r = word.str();
		if (r.empty())
			result = {Token::Type::WORD, "$", {location, line, col}};
		else
			result = {Token::Type::VAR, r, {location, line, col}};
	} break;
	case '\'': {
		ostringstream word;
		char c = input->peek();
		bool escape = false;
		
		while (!done() && (escape || c != '\'')) {
			if (escape) {
				escape = false;
				switch (c) {
				case 'n': word << '\n'; break;
				case 't': word << '\t'; break;
				case 'b': word << '\b'; break;
				case 'r': word << '\r'; break;
				default: word << c;
				}
			} else if (c == '\\') {
				escape = true;
			} else {
				word << c;
			}
			
			*input >> c; advance(c);
			c = input->peek();
		}
		
		*input >> c; advance(c);
		result = {Token::Type::STRING, word.str(), {location, line, col}};
	} break;
	case '\"': {
		ostringstream word;
		char c = input->peek();
		bool escape = false;
		
		while (!done() && (escape || c != '\"')) {
			if (escape) {
				escape = false;
				switch (c) {
				case 'n': word << '\n'; break;
				case 't': word << '\t'; break;
				case 'b': word << '\b'; break;
				case 'r': word << '\r'; break;
				default: word << c;
				}
			} else if (c == '\\') {
				escape = true;
			} else {
				word << c;
			}
			
			*input >> c; advance(c);
			c = input->peek();
		}
		
		*input >> c; advance(c);
		result = {Token::Type::EX_STRING, word.str(), {location, line, col}};
	} break;
	default:
		ostringstream word; word << c;
		char c = input->peek();
		
		while (!done() && !isspace(c) && SPECIAL_CHARS.find(c) == string::npos) {
			word << c;
			
			*input >> c; advance(c);
			c = input->peek();
		}
		
		string s = word.str();
		if (s == "...")
			result = {Token::Type::ELLIPSES, s, {location, line, col}};
		else
			result = {Token::Type::WORD, s, {location, line, col}};
	}
	
	toNextWS();
	return result;
}

bool ccl::Lexer::done() {
	return input->eof();
}

void ccl::Lexer::toNextWS() {
	char c = ' ';
	while (!this->input->eof() && isspace(c)) {
		*input >> c;
		advance(c);
	}
	if (!isspace(c)) {
		this->input->unget();
		this->col--;
	}
}