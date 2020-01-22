#include <iostream>
#include "cexpr_equation.hpp"

int main()
{
	std::cout << cexpr_equation<"+ x0 x1", int>::eval(2, 5) << std::endl;
}
