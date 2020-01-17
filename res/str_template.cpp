#include <iostream>
#include <cstdarg>
#include <initializer_list>
#include <array>
#include "cexpr_string.hpp"

template<cexpr_string S>
class Equation {
public:
	template<typename ...T>
	static constexpr int eval(T&&... args) 
	{
		std::array<int, sizeof...(args)> values{ args... };
		int acc{};
		char op{};
		bool start{ true };

		for (auto c : S) 
		{
			if (op == 0 && !start)
			{
				op = c;
			}
			else
			{
				if (op == '*')
				{
					acc *= values[c - 'a'];
				}
				else
				{
					acc += values[c - 'a'];
				}

				op = 0;
			}

			start = false;
		}

		return acc;
	}
};

// trust me it works with g++-9.2
int main()
{
	constexpr auto result{ Equation<"a+b+a">::eval(2, 3) };
	std::cout << result << std::endl;
	return 0;
}
