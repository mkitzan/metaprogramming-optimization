#include <iostream>
#include "cexpr_equation.hpp"

int main()
{
	constexpr auto f{ cexpr_equation<"* + x0 * 50 2 * x1 10", int>::eval(6, 10) };
	std::cout << f << std::endl;
}
