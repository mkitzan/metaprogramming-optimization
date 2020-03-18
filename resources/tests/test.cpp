#include <iostream>

#include "data.hpp"
#include "sql/query.hpp

using query =
	sql::query<
		"select genre as type, year as published, name from books natural join authored where NOT genre = \"science fiction\" or NOT year < 1970 or name <> \"Harlan Elison\"",
		books,
		authored
	>;

int main()
{
	books t0{ sql::load<books, '\t'>(data_folder + books_data) };
	authored t1{ sql::load<authored, '\t'>(data_folder + authored_data) };

	for (query q{ t0, t1 }; auto const& [c0, c1, c2] : q)
	{
		std::cout << c0 << '\t' << c1 << '\t' << c2 << '\n';
	}

	return 0;
}
