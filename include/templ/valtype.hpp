#pragma once

namespace templ
{

	// Template shim used to convert a value into a type
	template <auto Val>
	struct VT
	{
		static constexpr auto val{ Val };
	};

} // namespace templ
