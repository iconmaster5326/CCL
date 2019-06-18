/*
 * object.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include "ccl/object.hpp"
#include "ccl/context.hpp"

using namespace std;
using namespace ccl;

ccl::_Object::_Object(Class& clazz, void* value) : clazz{clazz}, value{.asPtr = value} {}
ccl::_Object::_Object(Class& clazz, double value) : clazz{clazz}, value{.asDouble = value} {}
ccl::_Object::_Object(Class& clazz, uint64_t value) : clazz{clazz}, value{.asInt = value} {}
ccl::_Object::_Object(Execution& owner, Class& clazz, void* value) : owner{owner}, clazz{clazz}, value{.asPtr = value} {}
ccl::_Object::_Object(Execution& owner, Class& clazz, double value) : owner{owner}, clazz{clazz}, value{.asDouble = value} {}
ccl::_Object::_Object(Execution& owner, Class& clazz, uint64_t value) : owner{owner}, clazz{clazz}, value{.asInt = value} {}

ccl::_Object::~_Object() {
	clazz->gcFree(*this);
}

void ccl::_Object::gcGetChildren(std::function<void(GcNodeRef)> addChild)
{
	clazz->gcGetChildren(*this, addChild);
}
