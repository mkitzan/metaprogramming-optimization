#pragma once

#include "templ_valtype.hpp"

template<auto Elem0, auto Elem1>
struct Pair {
	static constexpr auto e0{ Elem0 };
	static constexpr auto e1{ Elem1 };
};

template<typename Type0, Type0 V0, typename Type1, Type1 V1>
constexpr auto pair(VT<V0> v0, VT<V1> v1)
{
	return Pair<V0, V1>{};
}
