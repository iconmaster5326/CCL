
/* 
 * File:   source_range.hpp
 * Author: iconmaster
 *
 * Created on February 19, 2016, 8:00 PM
 */

#ifndef SOURCE_RANGE_HPP
#define SOURCE_RANGE_HPP

namespace ccl {
	using namespace std;
	
	class source_range {
		public:
			string file;
			int line;
			
			source_range(string file, int line);
	};
}

#endif /* SOURCE_RANGE_HPP */

