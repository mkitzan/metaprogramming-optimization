#pragma once

#include <type_traits>
#include <utility>
#include <vector>
#include <set>

#include "sql/column.hpp"
#include "sql/index.hpp"
#include "sql/row.hpp"

namespace sql
{

	template <typename Index, typename... Cols>
	class schema
	{
	public:
		using row_type = sql::variadic_row<Cols...>::row_type;
		using container = typename
			std::conditional_t<
				std::is_same_v<Index, sql::index<>>,
				std::vector<row_type>,
				std::multiset<row_type, typename Index::template comp<row_type>>
			>;
		using const_iterator = typename container::const_iterator;

		schema() = default;

		template <typename T, typename... Ts>
		schema(std::vector<T> const& col, Ts const&... cols) : schema{}
		{
			insert(col, cols...);
		}

		template <typename T, typename... Ts>
		schema(std::vector<T>&& col, Ts&&... cols) : schema{}
		{
			insert(std::forward<T>(col), std::forward<Ts...>(cols...));
		}

		template <typename... Ts>
		inline void emplace(Ts const&... vals)
		{
			table_.emplace_back(vals...);
		}

		template <typename... Ts>
		inline void emplace(Ts&&... vals)
		{
			table_.emplace_back(std::forward<Ts...>(vals...));
		}

		template <typename T, typename... Ts>
		void insert(std::vector<T> const& col, Ts const&... cols)
		{
			for (std::size_t i{}; i < col.size(); ++i)
			{
				emplace(col[i], cols[i]...);
			}
		}

		template <typename T, typename... Ts>
		void insert(std::vector<T>&& col, Ts&&... cols)
		{
			for (std::size_t i{}; i < col.size(); ++i)
			{
				emplace(std::forward<T>(col[i]), std::forward<Ts...>(cols[i]...));
			}
		}

		void insert(row_type const& row)
		{
			table_.push_back(row);
		}

		void insert(row_type&& row)
		{
			table_.push_back(std::forward<row_type>(row));
		}

		inline const_iterator begin() const
		{
			return table_.begin();
		}

		inline const_iterator end() const
		{
			return table_.end();
		}

	private:
		container table_;
	};

} // namespace sql
