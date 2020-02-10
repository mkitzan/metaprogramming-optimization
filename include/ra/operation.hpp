#pragma once

namespace ra
{

	template <typename Relation>
	class unary
	{
	public:
		using input_type = decltype(Relation::next());

		template <typename... Relations>
		static void seed(Relations const&... rs)
		{
			Relation::seed(rs...);
		}

		static void reset()
		{
			Relation::reset();
		}
	};

	template <typename LeftRelation, typename RightRelation>
	class binary
	{
	public:
		using left_type = decltype(LeftRelation::next());
		using right_type = decltype(RightRelation::next());

		template <typename... Relations>
		static void seed(Relations const&... rs)
		{
			LeftRelation::seed(rs...);
			RightRelation::seed(rs...);
		}

		static void reset()
		{
			LeftRelation::reset();
			RightRelation::reset();
		}
	};

} // namespace ra
