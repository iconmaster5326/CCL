
/* 
 * File:   expression.hpp
 * Author: iconmaster
 *
 * Created on January 20, 2016, 4:10 PM
 */

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <list>
#include <string>

namespace ccl {
	using namespace std;
	
	typedef enum {
		E_NONE, E_CODE, E_EXPR, E_STR, E_STREX, E_NUM, E_VAR, E_TRUE, E_FLAG
	} expr_type;
	
	class expression;
	
	class flag_pair {
		public:
			char* name;
			expression* expr;
	};
	
	class expression {
		public:
			expr_type type;
			union {
				char* as_str;
				double as_num;
				list<void*>* as_list;
				flag_pair as_fp;
			} value;
			expression();
			expression(expr_type type);
			expression(expr_type type, string value);
			expression(expr_type type, char* value);
			expression(expr_type type, double value);
			expression(expr_type type, list<void*>* value);
			expression(expr_type type, char* name, expression* value);
			~expression();
		private:

	};
}

#endif /* EXPRESSION_HPP */

