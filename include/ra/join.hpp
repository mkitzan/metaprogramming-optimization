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
	public:
		using left_type = binary_type::left_type;
		using left_value = std::remove_cvref_t<left_type>;
		using left_ref = std::remove_const_t<left_type>;
		using right_type = binary_type::right_type;

		template <typename... Inputs>
		static inline void seed(Inputs const&... rs)
		{
			binary_type::seed(rs...);
			left_row = const_cast<left_ref>(LeftInput::next());
		}

		static inline void reset()
		{
			binary_type::reset();
			left_row = const_cast<left_ref>(LeftInput::next());
		}

		static left_value left_row;
	};

	template <typename LeftInput, typename RightInput>
	typename join<LeftInput, RightInput>::left_value join<LeftInput, RightInput>::left_row{};

	template <typename LeftInput, typename RightInput>
	class cross : public ra::join<LeftInput, RightInput>
	{
		using join_type = ra::join<LeftInput, RightInput>;
	public:
		using left_type = join_type::left_type;
		using right_type = join_type::right_type;
		using output_type = decltype(sql::link<std::remove_cvref_t<left_type>, std::remove_cvref_t<right_type>>());

		static auto next()
		{
			output_type dest_row{};

			try
			{
				sql::merge(dest_row, join_type::left_row, RightInput::next());
			}
			catch(ra::data_end const& e)
			{
				join_type::left_row = const_cast<join_type::left_ref>(LeftInput::next());
				RightInput::reset();
				sql::merge(dest_row, join_type::left_row, RightInput::next());
			}

			return dest_row;
		}
	};

} // namespace ra
