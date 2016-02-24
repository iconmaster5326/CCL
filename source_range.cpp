
/* 
 * File:   source_range.cpp
 * Author: iconmaster
 *
 * Created on February 19, 2016, 8:00 PM
 */

#include <string>

#include "source_range.hpp"

namespace ccl {
	using namespace std;
	
	source_range::source_range(string file, int line) : file(file), line(line) {}
}