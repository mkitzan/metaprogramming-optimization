#pragma once

#include <array>

template<typename ValT>
constexpr ValT convert(char* str)
{
	constexpr char nul{ '\0' }, dot{ '.' }, zro{ '0' }, min{ '-' };
	ValT acc{}, sign{ 1 }, scalar{ 10 };

	if (*str == min) {
		sign = -1;
		++str;
	}

	while (*str != nul && *str != dot) {
		acc = (acc * scalar) + (*str - zro);
		++str;
	}

	if (*str == dot) {
		scalar = 1;
		++str;

		while(*str != nul) {
			acc += (*str - zro) * (scalar /= ValT{ 10 });
			++str;
		}
	}

	return acc * sign;
}

template<typename ValT, char Op, typename Left, typename Right>
class Operator {
public:
	template<std::size_t N>
	static constexpr ValT eval(std::array<ValT, N> const& args)
	{
		ValT result{};

		if constexpr (Op == '+') {
			result = Left::eval(args) + Right::eval(args);
		} else if constexpr(Op == '-') {
			result = Left::eval(args) - Right::eval(args);
		} else if constexpr(Op == '*') {
			result = Left::eval(args) * Right::eval(args);
		} else if constexpr(Op == '/') {
			result = Left::eval(args) / Right::eval(args);
		}

		return result;
	}
};

template<typename ValT, ValT Pos>
class Variable {
public:
	template<std::size_t N>
	static constexpr ValT eval(std::array<ValT, N> const& args)
	{
		return args[Pos];
	}
};

template<typename ValT, ValT Const>
class Constant {
public:
	template<std::size_t N>
	static constexpr ValT eval(std::array<ValT, N> const& args)
	{
		return Const;
	}
};
