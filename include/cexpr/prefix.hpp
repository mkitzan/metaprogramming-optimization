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
	class prefix
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
		static constexpr auto parse() noexcept
		{
			constexpr auto token{ tokens_[Pos].cbegin() };
			
			if constexpr (isop(*token))
			{
				// Parse left and right expression tree nodes
				constexpr auto left{ parse<Pos + 1>() }, right{ parse<POS(left)>() };

				// Next token position and expression tree node to propogate up
				constexpr auto token_pos{ POS(right) };
				constexpr auto tree_node{ operation(ValT{}, TYPE_WRAP(*token), NODE(left), NODE(right)) };

				return TREE_EXPR(token_pos, tree_node);
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

		static constexpr tokens<char, tlength(Str), tcount(Str)> tokens_{ Str };

		using expression = decltype(parse<0>().second);
	};

} // namespace cexpr
