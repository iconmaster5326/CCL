
/* 
 * File:   post_call_hook.hpp
 * Author: iconmaster
 *
 * Created on January 21, 2016, 7:37 PM
 */

#ifndef POST_CALL_HOOK_HPP
#define POST_CALL_HOOK_HPP

namespace ccl {
	using namespace std;
	
	class executor;
	
	class pc_hook {
		public:
			virtual void call(executor* e) = 0;
	};
}

#endif /* POST_CALL_HOOK_HPP */

