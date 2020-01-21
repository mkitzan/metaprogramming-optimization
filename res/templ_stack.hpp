#pragma once

template<typename... T>
class templ_stack{};

template<typename... T, typename... S>
constexpr auto push(templ_stack<T...>, S...) -> templ_stack<S..., T...>;

template<typename S, typename... T>
constexpr auto pop(templ_stack<S, T...>) -> templ_stack<T...>;

template<typename S, typename... T>
constexpr auto top(templ_stack<S, T...>) -> S;

class empty{};
constexpr auto top(templ_stack<>) -> empty;
