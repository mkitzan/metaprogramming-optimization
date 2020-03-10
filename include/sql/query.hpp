#pragma once

#include <array>
#include <string>
#include <string_view>
#include <type_traits>

#include "cexpr/string.hpp"

#include "ra/projection.hpp"
#include "ra/relation.hpp"
#include "ra/rename.hpp"
#include "ra/selection.hpp"

#include "sql/column.hpp"
#include "sql/tokens.hpp"
#include "sql/predicate.hpp"
#include "sql/row.hpp"

namespace sql
{

	namespace
	{

		template <std::size_t Pos, typename Node>
		struct TreeNode
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

		static constexpr void reset()
		{
			expression::reset();
		}

	private:
		template <std::size_t Pos>
		static constexpr auto convert_sv() noexcept
		{
			return cexpr::string<char, tokens_[Pos].length() + 1>{ tokens_[Pos] };
		}

		static inline constexpr bool isdigit(char c) noexcept
		{
			return c == '-' || c == '.' || (c >= '0' && c <= '9');
		}

		template <std::size_t Pos, typename Row>
		static constexpr auto parse_terminators() noexcept
		{
			if constexpr (tokens_[Pos] == "(")
			{
				constexpr auto expr{ parse_logical_ops<Pos + 1, Row>() };

				return TreeNode<expr.pos + 1, typename decltype(expr)::node>{};
			}
			else if constexpr (tokens_[Pos] == "\'" || tokens_[Pos] == "\"")
			{
				return TreeNode<Pos + 3, sql::constant<convert_sv<Pos + 1>(), Row>>{};
			}
			else if constexpr (isdigit(tokens_[Pos][0]))
			{
				return TreeNode<Pos + 1, sql::constant<sql::convert(convert_sv<Pos>()), Row>>{};
			}
			else
			{
				return TreeNode<Pos + 1, sql::variable<convert_sv<Pos>(), Row>>{};
			}
		}

		template <std::size_t Pos, typename Row>
		static constexpr auto parse_comp_ops() noexcept
		{
			constexpr auto left{ parse_terminators<Pos, Row>() };
			
			// todo
		}

		template <std::size_t Pos, typename Row>
		static constexpr auto parse_negation_ops() noexcept
		{
			if constexpr (tokens_[Pos] == "not" || tokens_[Pos] == "NOT")
			{
				constexpr auto expr{ parse_comp_ops<Pos + 1, Row>() };
				return TreeNode<expr.pos, sql::operation<convert_sv<Pos>(), Row, decltype(expr)::node>>{};
			}
			else
			{
				return parse_comp_ops<Pos + 1, Row>();
			}
		}

		template <std::size_t Pos, typename Row>
		static constexpr auto parse_logical_ops() noexcept
		{
			constexpr auto left{ parse_negation_ops<Pos, Row>() };
			
			// todo
		}

		template <std::size_t Pos>
		static constexpr auto parse_from() noexcept
		{
			if constexpr (Pos + 3 < tokens_.count() && (tokens_[Pos + 2] == "where" || tokens_[Pos + 2] == "WHERE"))
			{
				constexpr auto predicate{ parse_logical_ops<Pos + 3, ra::relation<Schema>>() };
				return ra::selection<decltype(predicate)::node, ra::relation<Schema>>{};
			}
			else
			{
				return ra::relation<Schema>{};	
			}
		}

		template <std::size_t Pos>
		static constexpr auto column_type() noexcept
		{
			return decltype(sql::get<convert_sv<Pos>()>(typename Schema::row_type{})){};
		}

		template <std::size_t Pos>
		static constexpr auto next_column() noexcept
		{
			if constexpr (tokens_[Pos] == ",")
			{
				return Pos + 1;
			}
			else if constexpr (tokens_[Pos] == "from" || tokens_[Pos] == "FROM")
			{
				return Pos;
			}
			else
			{
				return next_column<Pos + 1>();
			}
		}

		template <std::size_t Pos, std::size_t Curr>
		static constexpr auto find_rename() noexcept
		{
			if constexpr (tokens_[Pos] == "," || tokens_[Pos] == "from" || tokens_[Pos] == "FROM")
			{
				return Curr;
			}
			else if constexpr (tokens_[Pos] == "as" || tokens_[Pos] == "AS")
			{
				return Pos + 1;
			}
			else
			{
				return find_rename<Pos + 1, Curr>();
			}
		}

		template <std::size_t Pos, bool Rename>
		static constexpr auto parse_column_info() noexcept
		{
			constexpr auto next{ next_column<Pos>() };

			if constexpr (Rename)
			{
				constexpr auto offset{ find_rename<Pos, Pos>() };
				
				return ColInfo<decltype(column_type<Pos>()), offset, next>{};
			}
			else
			{
				return ColInfo<decltype(column_type<Pos>()), Pos, next>{};
			}
		}

		template <std::size_t Pos, bool Rename>
		static constexpr auto recurse_columns() noexcept
		{
			if constexpr (tokens_[Pos] == "from" || tokens_[Pos] == "FROM")
			{
				return TreeNode<Pos, sql::void_row>{};
			}
			else
			{
				constexpr auto info{ parse_column_info<Pos, Rename>() };
				constexpr auto child{ recurse_columns<info.next, Rename>() };
				constexpr auto col{ sql::column<convert_sv<info.name>(), typename decltype(info)::type>{} };

				return TreeNode<child.pos, sql::row<decltype(col), std::remove_cvref_t<typename decltype(child)::node>>>{};
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
