#pragma once

#include "cexpr_math.hpp"
#include "cexpr_string.hpp"

template<cexpr_string Str, typename ValT>
class cexpr_equation {
public:
	template<typename ...ArgL>
	static constexpr ValT eval(ArgL&&... args) 
	{		
		const std::array<ValT, sizeof...(args)> values{ args... };
		return expression::eval(values);
	}

private:
	static constexpr bool isop(char ch)
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	// Want this logic in template recursion
	static constexpr auto parse()
	{
		constexpr auto token{ Str.next_token() };

		if constexpr (isop(*token)) {
			return Operator<ValT, *token, parse(), parse()>{};
		} else if constexpr (*token == 'x') {
			return Variable<ValT, convert(token + 1)>{};
		} else {
			return Constant<ValT, convert(token)>{};
		}
	}

	// What ever the template recursion expr is replaces `typename parse()` probably
	using expression = typename parse();
};
