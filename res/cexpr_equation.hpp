#pragma once

#include "cexpr_math.hpp"
#include "cexpr_string.hpp"
#include "templ_stack.hpp"

template<cexpr_string Str, typename ValT>
class cexpr_equation {
public:
	template<typename ...ArgL>
	static constexpr ValT eval(ArgL&&... args) 
	{		
		const std::array<ValT, sizeof...(args)> values{ args... };
		return expression.eval(values);
	}

private:
	static constexpr bool isop(char ch)
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	template<typename Stack>
	static constexpr auto build(Stack s)
	{
		constexpr auto token{ Str.next_token() };

		if constexpr (token == Str.end()) {
			return s;
		} else {
			return build(push(token, s));
		}
	}

	static constexpr auto tokenize()
	{
		return reverse(build(templ_stack{}), templ_stack{});
	}

	// Want logic like this
	template<typename Stack>
	static constexpr auto parse(Stack s)
	{
		constexpr auto token{ top(s) };

		if constexpr (isop(*token)) {
			s = pop(s);
			return Operator<ValT, *token, parse(s), parse(s)>{};
		} else if constexpr (*token == 'x') {
			return Variable<ValT, convert(token + 1)>{};
		} else {
			return Constant<ValT, convert(token)>{};
		}
	}

	static constexpr auto expression = parse(tokenize());
};
