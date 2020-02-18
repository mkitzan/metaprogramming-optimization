#include <iostream>
#include <exception>
#include <string>
#include <string_view>
#include <vector>

// global tokenizer data simulating cexpr templ 
size_t POS{};
std::vector<std::string_view> TOKENS{};
bool has_aggregate{}, has_groupby{}, has_where{}, has_rename{};

void assert(bool valid)
{
	if (!valid)
	{
		throw std::exception{};
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
		for (; i < str.size() && syntax(str[i]); ++i);
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

void from(){} 

void project(){}

void aggregate(){}

void rename(){}

void select()
{
	assert(!syntax(TOKENS[POS][0]));

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

	assert(TOKENS[POS++] == "select");

	if (TOKENS[POS] == "distinct")
	{
		++POS;
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
	parse("select name<str>,count(name<str>)as instances from table where id=1940 group by name");
	return 0;
}
