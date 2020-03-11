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
			fold<typename Output::column, typename Output::next>(dest, Input::next());
			
			return dest;
		}

	private:
		template <typename Column, typename Next>
		static inline constexpr void fold(output_type& dest, input_type const& src)
		{
			sql::set<Column::name>(dest, sql::get<Column::name>(src));

			if constexpr (Next::depth != 0)
			{
				fold<typename Next::column, typename Next::next>(dest, src);
			}
		}
	};

} // namespace ra
