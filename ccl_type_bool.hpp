
/* 
 * File:   ccl_type_bool.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_BOOL_HPP
#define CCL_TYPE_BOOL_HPP

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class ccl_type_true : public ccl_type {
		public:
			ccl_type_true();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_true();
		private:
			
	};
	
	class ccl_type_false : public ccl_type {
		public:
			ccl_type_false();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_false();
		private:
			
	};
}

#endif /* CCL_TYPE_BOOL_HPP */

