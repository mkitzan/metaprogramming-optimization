#pragma once

#include "cexpr_string.hpp"
#include "templ_entity.hpp"
#include "templ_math.hpp"
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
/*
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
*/

	static constexpr bool isop(char ch)
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	static constexpr auto parse()
	{
		// Need the Stack or TokenStream to be "global"
		constexpr auto token{ Str.next_token() };

		if constexpr (isop(*token)) {
			return operation(Entity<*token>{}, parse(), parse());
		} else if constexpr (*token == 'x') {
			return variable(Entity<convert<ValT>(token + 1)>{});
		} else {
			return constant(Entity<convert<ValT>(token)>{});
		}
	}

	//static constexpr auto expression{ parse(tokenize()) };
	static constexpr auto expression{ parse() };
};
