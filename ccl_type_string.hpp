
/* 
 * File:   ccl_type_string.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_STRING_HPP
#define CCL_TYPE_STRING_HPP

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class ccl_type_string : public ccl_type {
		public:
			ccl_type_string();
			string to_string(ccl_object* obj) override;
			bool equals(ccl_object* arg1, ccl_object* arg2) override;
			ccl_object* copy(ccl_object* arg) override;
			bool compare(ccl_object* arg1, ccl_object* arg2, int& result) override;
			ccl_iterator* iterator(ccl_object* arg) override;
			void register_proc_hooks() override;
			virtual ~ccl_type_string();
		private:
			
	};
	
	class string_iterator : public ccl_iterator {
		public:
			string s;
			int n = 0;

			string_iterator(string s);

			bool done() override;
			void next() override;
			ccl_object* value() override;
	};
}

#endif /* CCL_TYPE_STRING_HPP */

