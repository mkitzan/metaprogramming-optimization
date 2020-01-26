#include <iostream>
#include "cexpr/prefix.hpp"

int main()
{
	constexpr auto f{ cexpr::prefix<"* + x0 * 50 2 * x1 10", int>::eval(6, 10) };
	std::cout << f << std::endl;

	return 0;
}
