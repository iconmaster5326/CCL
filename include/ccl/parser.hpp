/*
 * parser.hpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_PARSER_HPP_
#define INCLUDE_CCL_PARSER_HPP_

#include "ccl/types.hpp"
#include "ccl/lexer.hpp"
#include "ccl/program.hpp"

namespace ccl {
	Program parse(Context& ctx, Lexer& lexer);
	
	template<typename... Args>
	inline Program parse(Context& ctx, Args... args) {
		Lexer lexer{args...};
		return parse(ctx, lexer);
	}
}

#endif /* INCLUDE_CCL_PARSER_HPP_ */
