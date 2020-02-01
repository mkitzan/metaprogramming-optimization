#include <iostream>
#include <string>

#include "sql/column.hpp"
#include "sql/schema.hpp"

int main()
{
	sql::schema<
		sql::column<"id", int>, 
		sql::column<"account", double>, 
		sql::column<"name", std::string>
	> table{};
	
	table.insert(42, 12345.67, std::string{ "me" });
	auto const& column{ select<"account">(table) };

	std::cout << column.at(0) << std::endl;
}
