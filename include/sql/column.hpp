#pragma once

#include "cexpr/string.hpp"

namespace sql
{

	template <cexpr::string N, typename T>
	struct column
	{
		static constexpr auto Name = N;
		using Type = T;
	};

} // namespace sql
