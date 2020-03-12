#pragma once

#include <type_traits>

#include "ra/operation.hpp"
#include "ra/relation.hpp"

#include "sql/row.hpp"

namespace ra
{

	namespace
	{

		template <typename T>
		struct wrapper
		{
			using mutable_type = std::remove_cvref_t<T>; 

			wrapper() : value{}
			{}

			wrapper(T v) : value{ const_cast<std::remove_const_t<T>>(v) }
			{}

			wrapper<T>& operator=(wrapper<T> const& other)
			{
				value = other.value;
				return *this;
			}

			wrapper<T>& operator=(wrapper<T>&& other)
			{
				value = other.value;
				return *this;
			}

			mutable_type value;
		};

	} // namespace

	template <typename LeftInput, typename RightInput>
	class join : public ra::binary<LeftInput, RightInput>
	{
		using binary_type = ra::binary<LeftInput, RightInput>;
	public:
		using left_type = binary_type::left_type;
		using right_type = binary_type::right_type;

		template <typename... Inputs>
		static inline void seed(Inputs const&... rs)
		{
			binary_type::seed(rs...);
			left_row = LeftInput::next();
		}

		static inline void reset()
		{
			binary_type::reset();
			left_row = LeftInput::next();
		}

		static wrapper<left_type> left_row;
	};

	template <typename LeftInput, typename RightInput>
	wrapper<typename join<LeftInput, RightInput>::left_type> join<LeftInput, RightInput>::left_row{};

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
			right_type right_row{};

			try
			{
				sql::merge(dest_row, join_type::left_row.value, RightInput::next());
			}
			catch(ra::data_end const& e)
			{
				join_type::left_row = LeftInput::next();
				RightInput::reset();
				sql::merge(dest_row, join_type::left_row.value, RightInput::next());
			}

			return dest_row;
		}
	};

} // namespace ra
