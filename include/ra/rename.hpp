#pragma once

#include "ra/operation.hpp"

#include "sql/row.hpp"

namespace ra
{

	template <typename Output, typename Input>
	class rename : public ra::unary<Input>
	{
	public:
		using input_type = ra::unary<Input>::input_type;
		using output_type = Output;

		static inline auto next()
		{
			output_type dest{};
			fold<output_type, input_type>(dest, Input::next());
			
			return dest;
		}

	private:
		template <typename Dest, typename Src>
		static inline constexpr void fold(Dest& dest, Src const& src)
		{
			if constexpr (Dest::depth == 0)
			{
				return;
			}
			else
			{
				dest.head() = src.head();
				fold<typename Dest::next, typename Src::next>(dest.tail(), src.tail());
			}
		}
	};
	
} // namespace ra
