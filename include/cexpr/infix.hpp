#pragma once

#include <array>

#include "cexpr/string.hpp"
#include "cexpr/tokens.hpp"
#include "templ/valtype.hpp"
#include "templ/math.hpp"
#include "templ/pair.hpp"

using namespace templ;

// Macros to improve readability by aliasing the operations of generic template entities
#define NODE(Pair) (Pair.second)
#define POS(Pair) (Pair.first)
#define TYPE_WRAP(Value) (VT<Value>{})
#define TREE_EXPR(Token, Node) (pair(TYPE_WRAP(Token), TYPE_WRAP(Node)))

namespace cexpr
{

	template<string Str, typename ValT>
	class infix
	{
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
		static constexpr auto parse_brk_cns() noexcept
		{
			constexpr auto token{ tokens_[Pos].cbegin() };
			
			// todo
		}

		template<std::size_t Pos>
		static constexpr auto parse_mul_div() noexcept
		{
			constexpr auto token{ tokens_[Pos].cbegin() };
			
			// todo
		}

		template<std::size_t Pos>
		static constexpr auto parse_add_sub() noexcept
		{
			constexpr auto token{ tokens_[Pos].cbegin() };
			
			// todo
		}

		static constexpr tokens<char, tlength(Str), tcount(Str)> tokens_{ Str };

		using expression = decltype(parse_add_sub<0>().second);
	};

} // namespace cexpr

