
/* 
 * File:   proc_hook.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef PROC_HOOK_HPP
#define PROC_HOOK_HPP

#include <list>
#include <map>

namespace ccl {
	using namespace std;
	
	class ccl_object; class executor;
	
	typedef ccl_object* (*proc_hook)(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec);
	
	class proc_hooks : private list<proc_hook> {
	public:
		proc_hooks();
		void connect(proc_hook hook);
		ccl_object* fire(ccl_object* input, list<ccl_object*>* args, map<string, ccl_object*>* flags, executor* exec);
	};
}

#endif /* PROC_HOOK_HPP */

