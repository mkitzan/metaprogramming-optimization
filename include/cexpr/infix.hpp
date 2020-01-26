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
#define TREE_EXPR(Pos, Node) (pair(TYPE_WRAP(Pos), TYPE_WRAP(Node)))

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
		static constexpr bool ismuldiv(char ch) noexcept
		{
			return ch == '*' || ch == '/';
		}

		static constexpr bool isaddsub(char ch) noexcept
		{
			return ch == '+' || ch == '-';
		}

		template<std::size_t Pos>
		static constexpr auto parse_brk_trm() noexcept
		{
			constexpr auto token{ tokens_[Pos].cbegin() };

			if constexpr (*token == '(')
			{
				// Parse sub-expression contained in brackets to propogate up
				constexpr auto subexpr{ parse_add_sub<Pos + 1>() };

				return TREE_EXPR(POS(subexpr) + 1, NODE(subexpr));
			}
			else if constexpr (*token == 'x')
			{
				// Next token position and expression tree node to propogate up
				constexpr auto token_pos{ Pos + 1 };
				constexpr auto tree_node{ variable(TYPE_WRAP(convert<ValT>(token + 1))) };

				return TREE_EXPR(token_pos, tree_node);
			}
			else
			{
				// Next token position and expression tree node to propogate up
				constexpr auto token_pos{ Pos + 1 };
				constexpr auto tree_node{ constant(TYPE_WRAP(convert<ValT>(token))) };

				return TREE_EXPR(token_pos, tree_node);
			}
		}

		template<typename Left>
		static constexpr auto recurse_mul_div() noexcept
		{
			constexpr auto left{ Left{} };
			constexpr auto token_pos{ POS(left) };
			constexpr auto token{ tokens_[token_pos % tokens_.count()][0] };

			if constexpr (token_pos == tokens_.count() || !ismuldiv(token))
			{
				// Sub-expression ended
				return left;
			}
			else
			{
				// Parse right side of the sub-expression and create next node from left and right
				constexpr auto right{ parse_brk_trm<token_pos + 1>() };
				constexpr auto tree_node{ operation(ValT{}, TYPE_WRAP(token), NODE(left), NODE(right)) };
				
				// Recurse down with new sub-expression
				return recurse_mul_div<decltype(TREE_EXPR(POS(right), tree_node))>();
			}
		}

		template<std::size_t Pos>
		static constexpr auto parse_mul_div() noexcept
		{
			// Parse left side of the expression and recurse expression
			constexpr auto left{ parse_brk_trm<Pos>() };

			// Recursively loop on the multiply/divide sub-expression
			return recurse_mul_div<decltype(left)>();
		}

		template<typename Left>
		static constexpr auto recurse_add_sub() noexcept
		{
			constexpr auto left{ Left{} };
			constexpr auto token_pos{ POS(left) };
			constexpr auto token{ tokens_[token_pos % tokens_.count()][0] };

			if constexpr (token_pos == tokens_.count() || !isaddsub(token))
			{
				// Sub-expression ended
				return left;
			}
			else
			{
				// Parse right side of the sub-expression and create next node from left and right
				constexpr auto right{ parse_mul_div<token_pos + 1>() };
				constexpr auto tree_node{ operation(ValT{}, TYPE_WRAP(token), NODE(left), NODE(right)) };
				
				// Recurse down with new sub-expression
				return recurse_add_sub<decltype(TREE_EXPR(POS(right), tree_node))>();
			}
		}

		template<std::size_t Pos>
		static constexpr auto parse_add_sub() noexcept
		{
			// Parse left side of the sub-expression
			constexpr auto left{ parse_mul_div<Pos>() };

			// Recursively loop on the add/subtraction expression
			return recurse_add_sub<decltype(left)>();
		}

		static constexpr tokens<char, tlength(Str), tcount(Str)> tokens_{ Str };

		using expression = decltype(parse_add_sub<0>().second);
	};

} // namespace cexpr
