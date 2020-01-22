#pragma once

// Template shim used to convert a value into a type
// Essentially the reverse of `decltype()`
template<auto Val>
struct VT {
	static constexpr auto val{ Val };
};
