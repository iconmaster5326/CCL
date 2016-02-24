
/* 
 * File:   fenv.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 5:21 PM
 */

#ifndef FENV_HPP
#define FENV_HPP

#include <map>
#include <string>

#include "object.hpp"

namespace ccl {
	using namespace std;
	
	class fenv {
		public:
			fenv* parent;
			map<string,ccl_object*> vars;
			fenv();
			fenv(fenv* parent);
			ccl_object* get(string name);
			void put(string name, ccl_object* value);
			void set(string name, ccl_object* value);
			bool has(string name);
			~fenv();
		private:
			
	};
	
	fenv* make_base_fenv();
}

#endif /* FENV_HPP */

