#pragma once

namespace ra
{

	template <typename Input>
	class unary
	{
	public:
		using input_type = decltype(Input::next());

		template <typename... Inputs>
		static void seed(Inputs const&... rs)
		{
			Input::seed(rs...);
		}

		static void reset()
		{
			Input::reset();
		}
	};

	template <typename LeftInput, typename RightInput>
	class binary
	{
	public:
		using left_type = decltype(LeftInput::next());
		using right_type = decltype(RightInput::next());

		template <typename... Inputs>
		static void seed(Inputs const&... rs)
		{
			LeftInput::seed(rs...);
			RightInput::seed(rs...);
		}

		static void reset()
		{
			LeftInput::reset();
			RightInput::reset();
		}
	};

} // namespace ra
