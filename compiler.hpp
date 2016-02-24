
/* 
 * File:   compiler.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 2:57 PM
 */

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <list>

#include "program.hpp"
#include "operation.hpp"
#include "instruction.hpp"
#include "expression.hpp"

namespace ccl {
	using namespace std;
	
	program* compile(list<instruction*>* code);
}

#endif /* COMPILER_HPP */

