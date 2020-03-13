#pragma once

#include <type_traits>

#include "ra/operation.hpp"
#include "ra/relation.hpp"

#include "sql/row.hpp"

namespace ra
{

	template <typename LeftInput, typename RightInput>
	class join : public ra::binary<LeftInput, RightInput>
	{
		using binary_type = ra::binary<LeftInput, RightInput>;
		using left_type = typename binary_type::left_type;
		using right_type = typename binary_type::right_type;
	public:
		using output_type = decltype(sql::merge<left_type, right_type>());

		template <typename... Inputs>
		static inline void seed(Inputs const&... rs)
		{
			binary_type::seed(rs...);
			sql::copy(output_row, LeftInput::next());
		}

		static inline void reset()
		{
			binary_type::reset();
			sql::copy(output_row, LeftInput::next());
		}

		static output_type output_row;
	};

	template <typename LeftInput, typename RightInput>
	typename join<LeftInput, RightInput>::output_type join<LeftInput, RightInput>::output_row{};

	template <typename LeftInput, typename RightInput>
	class cross : public ra::join<LeftInput, RightInput>
	{
		using join_type = ra::join<LeftInput, RightInput>;
	public:
		using output_type = join_type::output_type;

		static auto& next()
		{
			try
			{
				sql::copy(join_type::output_row, RightInput::next());
			}
			catch(ra::data_end const& e)
			{
				sql::copy(join_type::output_row, LeftInput::next());
				RightInput::reset();
				sql::copy(join_type::output_row, RightInput::next());
			}

			return join_type::output_row;
		}
	};

} // namespace ra
