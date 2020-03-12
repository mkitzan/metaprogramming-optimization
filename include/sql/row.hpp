#pragma once

#include <type_traits>
#include <vector>

#include "cexpr/string.hpp"

namespace sql
{

	struct void_row
	{
		static constexpr std::size_t depth{ 0 };
	};

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

		inline next const& tail() const
		{
			return next_;
		}

		inline next& tail()
		{
			return next_;
		}

		inline column::type const& head() const
		{
			return value_;
		}

		inline column::type& head()
		{
			return value_;
		}
	
	private:
		column::type value_;
		next next_;
	};

	template <typename Col, typename... Cols>
	struct variadic_row
	{
	private:
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
	inline constexpr auto const& get(Row const& r)
	{
		if constexpr (Row::column::name == Name)
		{
			return r.head();
		}
		else
		{
			return get<Name>(r.tail());
		}
	}

	// compiler function used by structured binding declaration
	template <std::size_t Pos, typename Row>
	inline constexpr auto const& get(Row const& r)
	{
		if constexpr (Pos == 0)
		{
			return r.head();
		}
		else
		{
			return get<Pos - 1>(r.tail());
		}
	}

	// function to assign a value to a column's value in a row
	template <cexpr::string Name, typename Row, typename T>
	constexpr void set(Row& r, T const& value)
	{
		if constexpr (Row::column::name == Name)
		{
			r.head() = value;
		}
		else
		{
			set<Name>(r.tail(), value);
		}
	}

	// function to query the sql::column type of a column name
	template <cexpr::string Name, typename Row>
	constexpr auto col()
	{
		if constexpr (Row::column::name == Name)
		{
			return Row::column;
		}
		else
		{
			return col<Name, typename Row::next>();
		}
	}

	// function to query the type of two rows merged together
	template <typename Left, typename Right>
	constexpr auto link()
	{
		if constexpr (std::is_same<Left, sql::void_row>::value)
		{
			return Right{};
		}
		else
		{
			return sql::row<typename Left::column, decltype(link<typename Left::next, Right>())>{};
		}
	}

	// function to join two rows into a merge row
	template <typename Dest, typename Left, typename Right>
	constexpr void merge(Dest& d, Left const& l, Right const& r)
	{
		if constexpr (std::is_same<Left, sql::void_row>::value)
		{
			if constexpr (std::is_same<Right, sql::void_row>::value)
			{
				return;
			}
			else
			{
				d.head() = r.head();
				merge(d.tail(), l, r.tail());	
			}
		}
		else
		{
			d.head() = l.head();
			merge(d.tail(), l.tail(), r);
		}
	}

	// function to query whether a row type has a column name
	template <cexpr::string Name, typename Row>
	constexpr bool exists()
	{
		if constexpr (std::is_same<Row, sql::void_row>::value)
		{
			return false;
		}
		else
		{
			if constexpr (Row::column::name == Name)
			{
				return true;
			}
			else
			{
				return exists<Name, typename Row::next>();
			}
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
