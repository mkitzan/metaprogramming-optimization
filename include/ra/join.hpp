#pragma once

#include "ra/operation.hpp"
#include "ra/relation.hpp"

#include "sql/row.hpp"

namespace ra
{

	template <typename LeftInput, typename RightInput>
	class cross_join : public ra::binary<LeftInput, RightInput>
	{
	public:
		using left_type = ra::binary<LeftInput, RightInput>::left_type;
		using right_type = ra::binary<LeftInput, RightInput>::right_type;
		using output_type = decltype(sql::link<left_type, right_type>());

		static inline auto next()
		{
			static auto left_row{ LeftInput::next() };
			output_type dest_row{};
			right_type right_row{};

			try
			{
				right_row = RightInput::next();
			}
			catch(ra::data_end const& e)
			{
				left_row = LeftInput::next();
				RightInput::reset();
				right_row = RightInput::next();
			}

			sql::merge(dest_row, left_row, right_row);
			
			return dest_row;
		}
	};

} // namespace ra
