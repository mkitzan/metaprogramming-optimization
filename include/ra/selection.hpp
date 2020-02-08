#pragma once

namespace ra
{
	
	template <typename Relation, typename... Attributes>
	struct selection
	{
		using output_type = sql::row<Attributes...>;
		using input_type = decltype(Relation::next());

		static constexpr auto next()
		{}
	};

} // namespace ra

