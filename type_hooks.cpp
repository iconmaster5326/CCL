/* 
 * File:   type_hooks.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 5:24 PM
 */

#include "object.hpp"
#include "ccl_type_number.hpp"
#include "ccl_type_string.hpp"

namespace ccl {
	using namespace std;
	
	namespace types {
		void register_proc_hooks() {
			str()->register_proc_hooks();
			num()->register_proc_hooks();
			list()->register_proc_hooks();
			map()->register_proc_hooks();
			b_true()->register_proc_hooks();
			b_false()->register_proc_hooks();
			code()->register_proc_hooks();
			proc()->register_proc_hooks();
			arg()->register_proc_hooks();
		}
	}
}