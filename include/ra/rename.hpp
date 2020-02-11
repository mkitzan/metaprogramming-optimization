#pragma once

#include "ra/operation.hpp"
#include "sql/row.hpp"

namespace ra
{

	template <typename Output, typename Input>
	class rename : public unary<Input>
	{
		using output_type = Output;
	public:
		static inline auto next()
		{
			output_type dest{};
			fold<typename Output::column, typename Output::next>(dest, Input::next());
			
			return dest;
		}

	private:
		template <typename Column, typename Next>
		static inline constexpr void fold(output_type& dest, unary<Input>::input_type const& src)
		{
			sql::set<Column::name>(dest, sql::get<Output::depth - (Next::depth + 1)>(src));

			if constexpr (Next::depth != 0)
			{
				fold<typename Next::column, typename Next::next>(dest, src);
			}
		}
	};
	
} // namespace ra
