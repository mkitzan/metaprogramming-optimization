#pragma once

#include "templ_valtype.hpp"

template<auto E0, auto E1>
struct Pair {
	static constexpr auto first{ E0 };
	static constexpr auto second{ E1 };
};

template<typename Type0, Type0 V0, typename Type1, Type1 V1>
constexpr auto pair(VT<V0> v0, VT<V1> v1)
{
	return Pair<V0, V1>{};
}
