/*
 * object.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include <boost/uuid/random_generator.hpp>

#include "ccl/object.hpp"
#include "ccl/context.hpp"

using namespace std;
using namespace ccl;
using namespace boost::uuids;

ccl::_Object::_Object(Execution& owner, Class& clazz, void* value) : owner{owner}, clazz{clazz}, value{.asPtr = value}, id{random_generator()()} {}
ccl::_Object::_Object(Execution& owner, Class& clazz, double value) : owner{owner}, clazz{clazz}, value{.asDouble = value}, id{random_generator()()} {}
ccl::_Object::_Object(Execution& owner, Class& clazz, uint64_t value) : owner{owner}, clazz{clazz}, value{.asInt = value}, id{random_generator()()} {}

ccl::_Object::~_Object() {
	clazz->gcFree(*this);
}

void ccl::_Object::gcGetChildren(std::function<void(GcNodeRef)> addChild)
{
	clazz->gcGetChildren(*this, addChild);
}
