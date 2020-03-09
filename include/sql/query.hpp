#pragma once

#include <array>
#include <string>
#include <type_traits>

#include "ra/projection.hpp"
#include "ra/relation.hpp"
#include "ra/rename.hpp"

#include "sql/column.hpp"
#include "sql/tokens.hpp"
#include "sql/row.hpp"

namespace sql
{

	namespace
	{

		template <std::size_t Pos, typename Node>
		struct TreeExpr
		{
			using node = Node;
			static constexpr std::size_t pos = Pos;
		};

		template <typename Type, std::size_t Name, std::size_t Next>
		struct ColInfo
		{
			using type = Type;
			static constexpr std::size_t name = Name;
			static constexpr std::size_t next = Next;
		};

	} // namespace

	template <cexpr::string Str, typename Schema>
	class query
	{
	public:
		static constexpr void seed(Schema const& table)
		{
			expression::seed(table);
		}

		static constexpr auto next()
		{
			return expression::next();
		}

	private:

		template <std::size_t Pos>
		static constexpr auto parse_from()
		{
			return ra::relation<Schema>{};
		}

		static constexpr bool aggregate(std::string_view const& sv)
		{
			return sv == "count" || sv == "avg" || sv == "max" || sv == "min" || sv == "sum";
		}

		template <std::size_t Pos>
		static constexpr auto column_type()
		{
			constexpr auto token{ tokens_[Pos] };

			if constexpr (token == "i32")
			{
				return int32_t{};
			}
			else if constexpr (token == "u32")
			{
				return uint32_t{};
			}
			else if constexpr (token == "flt")
			{
				return float{};
			}
			else if constexpr (token == "dbl")
			{
				return double{};
			}
			else
			{
				return std::string{};
			}
		}

		template <std::size_t Pos>
		static constexpr auto next_column()
		{
			if constexpr (tokens_[Pos] == ",")
			{
				return Pos + 1;
			}
			else if constexpr (tokens_[Pos] == "from")
			{
				return Pos;
			}
			else
			{
				return next_column<Pos + 1>();
			}
		}

		template <std::size_t Pos, std::size_t Curr>
		static constexpr auto find_rename()
		{
			if constexpr (tokens_[Pos] == "," || tokens_[Pos] == "from")
			{
				return Curr;
			}
			else if constexpr (tokens_[Pos] == "as")
			{
				return Pos + 1;
			}
			else
			{
				return find_rename<Pos + 1, Curr>();
			}
		}

		template <std::size_t Pos, bool Re>
		static constexpr auto parse_column_info()
		{
			constexpr auto next{ next_column<Pos>() };

			if constexpr (Re)
			{
				constexpr auto offset{ find_rename<Pos, Pos>() };
				
				return ColInfo<decltype(column_type<Pos + 2>()), offset, next>{};
			}
			else
			{
				return ColInfo<decltype(column_type<Pos + 2>()), Pos, next>{};
			}
		}

		template <std::size_t Pos, bool Re>
		static constexpr auto recurse_columns() noexcept
		{
			if constexpr (tokens_[Pos] == "from")
			{
				return TreeExpr<Pos, sql::void_row>{};
			}
			else
			{
				constexpr auto info{ parse_column_info<Pos, Re>() };
				constexpr auto name{ cexpr::string<char, tokens_[info.name].length() + 1>{ tokens_[info.name] } };
				constexpr auto col{ sql::column<name, typename decltype(info)::type>{} };
				constexpr auto child{ recurse_columns<info.next, Re>() };

				return TreeExpr<child.pos, sql::row<decltype(col), std::remove_cvref_t<typename decltype(child)::node>>>{};
			}
		}

		template <std::size_t Pos>
		static constexpr auto parse_projection() noexcept
		{
			constexpr auto projection{ recurse_columns<Pos, false>() };

			return ra::projection<typename decltype(projection)::node, decltype(parse_from<projection.pos + 1>())>{};
		}

		template <std::size_t Pos>
		static constexpr auto parse_rename() noexcept
		{
			constexpr auto rename{ recurse_columns<Pos, true>() };

			return ra::rename<typename decltype(rename)::node, decltype(parse_projection<Pos>())>{};
		}

		static constexpr sql::tokens<char, sql::preprocess(Str)> tokens_{ Str };

		using expression = decltype(parse_rename<1>());
	};

} // namespace sql
