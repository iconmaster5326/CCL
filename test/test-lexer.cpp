/*
 * test-lexer.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_SUITE_END();
