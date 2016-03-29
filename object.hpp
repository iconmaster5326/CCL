
/* 
 * File:   object.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 5:24 PM
 */

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

namespace ccl {
	using namespace std;
	
	class ccl_type; class ccl_object; 
	
	class ccl_iterator {
		public:
			virtual bool done() = 0;
			virtual void next() = 0;
			virtual ccl_object* value() = 0;
	};
	
	class ccl_type {
		public:
			char* name;
			
			ccl_type(char* name);
			
			virtual string to_string(ccl_object* obj);
			virtual bool equals(ccl_object* arg1, ccl_object* arg2);
			virtual ccl_object* copy(ccl_object* arg);
			virtual bool compare(ccl_object* arg1, ccl_object* arg2, int& result);
			virtual ccl_iterator* iterator(ccl_object* arg);
			
			virtual void register_proc_hooks();
			virtual ~ccl_type();
	};
	
	bool ccl_compare(ccl_object* a,ccl_object* b);
	
	struct ccl_object_compare_class {
		bool operator() (const ccl_object* lhs, const ccl_object* rhs);
	};
	
	class ccl_object {
		public:
			ccl_type* type;
			void* value;
			ccl_object(ccl_type* type, double value);
			ccl_object(ccl_type* type, void* value);
			~ccl_object();
			
			double num_val();
			string str_val();
		private:
			
	};
	
	namespace types {
		ccl_type* str();
		ccl_type* num();
		ccl_type* list();
		ccl_type* map();
		ccl_type* b_true();
		ccl_type* b_false();
		ccl_type* code();
		ccl_type* proc();
		ccl_type* arg();
		ccl_type* key();
		ccl_type* repeat();
		ccl_type* _added_hook();
		
		void register_proc_hooks();
	}
	
	namespace constants {
		ccl_object* b_true();
		ccl_object* b_false();
		ccl_object* empty_code();
	}
}

#endif /* OBJECT_HPP */

