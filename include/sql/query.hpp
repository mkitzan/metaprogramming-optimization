#pragma once

#include <array>
#include <string>

#include "ra/projection.hpp"
#include "ra/relation.hpp"
#include "ra/rename.hpp"

#include "sql/column.hpp"
#include "sql/tokens.hpp"
#include "sql/row.hpp"

#include "templ/valtype.hpp"
#include "templ/pair.hpp"

// Macros to improve readability by aliasing the operations of generic template entities
#define NODE(Pair) (Pair.second)
#define POS(Pair) (Pair.first)
#define TYPE_WRAP(Value) (templ::VT<Value>{})
#define TREE_EXPR(Pos, Node) (templ::pair(TYPE_WRAP(Pos), TYPE_WRAP(Node)))

namespace sql
{

	template <cexpr::string Str>
	class query
	{
	public:
	private:

		template <std::size_t Pos>
		static constexpr auto parse_from()
		{
			return 0;
		}

		static constexpr bool aggregate(std::string_view const& sv)
		{
			return sv == "count" || sv == "avg" || sv == "max" || sv == "min" || sv == "sum";
		}

		static constexpr auto column_type(std::size_t pos)
		{
			constexpr auto token{ tokens_[pos] };

			if constexpr (token == "i32")
			{
				return int32_t{};
			}
			else if constexpr (token == "u32")
			{
				return uint32_t{};
			}
			else if constexpr (token == "float")
			{
				return float{};
			}
			else if constexpr (token == "double")
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
			if constexpr (tokens_[Pos] != "," || tokens_[Pos] == "from" || Pos >= tokens_.size())
			{
				return Pos;
			}
			else
			{
				return next_column<Pos + 1>();
			}
		}

		template <std::size_t Pos>
		static constexpr auto parse_column_type()
		{
			constexpr auto val{ column_type(Pos + 1) };
			constexpr auto i{ next_column<Pos + 1>() };

			return TREE_EXPR(i + 1, val);
		}

		template <std::size_t Pos, std::size_t Curr>
		static constexpr auto as()
		{
			if constexpr (tokens_[Pos] == "from")
			{
				return Curr;
			}
			else if constexpr (tokens_[Pos] == "as")
			{
				return Pos + 1;
			}
			else
			{
				return as<Pos + 1, Curr>();
			}
		}

		template <std::size_t Pos>
		static constexpr auto type_start()
		{
			if constexpr (tokens_[Pos] == "<")
			{
				return Pos;
			}
			else
			{
				return type_start<Pos + 1>();
			}
		}

		template <std::size_t Pos, bool Re>
		static constexpr auto parse_column_name()
		{
			if constexpr (Re)
			{
				constexpr auto offset{ as<Pos, Pos>() };

				if constexpr (offset != Pos)
				{
					cexpr::string<char, tokens_[Pos].end() - tokens_[Pos].begin()> name{};
					
					name.fill(tokens_[Pos].begin(), tokens_[Pos].end());

					return TREE_EXPR(type_start<Pos>(), name);
				}
			}

			if constexpr (aggregate(tokens_[Pos]))
			{
				auto len0{ tokens_[Pos].end() - tokens_[Pos].begin() };
				auto len1{ tokens_[Pos + 2].end() - tokens_[Pos + 1].begin() };

				cexpr::string<char, len0 + len1 + 2> name{};

				name.fill(tokens_[Pos].begin(), tokens_[Pos].end());
				name[len0] = '(';
				name.fill_from(tokens_[Pos + 2].begin(), tokens_[Pos + 2].end(), name[len0 + 1]);
				name[len0 + len1 + 1] = ')';

				return TREE_EXPR(Pos + 3, name);
			}
			else
			{
				cexpr::string<char, tokens_[Pos].end() - tokens_[Pos].begin()> name{};
				name.fill(tokens_[Pos].begin(), tokens_[Pos].end());

				return TREE_EXPR(Pos + 1, name);
			}	
		}

		template <std::size_t Pos, bool Re>
		static constexpr auto recurse_columns() noexcept
		{
			if constexpr (tokens_[Pos] == "from")
			{
				return sql::void_row{};
			}
			else
			{
				constexpr auto name{ parse_column_name<Pos, Re>() };
				constexpr auto type{ parse_column_type<POS(name)>() };
				constexpr auto col{ sql::column<name, decltype(NODE(type))>{} };
				constexpr auto child{ recurse_columns<POS(type) + 1, Re>() };

				return TREE_EXPR(POS(type), (sql::row<decltype(col), decltype(child)>{}));
			}
		}

		template <std::size_t Pos>
		static constexpr auto parse_projection() noexcept
		{
			constexpr auto projection{ recurse_columns<Pos, false>() };

			return ra::projection<decltype(NODE(projection)), decltype(parse_from<POS(projection) + 1>())>{};
		}

		template <std::size_t Pos>
		static constexpr auto parse_rename() noexcept
		{
			constexpr auto rename{ recurse_columns<Pos, true>() };

			return ra::rename<decltype(NODE(rename)), decltype(parse_projection<Pos>())>{};
		}

		static constexpr sql::tokens<char, sql::preprocess(Str)> tokens_{ Str };

		using expression = decltype(parse_projection<1>());
	};

} // namespace sql
