#pragma once

#include <cstdarg>
#include <array>

#include "cexpr_string.hpp"

template<cexpr_string S>
class cexpr_math {
public:
	template<typename ...T>
	static constexpr T eval(T&&... args) 
	{
		const std::array<T, sizeof...(args)> values{ args... };
		T acc{};
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

private:

};

