
/* 
 * File:   ccl_type_map.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 4:52 PM
 */

#ifndef CCL_TYPE_MAP_HPP
#define CCL_TYPE_MAP_HPP

#include "object.hpp"

#include <map>

namespace ccl {
	using namespace std;
	
	typedef map<ccl_object*,ccl_object*,ccl_object_compare_class> ccl_map_type;
	
	class ccl_type_map : public ccl_type {
		public:
			ccl_type_map();
			string to_string(ccl_object* obj) override;
			ccl_object* copy(ccl_object* arg) override;
			ccl_iterator* iterator(ccl_object* arg) override;
			void register_proc_hooks() override;
			virtual ~ccl_type_map();
		private:
			
	};
	
	class map_iterator : public ccl_iterator {
		public:
			ccl_map_type::iterator ii;
			ccl_map_type::iterator end;
			
			map_iterator(ccl_map_type::iterator ii, ccl_map_type::iterator end);

			bool done() override;
			void next() override;
			ccl_object* value() override;
	};
}

#endif /* CCL_TYPE_MAP_HPP */

