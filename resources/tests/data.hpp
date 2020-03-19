#pragma once

#include <string>
#include <type_traits>

#include "sql/schema.hpp"

using books =
	sql::schema<
		sql::index<>,
		sql::column<"book", std::string>,
		sql::column<"genre", std::string>,
		sql::column<"year", unsigned>,
		sql::column<"pages", unsigned>
	>;

using stories =
	sql::schema<
		sql::index<>,
		sql::column<"story", std::string>,
		sql::column<"genre", std::string>,
		sql::column<"year", unsigned>
	>;

using authored =
	sql::schema<
		sql::index<>,
		sql::column<"title", std::string>,
		sql::column<"name", std::string>
	>;

using collected =
	sql::schema<
		sql::index<>,
		sql::column<"title", std::string>,
		sql::column<"collection", std::string>,
		sql::column<"pages", unsigned>
	>;

const std::string data_folder{ "./data/" };
const std::string books_data{ "books-table.tsv" };
const std::string stories_data{ "stories-table.tsv" };
const std::string authored_data{ "authored-table.tsv" };
const std::string collected_data{ "collected-table.tsv" };
