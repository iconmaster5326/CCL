
/* 
 * File:   ccl_type_number.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_NUMBER_HPP
#define CCL_TYPE_NUMBER_HPP

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class ccl_type_number : public ccl_type {
		public:
			ccl_type_number();
			string to_string(ccl_object* obj) override;
			bool equals(ccl_object* arg1, ccl_object* arg2) override;
			void register_proc_hooks() override;
			bool compare(ccl_object* arg1, ccl_object* arg2, int& result) override;
			virtual ~ccl_type_number();
		private:
			
	};
}

#endif /* CCL_TYPE_NUMBER_HPP */

