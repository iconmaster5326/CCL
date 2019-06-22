/*
 * program.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_PROGRAM_HPP_
#define INCLUDE_CCL_PROGRAM_HPP_

#include <deque>

#include "ccl/types.hpp"
#include "ccl/error.hpp"

namespace ccl {
	class _Program : public GcAble {
	public:
		Source source;
		_Program(const Source& source = Source{}) : source{source} {}
		virtual Object evaluate(Thread& thread, Scope& scope, Object& input) = 0;
	};
	
	CCL_DECL_GC_CLASS(ProgramNull) : public _Program {
	public:
		_ProgramNull(const Source& source = Source{}) : _Program(source) {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
	
	CCL_DECL_GC_CLASS(ProgramConstant) : public _Program {
	public:
		Object value;
		_ProgramConstant(const Object& value, const Source& source = Source{}) : _Program(source), value{value} {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
	
	CCL_DECL_GC_CLASS(ProgramCall) : public _Program {
	public:
		struct Arg {
			bool flag;
			std::string key;
			Program value;
			
			Arg(const std::string& key, const Program& value) : flag{true}, key{key}, value{value} {}
			Arg(const Program& value) : flag{false}, key{""}, value{value} {}
		};
		
		std::string function;
		std::deque<Arg> args;
		
		_ProgramCall(const std::string& function, const Source& source = Source{}) : _Program(source), function{function}, args{} {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
	
	CCL_DECL_GC_CLASS(ProgramPipe) : public _Program {
	public:
		Program lhs, rhs;
		_ProgramPipe(const Program& lhs, const Program& rhs, const Source& source = Source{}) : _Program(source), lhs{lhs}, rhs{rhs} {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
	
	CCL_DECL_GC_CLASS(ProgramSequence) : public _Program {
	public:
		Program lhs, rhs;
		_ProgramSequence(const Program& lhs, const Program& rhs, const Source& source = Source{}) : _Program(source), lhs{lhs}, rhs{rhs} {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
	
	CCL_DECL_GC_CLASS(ProgramVar) : public _Program {
	public:
		std::string name;
		_ProgramVar(const std::string& name, const Source& source = Source{}) : _Program(source), name{name} {}
		Object evaluate(Thread& thread, Scope& scope, Object& input) override;
	};
}

#endif /* INCLUDE_CCL_PROGRAM_HPP_ */
