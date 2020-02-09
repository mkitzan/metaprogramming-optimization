#pragma once

#include <vector>

#include "cexpr/string.hpp"

namespace sql
{

	template <typename Col, typename... Cols>
	class row
	{
		using col_type = typename Col::type;
	public:
		row() = default;

		template <typename... ColTs>
		row(col_type const& val, ColTs const&... vals) : value_{ val }, next_{ vals... }
		{}

		inline bool operator==(row const& r) const
		{
			if constexpr (!last())
			{
				return (value_ == r.value) && (next_ == r.next_);
			} else
			{
				return (next_ == r.next_);
			}
		}

		inline bool operator!=(row const& r) const
		{
			return !(*this == r);
		}
	
	private:		
		template <typename...>
		friend class schema;

		template <cexpr::string Name, typename Row>
		friend constexpr auto const& get(Row const& r);

		template <std::size_t Pos, typename Row>
		friend constexpr auto const& get(Row const& r);

		template <cexpr::string Name, typename Row, typename T>
		friend constexpr void set(Row& r, T const& value);

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

		col_type value_;
		next_type next_;
	};

	// user function to query row elements by column name
	template <cexpr::string Name, typename Row>
	constexpr auto const& get(Row const& r)
	{
		if constexpr (Row::name == Name)
		{
			return r.value_;
		}
		else
		{
			return get<Name>(r.next_);
		}
	}

	// compiler function used by structured binding declaration
	template <std::size_t Pos, typename Row>
	constexpr auto const& get(Row const& r)
	{
		if constexpr (Pos == 0)
		{
			return r.value_;
		}
		else
		{
			return get<Pos - 1>(r.next_);
		}
	}

	template <cexpr::string Name, typename Row, typename T>
	constexpr void set(Row& r, T const& value)
	{
		if constexpr (Row::name == Name)
		{
			r.value_ = value;
		}
		else
		{
			set<Name>(r.next_, value);
		}
	}

	// function to query the sql::column type of a column name
	template <cexpr::string Name, typename Row>
	constexpr auto col(Row const& r)
	{
		if constexpr (Row::name == Name)
		{
			return Row::col_type;
		}
		else
		{
			return get<Name>(r.next_);
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
