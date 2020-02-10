#include <iostream>
#include <string>
#include <vector>

#include "sql/schema.hpp"

int main()
{
	std::vector<int> ids{ 1914, 1985, 2020 };
	std::vector<std::string> names{ "g++", "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 9135.68, 1029.38 };

	sql::schema<
		sql::index<>,
		sql::column<"id", int>,
		sql::column<"name", std::string>,
		sql::column<"balance", double>
	> table{ ids, names, balances };

	auto row{ *table.begin() };

	std::cout << sql::get<"id">(row) << '\t'
	          << sql::get<"name">(row) << '\t'
	          << sql::get<"balance">(row) << '\n';
	
	for (auto [id, name, balance] : table)
	{
		std::cout << id << '\t' << name << '\t' << balance << '\n';
	}
}
