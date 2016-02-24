
/* 
 * File:   ccl_type_key.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_KEY_HPP
#define CCL_TYPE_KEY_HPP

#include <string>

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class key_spec {
		public:
			ccl_object* key;
			ccl_object* value;
			key_spec();
			key_spec(ccl_object* key, ccl_object* value);
	};
	
	class ccl_type_key : public ccl_type {
		public:
			ccl_type_key();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_key();
		private:
			
	};
}

#endif /* CCL_TYPE_KEY_HPP */

