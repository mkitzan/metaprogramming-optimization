#include <iostream>
#include <string>
#include <vector>

#include "sql/column.hpp"
#include "sql/schema.hpp"

int main()
{
	sql::schema<
		sql::column<"id", int>, 
		sql::column<"balance", double>, 
		sql::column<"name", std::string>
	> table{ 1914, 1029.38, std::string{ "g++" } };
	
	std::vector<int> ids{ 1985, 2020 };
	std::vector<double> balances{ 1357.24, 9135.68 };
	std::vector<std::string> names{ "needs", "concepts" };

	table.insert(ids, balances, names);
	
	for (auto const& [id, balance, name] : table)
	{
		std::cout << id << '\t' << balance << '\t' << name << '\n';
	}
}
