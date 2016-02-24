
/* 
 * File:   ccl_type_code.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_CODE_HPP
#define CCL_TYPE_CODE_HPP

#include <string>

#include "object.hpp"
#include "program.hpp"
#include "fenv.hpp"

namespace ccl {
	using namespace std;
	
	class code_spec {
		public:
			program* p;
			fenv* f;
			code_spec(program* p, fenv* f);
	};
	
	class ccl_type_code : public ccl_type {
		public:
			ccl_type_code();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_code();
		private:
			
	};
}

#endif /* CCL_TYPE_CODE_HPP */

