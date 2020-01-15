#include <cmath>
#include <string>
#include <cassert>
#include <iostream>

inline constexpr bool is_digit(char ch) 
{
	return (ch >= '0' && ch <= '9') || ch == '.';
}

inline constexpr bool is_number(const char* str) 
{
	return is_digit(str[0]) || (str[0] == '-' && is_digit(str[1]));
}

inline constexpr bool is_operator(const char* str)
{
	return str[0] == '+' || (str[0] == '-' && !is_digit(str[1])) || 
		str[0] == '*' || str[0] == '/' || str[0] == '(' || str[0] == ')' || str[0] == '^';
}

constexpr size_t next_token(const char *src, char* dest)
{
	size_t i{};

	for (; src[i] != '\0' && src[i] != ' '; ++i) {
		dest[i] = src[i];
	}
	dest[i] = '\0';

	if (src[i] == ' ') {
		++i;
	}

	return i;
}

constexpr size_t copy_token(const char *src, char *dest)
{
	size_t i{};

	for (; src[i] != '\0'; ++i) {
		dest[i] = src[i];
	}

	if (i) {
		dest[i++] = ' ';
	}
	dest[i] = '\0';

	return i;
}

constexpr double calculate(char op, double left, double right)
{
	double result{};

	switch (op) {
	case '+':
		result = left + right;
		break;
	case '-':
		result = left - right;
		break;
	case '*':
		result = left * right;
		break;
	case '/':
		result = left / right;
		break;
	case '^':
		result = std::pow(left, right);
		break;
	default:
		result = NAN;
		break;
	}

	return result;
}

constexpr double str_to_double(const char *str)
{
	double result{}, scalar{ 10 }, final{};
	
	if (*str == '-') {
		final = -1;
		++str;
	} else {
		final = 1;
	}

	while (*str != '\0' && *str != '.') {
		result = (result * scalar) + (*str - '0');
		++str;
	}

	if (*str == '.') {
		scalar = 1;
		++str;

		while(*str != '\0') {
			result += (*str - '0') * (scalar /= 10);
			++str;
		}
	}

	return result * final;
}

constexpr double parse_postfix(const char *str, size_t len, size_t count) 
{
	char token[len]{};
	double stack[count + 1]{};
	int pos{};

	while (*str != '\0') {
		str += next_token(str, token);
		
		if (is_number(token)) {
			stack[pos++] = str_to_double(token);
		} else {
			double right{ stack[--pos] }, left{ stack[--pos] };
			assert(pos >= 0);
			stack[pos++] = calculate(token[0], left, right);
		}
	}
	
	return stack[0];
}

constexpr int precedence(char op, char top)
{
	int result{};

	if (((op == '+' || op == '-') && (top == '*' || top == '/' || top == '^')) 
		|| ((op == '*' || op =='/') && top == '^')) {
		result = -1;
	} else if (((top == '+' || top == '-') && (op == '*' || op == '/' || op == '^')) 
		|| ((top == '*' || top =='/') && op == '^')) {
		result = 1;
	}

	return result;
}

constexpr size_t handle_operator(char op, char *stack, size_t pos, char **result)
{
	if (pos == 0) {
		stack[pos++] = op;
	} else if (op == '(' || stack[pos - 1] == '(' || precedence(op, stack[pos - 1]) == 1) {
		stack[pos++] = op;
	} else if (op == ')') {
		while (stack[pos - 1] != '(') {
			stack[pos] = '\0';
			*result += copy_token(&stack[--pos], *result); 
		}
		--pos;
	} else if (precedence(op, stack[pos - 1]) == 0) {
		if (op != '^') {
			stack[pos] = '\0';
			*result += copy_token(&stack[--pos], *result); 
		}
		stack[pos++] = op;
	} else {
		stack[pos] = '\0';
		*result += copy_token(&stack[--pos], *result); 
		pos = handle_operator(op, stack, pos, result);
	}

	return pos;
}

constexpr size_t parse_infix(const char *str, size_t len, char *result)
{
	char token[len]{}, stack[len]{};
	size_t operands{}, i{}, pos{};

	do {
		i = next_token(str, token);
		str += i;

		if (is_operator(token)) {
			pos = handle_operator(token[0], stack, pos, &result);
		} else {
			++operands;
			result += copy_token(token, result);
		}
	} while (i);

	while (pos) {
		stack[pos] = '\0';
		result += copy_token(&stack[--pos], result); 
	}

	*(result - 1) = '\0';
	return operands;
}

constexpr size_t length(const char *str) 
{
	size_t i{};

	for(; str[i]; ++i);

	return i;
}

constexpr double operator""_calc(const char *str, size_t len) 
{
	char parsed[len * 2]{};
	size_t operands{ parse_infix(str, len, parsed) };
	len = length(parsed);

	return parse_postfix(parsed, len, operands);
}

int main() 
{
	constexpr double result{ "2 ^ 3.11 / ( 5 + -0.9 ) * 10"_calc };
	std::cout << "2 ^ 3.11 / ( 5 + -0.9 ) * 10 = " << result << std::endl;
}
