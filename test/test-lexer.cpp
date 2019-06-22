/*
 * test-lexer.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <vector>

#include "ccl/lexer.hpp"

using namespace std;
using namespace ccl;

static string location = "TEST LOCATION";

namespace std {
	ostream& operator<<(ostream& os, const Token::Type t) {
		os << (void*) t;
		return os;
	}
}

BOOST_AUTO_TEST_SUITE(lexer);

BOOST_AUTO_TEST_CASE(oneWord) {
	Lexer lexer{location, "hello"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(twoWords) {
	Lexer lexer{location, "hello world"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "world");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 7);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(locationsMultiline) {
	Lexer lexer{location, "a\nb c\nd"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.source.line, 2);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.source.line, 2);
	BOOST_CHECK_EQUAL(t.source.col, 3);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.source.line, 3);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(leadingWhitespace) {
	Lexer lexer{location, "   hello"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 4);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(trailingWhitespace) {
	Lexer lexer{location, "hello   "};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(flagEdgeCases) {
	Lexer lexer{location, "--a -a-b -a- -1 - -. -.1 -1.a -a.1 a- a-b"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "-a");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "a-b");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "a-");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "-1");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "-");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, ".");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "-.1");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "1.a");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "a.1");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "a-");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "a-b");
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(varEdgeCase) {
	Lexer lexer{location, "$ $-a $a- $$a $a$ a$ a$b"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "$");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::VAR);
	BOOST_CHECK_EQUAL(t.value, "-a");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::VAR);
	BOOST_CHECK_EQUAL(t.value, "a-");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::VAR);
	BOOST_CHECK_EQUAL(t.value, "$a");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::VAR);
	BOOST_CHECK_EQUAL(t.value, "a$");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "a$");
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "a$b");
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(strings) {
	Lexer lexer{location, "'' 'a' 'a b' ' ' '\\'' '\"' '\\n' 'a\\nb' '\\\\' '\\$' '\n' 'a\nb'"};
	Token t;
	
	vector<string> values{"", "a", "a b", " ", "'", "\"", "\n", "a\nb", "\\", "$", "\n", "a\nb"};
	
	for (const string& s : values) {
		BOOST_CHECK_NO_THROW({
			t = lexer.next();
		});
		BOOST_CHECK_EQUAL(t.type, Token::Type::STRING);
		BOOST_CHECK_EQUAL(t.value, s);
	}
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(exStrings) {
	Lexer lexer{location, "\"\" \"a\" \"a b\" \" \" \"'\" \"\\\"\" \"\\n\" \"a\\nb\" \"\\\\\" \"\\$\" \"\n\" \"a\nb\""};
	Token t;
	
	vector<string> values{"", "a", "a b", " ", "'", "\"", "\n", "a\nb", "\\", "$", "\n", "a\nb"};
	
	for (const string& s : values) {
		BOOST_CHECK_NO_THROW({
			t = lexer.next();
		});
		BOOST_CHECK_EQUAL(t.type, Token::Type::EX_STRING);
		BOOST_CHECK_EQUAL(t.value, s);
	}
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(everyToken) {
	Lexer lexer{location, "a -f 'x' | $g \"y\" & [(...)]{};"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "a");
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::FLAG);
	BOOST_CHECK_EQUAL(t.value, "f");
	BOOST_CHECK_EQUAL(t.source.col, 3);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::STRING);
	BOOST_CHECK_EQUAL(t.value, "x");
	BOOST_CHECK_EQUAL(t.source.col, 6);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::PIPE);
	BOOST_CHECK_EQUAL(t.value, "|");
	BOOST_CHECK_EQUAL(t.source.col, 10);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::VAR);
	BOOST_CHECK_EQUAL(t.value, "g");
	BOOST_CHECK_EQUAL(t.source.col, 12);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::EX_STRING);
	BOOST_CHECK_EQUAL(t.value, "y");
	BOOST_CHECK_EQUAL(t.source.col, 15);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::AND);
	BOOST_CHECK_EQUAL(t.value, "&");
	BOOST_CHECK_EQUAL(t.source.col, 19);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::LBRACKET);
	BOOST_CHECK_EQUAL(t.value, "[");
	BOOST_CHECK_EQUAL(t.source.col, 21);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::LPAREN);
	BOOST_CHECK_EQUAL(t.value, "(");
	BOOST_CHECK_EQUAL(t.source.col, 22);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::ELLIPSES);
	BOOST_CHECK_EQUAL(t.value, "...");
	BOOST_CHECK_EQUAL(t.source.col, 23);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::RPAREN);
	BOOST_CHECK_EQUAL(t.value, ")");
	BOOST_CHECK_EQUAL(t.source.col, 26);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::RBRACKET);
	BOOST_CHECK_EQUAL(t.value, "]");
	BOOST_CHECK_EQUAL(t.source.col, 27);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::LBRACE);
	BOOST_CHECK_EQUAL(t.value, "{");
	BOOST_CHECK_EQUAL(t.source.col, 28);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::RBRACE);
	BOOST_CHECK_EQUAL(t.value, "}");
	BOOST_CHECK_EQUAL(t.source.col, 29);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::SEMICOLON);
	BOOST_CHECK_EQUAL(t.value, ";");
	BOOST_CHECK_EQUAL(t.source.col, 30);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_CASE(peeking) {
	Lexer lexer{location, "hello world"};
	Token t;
	
	BOOST_CHECK_NO_THROW({
		t = lexer.peek();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.peek();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "hello");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 1);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.peek();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "world");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 7);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.peek();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "world");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 7);
	
	BOOST_CHECK_NO_THROW({
		t = lexer.next();
	});
	BOOST_CHECK_EQUAL(t.type, Token::Type::WORD);
	BOOST_CHECK_EQUAL(t.value, "world");
	BOOST_CHECK_EQUAL(*t.source.location, location);
	BOOST_CHECK_EQUAL(t.source.line, 1);
	BOOST_CHECK_EQUAL(t.source.col, 7);
	
	BOOST_CHECK(lexer.done());
}

BOOST_AUTO_TEST_SUITE_END();
