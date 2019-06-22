/*
 * classes.cpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#include "ccl/classes.hpp"
#include "ccl/object.hpp"

using namespace ccl;
using namespace std;

ClassNil ccl::_ClassNil::CLASS{gcStaticAlloc};
Object ccl::_ClassNil::NIL{gcStaticAlloc, (Class) CLASS, nullptr};

ClassBool ccl::_ClassBool::CLASS{gcStaticAlloc};
Object ccl::_ClassBool::TRUE{gcStaticAlloc, (Class) CLASS, nullptr};
Object ccl::_ClassBool::FALSE{gcStaticAlloc, (Class) CLASS, nullptr};

ClassString ccl::_ClassString::CLASS{gcStaticAlloc};
