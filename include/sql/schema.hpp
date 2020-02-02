#pragma once

#include <vector>

#include "cexpr/string.hpp"
#include "sql/column.hpp"
#include "sql/row.hpp"

namespace sql
{

	template <typename Col, typename... Cols>
	class schema
	{
		using col_type = typename Col::type;
		using row_type = row<Col, Cols...>;
	public:
		schema() : column_{}, next_{}
		{}

		template <typename T, typename... Ts>
		schema(T const& col, Ts const&... cols) : schema{}
		{
			insert(col, cols...);
		}

		template <typename T, typename... Ts>
		schema(T&& col, Ts&&... cols) : schema{}
		{
			insert(col, cols...);
		}

		template <typename... Ts>
		auto insert(col_type const& val, Ts const&... vals)
		{
			column_.push_back(val);

			if constexpr (!last())
			{
				next_.insert(vals...);
			}
		}

		template <typename... Ts>
		auto insert(col_type&& val, Ts&&... vals)
		{
			column_.push_back(val);

			if constexpr (!last())
			{
				next_.insert(vals...);
			}
		}

		template <typename... Ts>
		void insert(std::vector<col_type> const& val, Ts const&... vals)
		{
			column_.insert(std::end(column_), std::cbegin(val), std::cend(val));

			if constexpr (!last())
			{
				next_.insert(vals...);
			}
		}

		template <typename... Ts>
		void insert(std::vector<col_type>&& val, Ts&&... vals)
		{
			column_.insert(std::end(column_), std::begin(val), std::end(val));

			if constexpr (!last())
			{
				next_.insert(vals...);
			}
		}

		row_type begin() const
		{
			row_type row{};
			row.it_ = column_.cbegin();

			if constexpr (!last())
			{
				row.next_ = next_.begin();
			}
			
			return row;
		}

		row_type end() const
		{
			row_type row{};
			row.it_ = column_.cend();

			if constexpr (!last())
			{
				row.next_ = next_.end();
			}
			
			return row;
		}

	private:
		struct null_schema
		{};

		static inline constexpr bool last()
		{
			return sizeof...(Cols) == 0;
		}

		static inline constexpr auto resolve()
		{
			if constexpr (last()) 
			{
				return null_schema{};
			} 
			else
			{
				return schema<Cols...>{};
			}
		}

		using next_type = decltype(resolve());

		std::vector<col_type> column_;
		next_type next_;
	};

} // namespace sql
