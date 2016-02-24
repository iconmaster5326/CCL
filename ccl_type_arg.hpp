
/* 
 * File:   ccl_type_arg.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_ARG_HPP
#define CCL_TYPE_ARG_HPP

#include <string>

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class arg_spec {
		public:
			string name;
			ccl_object* default_value;
			bool flag;
			arg_spec();
			arg_spec(string name, ccl_object* default_value, bool flag);
	};
	
	class ccl_type_arg : public ccl_type {
		public:
			ccl_type_arg();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_arg();
		private:
			
	};
}

#endif /* CCL_TYPE_ARG_HPP */

