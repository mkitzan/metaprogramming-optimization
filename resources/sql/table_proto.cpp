#include <iostream>
#include <string>
#include <vector>

#include "cexpr/string.hpp"

template <cexpr::string N, typename T>
struct Column
{
	static constexpr auto Name = N;
	using Type = T;
};

struct End
{
	constexpr bool operator==(End other)
	{
		return true;
	}
};

template <typename Col, typename... Cols>
struct Schema
{
	constexpr bool operator==(End e)
	{
		return false;
	}

	static constexpr auto resolve()
	{
		if constexpr (sizeof...(Cols) > 0) 
		{
			return Schema<Cols...>{};
		} 
		else
		{
			return End{};
		}
	}

	using Next = decltype(resolve());

	static constexpr auto name{ Col::Name };
	std::vector<typename Col::Type> column{};
	Next next{};
};

template <cexpr::string Key, typename Table>
auto const& select(Table& table)
{
	if constexpr (table.name == Key)
	{
		return table.column;
	}
	else
	{
		return select<Key, typename Table::Next>(table.next);
	}
}

template <typename Table, typename T, typename... Ts>
void insert(Table& table, T val, Ts&&... vals)
{
	table.column.push_back(val);

	if constexpr (table.next == End{})
	{
		return;
	}
	else
	{
		return insert(table.next, vals...);
	}
}

template <typename Table, typename T, typename... Ts>
void bind(Table& table, T&& val, Ts&&... vals)
{
	table.column = val;

	if constexpr (table.next == End{})
	{
		return;
	}
	else
	{
		return bind(table.next, vals...);
	}
}

int main()
{
	Schema<	Column<"id", int>, Column<"account", double>, Column<"name", std::string> > table{};
	
	insert(table, 42, 12345.67, std::string{ "me" });
	auto const& column{ select<"account">(table) };

	std::cout << column.at(0) << std::endl;
}
