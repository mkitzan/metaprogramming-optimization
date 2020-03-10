#include <iostream>
#include <string>
#include <vector>

#include "sql/query.hpp"
#include "sql/schema.hpp"

int main()
{
	std::vector<int> ids{ 1914, 1985, 2020 };
	std::vector<std::string> names{ "g++", "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 9135.68, 1029.38 };

	using schema =
		sql::schema<
			sql::index<>,
			sql::column<"id", int>,
			sql::column<"name", std::string>,
			sql::column<"balance", double>
		>;
	using query =
		sql::query<
			"select id as year, balance, name as comment from T0 where id > 1945 and not balance < 1500.0",
			schema
		>;
	
	schema table{ ids, names, balances };
	query::seed(table);

	auto row{ query::next() };

	std::cout << sql::get<"year">(row) << '\t'
	          << sql::get<"balance">(row) << '\t'
	          << sql::get<"comment">(row) << '\n';
	
	for (auto [id, name, balance] : table)
	{
		std::cout << id << '\t' << name << '\t' << balance << '\n';
	}
}
