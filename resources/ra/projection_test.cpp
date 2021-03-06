#include <iostream>
#include <string>
#include <vector>

#include "ra/projection.hpp"
#include "ra/relation.hpp"
#include "sql/schema.hpp"

int main()
{
	std::vector<int> ids{ 1914, 1985, 2020 };
	std::vector<double> balances{ 1357.24, 9135.68, 1029.38 };
	std::vector<std::string> names{ "g++", "needs", "concepts" };

	sql::schema<
		sql::index<>,
		sql::column<"id", int>,
		sql::column<"balance", double>,
		sql::column<"name", std::string>
	> table{ ids, balances, names };

	for (auto [id, balance, name] : table)
	{
		std::cout << id << '\t' << balance << '\t' << name << '\n';
	}

	using query =
	ra::projection<
		typename sql::variadic_row<sql::column<"id", int>, sql::column<"balance", double>>::row_type,
		ra::relation<decltype(table)>
	>;
	
	query::seed(table);

	try
	{
		std::cout << '\n';
		for(;;)
		{
			auto [id, balance] = query::next();
			std::cout << id << '\t' << balance << '\n';
		}
	}
	catch(const ra::data_end& e)
	{}
}
