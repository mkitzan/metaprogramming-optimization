#pragma once

#include <cstddef>

#include "cexpr/string.hpp"

namespace sql
{

	namespace
	{

		// shim to allow all value types like double or float
		//	to be used as non-type template parameters.
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

} // namespace sql
