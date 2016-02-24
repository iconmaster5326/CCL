
/* 
 * File:   ccl_type_list.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_LIST_HPP
#define CCL_TYPE_LIST_HPP

#include "object.hpp"

#include <vector>

namespace ccl {
	using namespace std;
	
	class ccl_type_list : public ccl_type {
		public:
			ccl_type_list();
			string to_string(ccl_object* obj) override;
			ccl_object* copy(ccl_object* arg) override;
			ccl_iterator* iterator(ccl_object* arg) override;
			void register_proc_hooks() override;
			virtual ~ccl_type_list();
		private:
			
	};
	
	class list_iterator : public ccl_iterator {
		public:
			vector<ccl_object*>::iterator ii;
			vector<ccl_object*>::iterator end;
			
			list_iterator(vector<ccl_object*>::iterator ii, vector<ccl_object*>::iterator end);

			bool done() override;
			void next() override;
			ccl_object* value() override;
	};
}

#endif /* CCL_TYPE_LIST_HPP */

