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
	};

} // namespace ra
