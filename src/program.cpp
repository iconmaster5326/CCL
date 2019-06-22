/*
 * program.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#include "ccl/program.hpp"
#include "ccl/scope.hpp"
#include "ccl/object.hpp"
#include "ccl/context.hpp"
#include "ccl/classes.hpp"

using namespace std;
using namespace ccl;

Object ccl::_ProgramNull::evaluate(Thread& thread, Scope& scope, Object& input)
{
	return input;
}

Object ccl::_ProgramConstant::evaluate(Thread& thread, Scope& scope, Object& input)
{
	return value;
}

Object ccl::_ProgramCall::evaluate(Thread& thread, Scope& scope, Object& input)
{
	auto fnOpt = scope->get(function);
	if (!fnOpt) {
		throw Error("Function '" + function + "' not found");
	}
	
	Object fnOb = fnOpt.value();
	deque<Object> args;
	unordered_map<string, Object> flags;
	
	for (const Arg& arg : this->args) {
		if (arg.flag) {
			flags.insert_or_assign(arg.key, arg.value->evaluate(thread, scope, input));
		} else {
			args.emplace_back(arg.value->evaluate(thread, scope, input));
		}
	}
	
	return fnOb->execute(thread, input, args, flags);
}

Object ccl::_ProgramPipe::evaluate(Thread& thread, Scope& scope, Object& input)
{
	Object o = lhs->evaluate(thread, scope, input);
	return rhs->evaluate(thread, scope, o);
}

Object ccl::_ProgramSequence::evaluate(Thread& thread, Scope& scope, Object& input)
{
	lhs->evaluate(thread, scope, input);
	return rhs->evaluate(thread, scope, scope->input);
}

Object ccl::_ProgramVar::evaluate(Thread& thread, Scope& scope, Object& input)
{
	auto opt = scope->get(name);
	if (!opt) {
		return _ClassNil::NIL;
	}
	return opt.value();
}
