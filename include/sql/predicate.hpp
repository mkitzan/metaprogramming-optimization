#pragma once

#include <cstddef>

#include "cexpr/string.hpp"

namespace sql
{

	namespace
	{

		template <typename ValT>
		struct number
		{
			constexpr number(ValT val) : value{ val }
			{}

			ValT value;
		};

	} // namespace

	template <cexpr::string Op, typename Row, typename Left, typename Right=void>
	struct operation
	{
		static constexpr bool eval(Row const& row)
		{
			bool result{};

			if constexpr (Op == "=")
			{
				result = Left::eval(row) == Right::eval(row);
			}
			else if constexpr (Op == ">")
			{
				result = Left::eval(row) > Right::eval(row);
			}
			else if constexpr(Op == "<")
			{
				result = Left::eval(row) < Right::eval(row);
			}
			else if constexpr(Op == ">=")
			{
				result = Left::eval(row) >= Right::eval(row);
			}
			else if constexpr(Op == "<=")
			{
				result = Left::eval(row) <= Right::eval(row);
			}
			else if constexpr(Op == "!=" || Op == "<>")
			{
				result = Left::eval(row) != Right::eval(row);
			}
			else if constexpr(Op == "and" || Op == "AND")
			{
				result = Left::eval(row) && Right::eval(row);
			}
			else if constexpr(Op == "or" || Op == "OR")
			{
				result = Left::eval(row) || Right::eval(row);
			}
			else if constexpr(Op == "not" || Op == "NOT")
			{
				result = !Left::eval(row);
			}

			return result;
		}
	};

	template <cexpr::string Column, typename Row>
	struct variable
	{
		static constexpr auto eval(Row const& row)
		{
			return sql::get<Column>(row);
		}
	};

	template <auto Const, typename Row>
	struct constant
	{
		static constexpr auto eval(Row const& row)
		{
			return Const.value;
		}
	};

	template <typename ValT, typename CharT, std::size_t N>
	constexpr number<ValT> convert(cexpr::string<CharT, N> const& str)
	{
		auto curr{ str.cbegin() }, end{ str.cend() };
		constexpr CharT nul{ '\0' }, dot{ '.' }, zro{ '0' }, min{ '-' };
		ValT acc{}, sign{ 1 }, scalar{ 10 };

		if (*curr == min)
		{
			sign = -1;
			++curr;
		}

		while (curr != end && *curr != dot)
		{
			acc = (acc * scalar) + (*curr - zro);
			++curr;
		}

		if (curr != end && *curr == dot)
		{
			scalar = 1;
			++curr;

			while(curr != end)
			{
				acc += (*curr - zro) * (scalar /= ValT{ 10 });
				++curr;
			}
		}

		return number<ValT>{ acc * sign };
	}

} // namespace sql
