#pragma once

template<typename... T>
struct templ_stack{};

template<typename S, typename... T>
constexpr auto push(S, templ_stack<T...>) -> templ_stack<S, T...>;

template<typename S, typename... T>
constexpr auto pop(templ_stack<S, T...>) -> templ_stack<T...>;

template<typename S, typename... T>
constexpr auto top(templ_stack<S, T...>) -> S;

struct empty{};
constexpr auto top(templ_stack<>) -> empty;

template<typename OldStack, typename RevStack>
static constexpr auto reverse(OldStack old, RevStack rev)
{
	constexpr auto elem{ top(old) };

	if constexpr (elem == empty{}) {
		return rev;
	} else {
		return reverse(pop(old), push(elem, rev));
	}
}
