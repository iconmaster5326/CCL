
/* 
 * File:   debugger.hpp
 * Author: iconmaster
 *
 * Created on February 20, 2016, 8:55 PM
 */

#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <string>

#include "machine.hpp"

namespace ccl {
	using namespace std;
	
	void begin_debugger(string input, machine* m);
}

#endif /* DEBUGGER_HPP */

