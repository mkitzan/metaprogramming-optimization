#include <iostream>
#include <string>
#include <vector>

#include "sql/column.hpp"
#include "sql/schema.hpp"

int main()
{
	sql::schema<
		sql::column<"id", int>,
		sql::column<"name", std::string>,
		sql::column<"balance", double>
	> table{ 1914, std::string{ "g++" }, 1029.38 };

	auto row_iter{ table.begin() };

	std::cout << sql::get<"id">(row_iter) << '\t'
	          << sql::get<"name">(row_iter) << '\t'
	          << sql::get<"balance">(row_iter) << '\n';

	std::vector<int> ids{ 1985, 2020 };
	std::vector<std::string> names{ "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 9135.68 };

	table.insert(ids, names, balances);
	
	for (auto [id, name, balance] : table)
	{
		std::cout << id << '\t' << name << '\t' << balance << '\n';
	}
}
