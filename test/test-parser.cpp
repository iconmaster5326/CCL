/*
 * test-parser.cpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <vector>

#include "ccl/parser.hpp"
#include "ccl/context.hpp"

using namespace std;
using namespace ccl;

static string location = "TEST LOCATION";

namespace std {
	static ostream& operator<<(ostream& os, const Token::Type t) {
		os << (void*) t;
		return os;
	}
}

BOOST_AUTO_TEST_SUITE(parser);

BOOST_AUTO_TEST_CASE(aParse) {
	Context ctx;
	
	BOOST_CHECK_NO_THROW({
		Program p = parse(ctx, location, "$x | y z 'v' \"w\" ; u v");
	});
}

BOOST_AUTO_TEST_SUITE_END();
