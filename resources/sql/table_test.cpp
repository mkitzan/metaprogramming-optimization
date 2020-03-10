#include <iostream>
#include <string>
#include <vector>

#include "sql/query.hpp"
#include "sql/schema.hpp"

namespace
{

	using schema =
		sql::schema<
			sql::index<>,
			sql::column<"id", int>,
			sql::column<"name", std::string>,
			sql::column<"balance", double>
		>;
	using query =
		sql::query<
			"select id as year, balance, name as comment from T0 where id >= 1945 and not balance < 1500.0",
			schema
		>;

} // namespace


int main()
{
	std::vector<int> ids{ 1914, 1945, 1985, 2020 };
	std::vector<std::string> names{ "skip", "g++", "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 3254.65, 9135.68, 1829.38 };

	schema table{ ids, names, balances };
	query q{ table };

	for (auto [year, balance, comment] : q)
	{
		std::cout << year << '\t' << balance << '\t' << comment << '\n';
	}
}
