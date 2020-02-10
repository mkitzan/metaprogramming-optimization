#pragma once

#include <type_traits>
#include <vector>

#include "cexpr/string.hpp"

namespace sql
{

	template <typename Col, typename Next>
	class row
	{
	public:
		using column = Col;
		using next = Next;
		static constexpr std::size_t depth{ 1 + next::depth };

		row() = default;

		template <typename... ColTs>
		row(column::type const& val, ColTs const&... vals) : value_{ val }, next_{ vals... }
		{}
	
	private:
		template <cexpr::string Name, typename Row>
		friend constexpr auto const& get(Row const& r);

		template <std::size_t Pos, typename Row>
		friend constexpr auto const& get(Row const& r);

		template <cexpr::string Name, typename Row, typename T>
		friend constexpr void set(Row& r, T const& value);

		column::type value_;
		next next_;
	};

	template <typename Col, typename... Cols>
	struct variadic_row
	{
	private:
		struct void_row
		{
			static constexpr std::size_t depth{ 0 };
		};

		static inline constexpr auto resolve()
		{
			if constexpr (sizeof...(Cols) != 0)
			{
				return typename variadic_row<Cols...>::row_type{};
			}
			else
			{
				return void_row{};
			}
		}

	public:
		using row_type = row<Col, decltype(resolve())>;
	};

	// user function to query row elements by column name
	template <cexpr::string Name, typename Row>
	constexpr auto const& get(Row const& r)
	{
		if constexpr (Row::column::name == Name)
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
		if constexpr (Row::column::name == Name)
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
		if constexpr (Row::column::name == Name)
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

	template <typename Col, typename Next>
	class tuple_size<sql::row<Col, Next>> : public integral_constant<size_t, sql::row<Col, Next>::depth>
	{};

	template <size_t I, typename Col, typename Next>
	struct tuple_element<I, sql::row<Col, Next>>
	{
		using type = decltype(sql::get<I>(sql::row<Col, Next>{}));
	};

} // namespace std
