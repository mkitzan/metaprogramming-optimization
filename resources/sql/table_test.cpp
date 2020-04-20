#include <iostream>
#include <string>
#include <vector>

#include "sql.hpp"

namespace
{

	using left_schema =
		sql::schema<
			"left",
			sql::index<>,
			sql::column<"id", int>,
			sql::column<"name", std::string>,
			sql::column<"balance", double>
		>;
	using right_schema =
		sql::schema<
			"right",
			sql::index<>,
			sql::column<"id", int>,
			sql::column<"attr", std::string>
		>;
	using query =
		sql::query<
			"select id as year, balance, name as comment, attr "
			"from left natural join right "
			"where id >= 1945 and not balance < 1500.0",
			left_schema,
			right_schema
		>;

} // namespace

int main()
{
	std::vector<int> left_ids{ 1914, 1945, 1985, 2020 };
	std::vector<std::string> names{ "skip", "g++", "needs", "concepts" };
	std::vector<double> balances{ 1357.24, 3254.65, 9135.68, 1829.38 };
	std::vector<int> right_ids{ 1945, 1985, 1945, 1985 };
	std::vector<std::string> attrs{ "corona", "virus", "COBRA", "Normandy" };

	left_schema left{ left_ids, names, balances };
	right_schema right{ right_ids, attrs };

	for (query q{ left, right }; auto const& [year, balance, comment, attr] : q)
	{
		std::cout << year << '\t' << balance << '\t' << comment << '\t' << attr << '\n';
	}
}
