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
			if constexpr (Op == "=")
			{
				return Left::eval(row) == Right::eval(row);
			}
			else if constexpr (Op == ">")
			{
				return Left::eval(row) > Right::eval(row);
			}
			else if constexpr(Op == "<")
			{
				return Left::eval(row) < Right::eval(row);
			}
			else if constexpr(Op == ">=")
			{
				return Left::eval(row) >= Right::eval(row);
			}
			else if constexpr(Op == "<=")
			{
				return Left::eval(row) <= Right::eval(row);
			}
			else if constexpr(Op == "!=" || Op == "<>")
			{
				return Left::eval(row) != Right::eval(row);
			}
			else if constexpr(Op == "and" || Op == "AND")
			{
				return Left::eval(row) && Right::eval(row);
			}
			else if constexpr(Op == "or" || Op == "OR")
			{
				return Left::eval(row) || Right::eval(row);
			}
			else if constexpr(Op == "not" || Op == "NOT")
			{
				return !Left::eval(row);
			}
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
