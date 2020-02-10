#pragma once

namespace ra
{

	template <typename Relation>
	class operation
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

} // namespace ra
