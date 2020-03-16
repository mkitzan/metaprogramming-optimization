#pragma once

#include <fstream>
#include <string>
#include <type_traits>

#include "sql/schema.hpp"

using books =
	sql::schema<
		sql::index<>,
		sql::column<"title", std::string>,
		sql::column<"genre", std::string>,
		sql::column<"year", unsigned>,
		sql::column<"pages", unsigned>
	>;

using stories =
	sql::schema<
		sql::index<>,
		sql::column<"title", std::string>,
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
		sql::column<"story", std::string>,
		sql::column<"collection", std::string>,
		sql::column<"pages", unsigned>
	>;

const std::string data_folder{ "../data/" };

namespace
{

	template <typename Row, char Delim>
	void fill(std::fstream& file, Row& row)
	{
		if constexpr (!std::is_same_v<Row, sql::void_row>)
		{
			if constexpr (std::is_same_v<typename Row::column::type, std::string>)
			{
				if constexpr (std::is_same_v<typename Row::next, sql::void_row>)
				{
					std::getline(file, row.head());
				}
				else
				{
					std::getline(file, row.head(), Delim);
				}
			}
			else
			{
				file >> row.head();
			}

			fill<typename Row::next>(file, row.tail());
		}
	}

} // namespace

template <typename Schema, char Delim>
Schema load(std::string const& data)
{
	auto file{ std::fstream(data_folder + data) };
	Schema table{};
	typename Schema::row_type row{};

	while (file)
	{
		fill<typename Schema::row_type, Delim>(file, row);
		table.insert(std::move(row));
	}

	return table;
}
