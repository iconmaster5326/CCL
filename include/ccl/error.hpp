/*
 * error.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_ERROR_HPP_
#define INCLUDE_CCL_ERROR_HPP_

#include <string>
#include <stdexcept>

namespace ccl {
	class Source {
	public:
		static Source UNKNOWN;
		
		const std::string* location;
		int line, col;
		
		inline Source() : location{nullptr}, line{-1}, col{-1} {}
		inline Source(const std::string& location, int line = -1, int col = -1) : location{&location}, line{line}, col{col} {}
	};
}

#endif /* INCLUDE_CCL_ERROR_HPP_ */