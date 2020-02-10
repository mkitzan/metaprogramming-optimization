#include <iostream>
#include <string>
#include <vector>

#include "ra/relation.hpp"
#include "ra/rename.hpp"
#include "sql/column.hpp"
#include "sql/schema.hpp"

int main()
{
	std::vector<int> ids{ 1914, 1985, 2020 };
	std::vector<std::string> names{ "g++", "needs", "concepts" };

	sql::schema<
		sql::column<"id", int>,
		sql::column<"name", std::string>
	> table{ ids, names };
	
	using query = ra::rename<ra::relation<decltype(table)>, sql::column<"year", int>, sql::column<"word", std::string>>;
	query::seed(table);

	try
	{
		for(;;)
		{
			auto row = query::next();
			std::cout << sql::get<"year">(row) << '\t' << sql::get<"word">(row) << '\n';
		}
	}
	catch(const std::exception& e)
	{}
}
