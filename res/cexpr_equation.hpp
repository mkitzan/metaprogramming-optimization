#pragma once

#include <array>

#include "cexpr_string.hpp"
#include "cexpr_tokens.hpp"
#include "templ_valtype.hpp"
#include "templ_math.hpp"
#include "templ_pair.hpp"

template<cexpr_string Str, typename ValT>
class cexpr_equation {
public:
	template<typename ...ArgL>
	static constexpr ValT eval(ArgL&&... args) noexcept
	{		
		const std::array<ValT, sizeof...(args)> values{ args... };
		return expression::eval(values);
	}

private:
	static constexpr bool isop(char ch) noexcept
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	template<std::size_t Pos>
	static constexpr auto parse() noexcept
	{
		constexpr auto token{ tokens[Pos].cbegin() };
		
		if constexpr (isop(*token)) {
			constexpr auto left{ parse<Pos + 1>() }, right{ parse<left.e0>() };
			return pair(VT<right.e0>{}, VT<operation(ValT{}, VT<*token>{}, left.e1, right.e1)>{});
		} else if constexpr (*token == 'x') {
			return pair(VT<Pos + 1>{}, VT<variable(VT<convert<ValT>(token + 1)>{})>{});
		} else {
			return pair(VT<Pos + 1>{}, VT<constant(VT<convert<ValT>(token)>{})>{});
		}
	}

	static constexpr cexpr_tokens<char, tlength(Str), tcount(Str)> tokens{ Str };

	using expression = decltype(parse<0>().e1);
};
