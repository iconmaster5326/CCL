/*
 * class.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include "ccl/class.hpp"
#include "ccl/error.hpp"

using namespace std;
using namespace ccl;

ccl::_Class::~_Class() {}
void ccl::_Class::gcGetChildren(_Object& self, std::function<void(GcNodeRef)> addChild) {}
void ccl::_Class::gcFree(_Object& self) {}

size_t ccl::_Class::hash(const _Object& self) const
{
	return (size_t) &self;
}

bool ccl::_Class::equal(const _Object& self, const Object other) const
{
	return &self == &*other;
}

bool ccl::_Class::lessThan(const _Object& self, const Object other) const
{
	return &self < &*other;
}

std::string ccl::_Class::name() {
	return "object";
}

Object ccl::_Class::execute(_Object& self, Thread& thread, Object& input,
		std::deque<Object>& args,
		std::unordered_map<std::string, Object>& flags) {
	throw Error("Cannot call instances of class <" + name() + ">");
}
