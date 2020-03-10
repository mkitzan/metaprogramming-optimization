#include <iostream>
#include <string>
#include <vector>

#include "ra/relation.hpp"
#include "ra/rename.hpp"
#include "sql/schema.hpp"

int main()
{
	std::vector<int> ids{ 1914, 1985, 2020 };
	std::vector<std::string> names{ "g++", "needs", "concepts" };

	sql::schema<
		sql::index<>,
		sql::column<"id", int>,
		sql::column<"name", std::string>
	> table{ ids, names };
	
	using query =
	ra::rename< 
		typename sql::variadic_row<sql::column<"year", int>, sql::column<"word", std::string>>::row_type,
		ra::relation<decltype(table)>
	>;
	
	
	query::seed(table);

	try
	{
		for(;;)
		{
			auto row = query::next();
			std::cout << sql::get<"year">(row) << '\t' << sql::get<"word">(row) << '\n';
		}
	}
	catch(const ra::data_end& e)
	{}
}
