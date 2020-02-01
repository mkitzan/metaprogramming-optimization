#pragma once

#include <iterator>
#include <type_traits>
#include <vector>

#include "cexpr/string.hpp"
#include "sql/column.hpp"

namespace sql
{
	
	template <typename Col, typename... Cols>
	class schema
	{
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

		// friend to circumvent g++9.2 constexpr obj template param parsing bug
		template <cexpr::string Key>
		friend constexpr auto const& select(schema const& table)
		{
			if constexpr (table.name_ == Key)
			{
				return table.column_;
			}
			else
			{
				return select<Key>(table.next_);
			}
		}

		template <typename... Ts>
		auto insert(typename Col::Type const& val, Ts const&... vals)
		{
			column_.push_back(val);

			if constexpr (!last())
			{
				return next_.insert(vals...);
			}
		}

		template <typename... Ts>
		auto insert(typename Col::Type&& val, Ts&&... vals)
		{
			column_.push_back(val);

			if constexpr (!last())
			{
				return next_.insert(vals...);
			}
		}

		template <typename T, typename... Ts>
		void insert(std::vector<T> const& val, Ts const&... vals)
		{
			column_.insert(std::end(column_), std::cbegin(val), std::cend(val));

			if constexpr (!last())
			{
				return next_.insert(vals...);
			}
		}

		template <typename T, typename... Ts>
		void insert(std::vector<T>&& val, Ts&&... vals)
		{
			column_.insert(std::end(column_), std::begin(val), std::end(val));

			if constexpr (!last())
			{
				return next_.insert(vals...);
			}
		}
		
	private:
		struct end
		{};

		static inline constexpr bool last()
		{
			return sizeof...(Cols) == 0;
		}

		static inline constexpr auto resolve()
		{
			if constexpr (last()) 
			{
				return end{};
			} 
			else
			{
				return schema<Cols...>{};
			}
		}

		using next = decltype(resolve());

		static constexpr auto name_{ Col::Name };
		std::vector<typename Col::Type> column_;
		next next_;
	};

} // namespace sql
