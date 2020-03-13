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

		inline constexpr next const& tail() const
		{
			return next_;
		}

		inline constexpr next& tail()
		{
			return next_;
		}

		inline constexpr column::type const& head() const
		{
			return value_;
		}

		inline constexpr column::type& head()
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
	constexpr auto const& get(Row const& r)
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
	constexpr auto const& get(Row const& r)
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

	namespace
	{
		
		template <typename Left, typename Right>
		constexpr auto recr_merge()
		{
			if constexpr (std::is_same<Left, sql::void_row>::value)
			{
				return Right{};
			}
			else
			{
				return sql::row<typename Left::column, decltype(recr_merge<typename Left::next, Right>())>{};
			}
		}

		template <typename Dest, typename Row>
		void recr_copy(Dest& dest, Row const& src)
		{
			if constexpr (std::is_same<Row, sql::void_row>::value)
			{
				return;
			}
			else
			{
				dest.head() = src.head();
				recr_copy(dest.tail(), src.tail());
			}
		}

	} // namespace

	// function merge two row types where key maybe a shared first column
	template <typename Left, typename Right>
	inline constexpr auto merge()
	{
		if constexpr (Left::column::name == Right::column::name)
		{
			return recr_merge<Left, typename Right::next>();
		}
		else
		{
			return recr_merge<Left, Right>();
		}
	}

	// function to join two rows into a merge row
	template <typename Dest, typename Row>
	inline void copy(Dest& dest, Row const& src)
	{
		if constexpr (Dest::column::name == Row::column::name)
		{
			recr_copy(dest, src);
		}
		else
		{
			copy(dest.tail(), src);
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
