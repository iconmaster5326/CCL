/*
 * scope.hpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#ifndef INCLUDE_CCL_SCOPE_HPP_
#define INCLUDE_CCL_SCOPE_HPP_

#include <unordered_map>
#include <optional>

#include "ccl/types.hpp"

namespace ccl {
	class _Scope : public GcAble {
	public:
		std::unordered_map<std::string, Object> vars;
		std::optional<Scope> parent;
		Object input;
		Mutex mutex;
		
		_Scope();
		_Scope(Scope& parent);
		_Scope(Scope& parent, Object& input);
		virtual ~_Scope();
		
		virtual std::optional<Object> get(const std::string& name);
		virtual void set(const std::string& name, Object value);
		virtual void setLocal(const std::string& name, Object value);
		virtual bool canSet(const std::string& name);
	};
}

#endif /* INCLUDE_CCL_SCOPE_HPP_ */
