
/* 
 * File:   proc_user.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 7:25 PM
 */

#ifndef PROC_USER_HPP
#define PROC_USER_HPP

#include "proc.hpp"
#include "program.hpp"
#include "fenv.hpp"
#include "ccl_type_arg.hpp"

#include <list>

namespace ccl {
	using namespace std;
	
	class proc_user : public proc {
		public:
			program* p;
			fenv* f;
			list<arg_spec>* specs;
			proc_user(program* p, fenv* f, list<arg_spec>* specs = new list<arg_spec>());
			ccl_object* call_impl(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec) override;
			virtual ~proc_user();
	};
}

#endif /* PROC_USER_HPP */

