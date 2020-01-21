#include <iostream>

#include "cexpr_string.hpp"
#include "cexpr_math.hpp"

int main()
{
	constexpr cexpr_math<"a ^ 3.11 / ( b + -0.9 ) * c"> e{};
	std::cout << e(2, 3, 4) << std::endl;
	return 0;
}
