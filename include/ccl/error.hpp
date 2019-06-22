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
		const std::string* location;
		int line, col;
		
		Source() : location{nullptr}, line{-1}, col{-1} {}
		Source(const std::string& location, int line = -1, int col = -1) : location{&location}, line{line}, col{col} {}
	};
	
	class Error : public std::runtime_error {
	public:
		Error(const std::string& _what) : std::runtime_error(_what) {}
	};
}

#endif /* INCLUDE_CCL_ERROR_HPP_ */
