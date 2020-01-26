#pragma once

#include <array>
#include <cstddef>

#include "templ/valtype.hpp"

namespace templ {

	template<typename ValT, char Op, typename Left, typename Right>
	struct Operator {
		template<std::size_t N>
		static constexpr ValT eval(std::array<ValT, N> const& args)
		{
			ValT result{};

			if constexpr (Op == '+')
			{
				result = Left::eval(args) + Right::eval(args);
			}
			else if constexpr(Op == '-')
			{
				result = Left::eval(args) - Right::eval(args);
			}
			else if constexpr(Op == '*')
			{
				result = Left::eval(args) * Right::eval(args);
			}
			else if constexpr(Op == '/')
			{
				result = Left::eval(args) / Right::eval(args);
			}

			return result;
		}
	};

	template<typename ValT, ValT Pos>
	struct Variable {
		template<std::size_t N>
		static constexpr ValT eval(std::array<ValT, N> const& args)
		{
			return args[Pos];
		}
	};

	template<typename ValT, ValT Const>
	struct Constant {
		template<std::size_t N>
		static constexpr ValT eval(std::array<ValT, N> const& args)
		{
			return Const;
		}
	};

	template<typename ValT, char Op, typename LExpr, typename RExpr>
	constexpr auto operation(ValT num, VT<Op> op, LExpr left, RExpr right)
	{
		return Operator<ValT, Op, LExpr, RExpr>{};
	}

	template<typename ValT, ValT VariT>
	constexpr auto variable(VT<VariT> num)
	{
		return Variable<ValT, VariT>{};
	}

	template<typename ValT, ValT ConsT>
	constexpr auto constant(VT<ConsT> num)
	{
		return Constant<ValT, ConsT>{};
	}

	template<typename ValT>
	constexpr ValT convert(const char* str)
	{
		constexpr char nul{ '\0' }, dot{ '.' }, zro{ '0' }, min{ '-' };
		ValT acc{}, sign{ 1 }, scalar{ 10 };

		if (*str == min)
		{
			sign = -1;
			++str;
		}

		while (*str != nul && *str != dot)
		{
			acc = (acc * scalar) + (*str - zro);
			++str;
		}

		if (*str == dot)
		{
			scalar = 1;
			++str;

			while(*str != nul)
			{
				acc += (*str - zro) * (scalar /= ValT{ 10 });
				++str;
			}
		}

		return acc * sign;
	}

} // namespace templ
