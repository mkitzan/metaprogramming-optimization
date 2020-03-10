#pragma once

#include <cstddef>

#include "cexpr/string.hpp"

namespace sql
{

	template <cexpr::string Op, typename Row, typename Left, typename Right>
	struct operator
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

	template <cexpr::string Col, typename Row>
	struct variable
	{
		static constexpr auto eval(Row const& row)
		{
			return sql::get<Col>(row);
		}
	};

	template <auto Const, typename Row>
	struct constant
	{
		static constexpr auto eval(Row const& row)
		{
			return Const;
		}
	};

	template <typename Type>
	constexpr Type convert(cexpr::string const& str)
	{
		auto curr{ str.begin() }, end{ str.end() };
		constexpr decltype(str)::char_type nul{ '\0' }, dot{ '.' }, zro{ '0' }, min{ '-' };
		Type acc{}, sign{ 1 }, scalar{ 10 };

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
				acc += (*cur - zro) * (scalar /= Type{ 10 });
				++curr;
			}
		}

		return acc * sign;
	}

} // namespace sql
