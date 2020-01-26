#include <iostream>

#include "cexpr/infix.hpp"
#include "cexpr/prefix.hpp"

int main()
{
	constexpr auto infix{ cexpr::infix<"( x0 + 50 * 2 ) * ( x1 * 10 )", int>::eval(6, 10) };
	constexpr auto prefix{ cexpr::prefix<"* + x0 * 50 2 * x1 10", int>::eval(6, 10) };

	std::cout << "Infix parser:\t" << infix << "\nPrefix parser:\t" << prefix << '\n';

	return 0;
}
