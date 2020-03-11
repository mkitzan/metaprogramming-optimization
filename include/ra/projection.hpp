#pragma once

#include "ra/operation.hpp"

#include "sql/row.hpp"

namespace ra
{

	template <typename Output, typename Input>
	class projection : public ra::unary<Input>
	{
	public:
		using input_type =  ra::unary<Input>::input_type;
		using output_type = Output;

		static inline auto next()
		{
			output_type dest{};
			fold<output_type>(dest, Input::next());
			
			return dest;
		}

	private:
		template <typename Dest>
		static inline constexpr void fold(Dest& dest, input_type const& src)
		{
			if constexpr (Dest::depth == 0)
			{
				return;
			}
			else
			{
				dest.head() = sql::get<Dest::column::name>(src);
				fold<typename Dest::next>(dest.tail(), src);	
			}
		}
	};

} // namespace ra
