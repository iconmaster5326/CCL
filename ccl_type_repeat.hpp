
/* 
 * File:   ccl_type_repeat.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_REPEAT_HPP
#define CCL_TYPE_REPEAT_HPP

#include "object.hpp"

#include <vector>

namespace ccl {
	using namespace std;
	
	class ccl_type_repeat : public ccl_type {
		public:
			ccl_type_repeat();
			string to_string(ccl_object* obj) override;
			virtual ~ccl_type_repeat();
		private:
			
	};
}

#endif /* CCL_TYPE_REPEAT_HPP */

