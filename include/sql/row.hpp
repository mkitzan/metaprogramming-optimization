#pragma once

#include <vector>

#include "cexpr/string.hpp"
#include "sql/schema.hpp"

namespace sql
{

	template <typename Col, typename... Cols>
	class row
	{
		using row_iterator = std::vector<typename Col::type>::const_iterator;
	public:
		inline bool operator==(row const& r) const
		{
			if constexpr (!last())
			{
				return (next_ == r.next_) && (it_ == r.it_);
			} else
			{
				return (it_ == r.it_);
			}
		}

		inline bool operator!=(row const& r) const
		{
			return !(*this == r);
		}

		inline row& operator++()
		{
			++it_;

			if constexpr (!last())
			{
				++next_;
			}

			return *this;
		}

		// shim function used by structured binding declaration
		inline row const& operator*() const
		{
			return *this;
		}

	private:		
		template <typename, typename...>
		friend class schema;

		template <cexpr::string Name, typename Row>
		friend auto const& get(Row const& r);

		template <std::size_t Pos, typename Row>
		friend auto const& get(Row const& r);

		struct null_row
		{};

		static inline constexpr bool last()
		{
			return sizeof...(Cols) == 0;
		}

		static inline constexpr auto resolve()
		{
			if constexpr (last()) 
			{
				return null_row{};
			} 
			else
			{
				return row<Cols...>{};
			}
		}

		using next_type = decltype(resolve());

		static constexpr auto name{ Col::name };

		row_iterator it_;
		next_type next_;
	};

	// user function to query row elements by column name
	template <cexpr::string Name, typename Row>
	auto const& get(Row const& r)
	{
		if constexpr (Row::name == Name)
		{
			return *(r.it_);
		}
		else
		{
			return get<Name>(r.next_);
		}
	}

	// compiler function used by structured binding declaration
	template <std::size_t Pos, typename Row>
	auto const& get(Row const& r)
	{
		if constexpr (Pos == 0)
		{
			return *(r.it_);
		}
		else
		{
			return get<Pos - 1>(r.next_);
		}
	}

} // namespace sql

// STL injections to allow row to be used in structured binding declarations
namespace std
{

	template <typename... Ts>
	class tuple_size<sql::row<Ts...>> : public integral_constant<size_t, sizeof...(Ts)>
	{};

	template <size_t I, typename... Ts>
	struct tuple_element<I, sql::row<Ts...>>
	{
		using type = decltype(sql::get<I>(sql::row<Ts...>{}));
	};

} // namespace std
