#include <iostream>

#include "data.hpp"
#include "sql/query.hpp"

using query =
	sql::query<
		"select genre, year, pages from T0 where genre != \"science fiction\" and NOT year > 1970 and not pages = 300",
		books
	>;

int main()
{
	books t0{ sql::load<books, '\t'>(data_folder + books_data) };

	for (query q{ t0 }; auto const& [c0, c1, c2] : q)
	{
		std::cout << c0 << '|' << c1 << '|' << c2 << '\n';
	}

	return 0;
}
