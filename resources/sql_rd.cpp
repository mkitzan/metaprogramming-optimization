#include <iostream>
#include <exception>
#include <string>
#include <string_view>
#include <vector>

// global tokenizer data simulating cexpr templ 
size_t POS{};
std::vector<std::string_view> TOKENS{};
bool has_aggregate{}, has_groupby{}, has_where{}, has_rename{};

void assert(bool valid, std::string msg)
{
	if (!valid)
	{
		throw std::runtime_error{ msg };
	}
}

bool syntax(char c)
{
	return c==',' || c=='(' || c==')' || c=='\'' || c=='\"' || c=='<' || c=='>' || c=='=';
}

size_t skip(size_t i, std::string const& str)
{
	for (; i < str.size() && str[i] == ' '; ++i);
	return i;
}

bool agg(std::string_view const& str)
{
	return str == "count" || str == "avg" || str == "max" || str == "min" || str == "sum";
}

size_t next(size_t i, std::string const& str)
{
	if (syntax(str[i]))
	{
		++i;
	}
	else
	{
		for (; i < str.size() && str[i] != ' ' && !syntax(str[i]); ++i);
	}
	
	return i;
}

void flags()
{
	auto size{ TOKENS.size() - 1 };

	if (size > 0)
	{
		has_aggregate |= agg(TOKENS[size]) && syntax(TOKENS[size - 1][0]);
		has_groupby |= TOKENS[size] == "by" && TOKENS[size] == "group";
		has_where |= TOKENS[size] == "where";
		has_rename |= TOKENS[size] == "as";
	}
}

void tokenize(std::string const& query)
{
	auto str{ query.c_str() };
	auto curr{ (size_t) 0 }, last{ (size_t) 0 };

	while (curr < query.size())
	{
		curr = skip(curr, query);
		last = curr;
		last = next(last, query);

		TOKENS.emplace_back(&str[curr], last - curr);
		curr = last;

		flags();
	}

	for (auto const& token : TOKENS)
	{
		std::cout << token << '\n';
	}
}

void expression(){}

void having(){}

void groupby(){}

void where(){}

void from()
{

}

void column(size_t& i)
{
	std::cout << '\t' << TOKENS[i];
	// name = TOKENS[i];
	assert(TOKENS[++i] == "<", "Missing \'<\'");
	// type = parse_type(TOKENS[++i]);
	std::cout << '\t' << TOKENS[++i];
	assert(TOKENS[++i] == ">", "Missing \'>\'");
	++i;
}

void recr_project()
{
	assert(!syntax(TOKENS[POS][0]), "Unexpected syntax");

	// form column
	if (agg(TOKENS[POS]))
	{
		POS += 7;
	}
	else
	{
		column(POS);
	}

	if (TOKENS[POS] == "as")
	{
		POS += 2;
	}

	if (TOKENS[POS] == ",")
	{
		std::cout << std::endl;
		++POS;
		recr_project();
	}
	else if (TOKENS[POS] == "from")
	{
		// return void_row;
		return;
	}
	else
	{
		assert(false, "Unexpected token");
	}
}

void project()
{
	std::cout << "Project schema\n";
	recr_project();
	std::cout << std::endl;
	from();
}

void recr_aggregate(size_t i)
{
	assert(!syntax(TOKENS[i][0]), "Unexpected syntax");

	// form column
	if (agg(TOKENS[i]))
	{
		std::cout << "\t" << TOKENS[i];
		//agg_type(TOKENS[i]);
		assert(TOKENS[++i] == "(", "Missing \'(\'");
		column(++i);
		assert(TOKENS[i] == ")", "Missing \')\'");
		++i;
	}
	else
	{
		std::cout << "\t";
		column(i);
	}

	if (TOKENS[i] == "as")
	{
		i += 2;
	}

	if (TOKENS[i] == ",")
	{
		std::cout << std::endl;
		recr_aggregate(++i);
	}
	else if (TOKENS[i] == "from")
	{
		// return void_row;
		return;
	}
	else
	{
		assert(false, "Unexpected token");
	}
}

void aggregate()
{
	std::cout << "Aggregate schema\n";
	recr_aggregate(POS);
	std::cout << std::endl;
	project();
}

void recr_rename(size_t i)
{
	assert(!syntax(TOKENS[i][0]), "Unexpected syntax");

	// form column
	if (agg(TOKENS[i]))
	{
		std::cout << "\t" << TOKENS[i];
		//agg_type(TOKENS[i]);
		assert(TOKENS[++i] == "(", "Missing \'(\'");
		column(++i);
		assert(TOKENS[i] == ")", "Missing \')\'");
		++i;
	}
	else
	{
		std::cout << "\t";
		column(i);
	}

	if (TOKENS[i] == "as")
	{
		std::cout << '\t' << TOKENS[++i];
		// column name = TOKENS[++i]
		// column type = previous column's type
		++i;
	}

	if (TOKENS[i] == ",")
	{
		std::cout << std::endl;
		recr_rename(++i);
	}
	else if (TOKENS[i] == "from")
	{
		// return void_row;
		return;
	}
	else
	{
		assert(false, "Unexpected token");
	}
}

void rename()
{
	std::cout << "Rename schema\n";
	recr_rename(POS);
	std::cout << std::endl;
	aggregate();
}

void select()
{
	assert(!syntax(TOKENS[POS][0]), "Unexpected syntax");

	if (has_rename)
	{
		rename(); // -> aggregate -> project
	}
	else if (has_aggregate)
	{
		aggregate(); // -> project
	}
	else
	{
		project();
	}
}

void parse(std::string query)
{
	TOKENS.clear();
	POS = 0;
	tokenize(query);

	std::cout << "===---------------===\n";

	assert(TOKENS[POS++] == "select", "Expected \'select\'");

	if (TOKENS[POS] == "distinct")
	{
		++POS;
		std::cout << "Distinct node\n";
		// unary op distinct node
		select();
	}
	else
	{
		if (TOKENS[POS] == "all")
		{
			++POS;
		}

		select();
	}
}

int main()
{
	parse("select name<str>,count(name<str>)as counts from T0 where id=1940 group by name");
	return 0;
}
