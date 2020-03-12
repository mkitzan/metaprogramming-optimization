#include <iostream>
#include <string>
#include <vector>

#include "sql/query.hpp"
#include "sql/schema.hpp"

namespace
{

	using left_schema =
		sql::schema<
			sql::index<>,
			sql::column<"attr", std::string>
		>;
	using right_schema =
		sql::schema<
			sql::index<>,
			sql::column<"id", int>,
			sql::column<"name", std::string>,
			sql::column<"balance", double>
		>;
	using query =
		sql::query<
			"select id as year, balance, name as comment, attr from T0 cross join T1 where id >= 1945 and not balance < 1500.0",
			left_schema,
			right_schema
		>;

} // namespace


int main()
{
	std::vector<std::string> attrs{ "corona", "virus" };
	std::vector<int> ids{ 1914, 1945, 1985, 2020 };
	std::vector<std::string> names{ "skip", "g++", "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 3254.65, 9135.68, 1829.38 };

	left_schema left{ attrs };
	right_schema right{ ids, names, balances };

	for (query q{ left, right }; auto const& [year, balance, comment, attr] : q)
	{
		std::cout << year << '\t' << balance << '\t' << comment << '\t' << attr << '\n';
	}
}
