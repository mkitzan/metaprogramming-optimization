#include <iostream>
#include "cexpr_string.hpp"

template<cexpr_string S>
class Equation {
public:
	static constexpr int eval(int x, int y) 
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

// trust me it works with g++-9.2
int main()
{
	constexpr auto result{ Equation<"x+y">::eval(2, 2) };
	std::cout << result << std::endl;
	return 0;
}
