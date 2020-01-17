#include <iostream>
#include "cexpr_string.hpp"

template<cexpr_string S>
class Equation {
public:
	constexpr int operator()(int x, int y) 
	{
		int res{};

		switch(S[1]) 
		{
		case '+':
			res = x + y; break;
		case '-':
			res = x - y; break;
		case '*':
			res = x * y; break;
		case '/':
			res = x / y; break;
		}

		return res;
	}
};

int main()
{
	constexpr auto result{ Equation<"x+y">(2, 2) };
	std::cout << result << std::endl;
	return 0;
}
