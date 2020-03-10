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
		static constexpr auto parse_terminators()
		{}

		template <std::size_t Pos>
		static constexpr auto parse_comp_ops()
		{}

		template <std::size_t Pos>
		static constexpr auto parse_negation_ops()
		{}

		template <std::size_t Pos>
		static constexpr auto parse_logical_ops()
		{}

		template <std::size_t Pos>
		static constexpr auto parse_from()
		{
			if constexpr (Pos + 3 < tokens_.count() && (tokens_[Pos + 2] == "where" || tokens_[Pos + 2] == "WHERE"))
			{
				
				return ra::selection<decltype(parse_logical_ops<Pos + 3>()), ra::relation<Schema>>{};
			}
			else
			{
				return ra::relation<Schema>{};	
			}
		}

		template <std::size_t Pos>
		static constexpr auto column_type()
		{
			constexpr cexpr::string<char, tokens_[Pos].length() + 1> name{ tokens_[Pos] };
			return decltype(sql::get<name>(typename Schema::row_type{})){};
		}

		template <std::size_t Pos>
		static constexpr auto next_column()
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
		static constexpr auto find_rename()
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
		static constexpr auto parse_column_info()
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
				constexpr auto name{ cexpr::string<char, tokens_[info.name].length() + 1>{ tokens_[info.name] } };
				constexpr auto col{ sql::column<name, typename decltype(info)::type>{} };

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
