/*
 * scope.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include "ccl/scope.hpp"
#include "ccl/object.hpp"
#include "ccl/context.hpp"

using namespace std;
using namespace ccl;

ccl::_Scope::_Scope()
{
}

ccl::_Scope::_Scope(Scope parent) : parent{parent}
{
}

ccl::_Scope::_Scope(Scope parent, Object input) : parent{parent} //, input{input}
{
}

ccl::_Scope::~_Scope() {}

std::optional<Object> ccl::_Scope::get(const std::string& name)
{
	return optional<Object>();
}

void ccl::_Scope::set(const std::string& name, Object value)
{
}

void ccl::_Scope::setLocal(const std::string& name, Object value)
{
}

bool ccl::_Scope::canSet(const std::string& name)
{
	return false;
}
