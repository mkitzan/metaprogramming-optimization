#pragma once

#include <type_traits>

namespace ra
{

	template <typename Schema>
	struct relation
	{
		static auto next()
		{
			return *curr++;
		}
		
		template <typename Relation, typename... Relations>
		static void seed(Relation const& r, Relations... rs)
		{
			if constexpr (std::is_same<Relation, Schema>::value)
			{
				curr = r.begin();
				end = r.end();
			}
			else
			{
				seed(rs...);
			}			
		}

		static Schema::const_iterator curr{ nullptr };
		static Schema::const_iterator end{ nullptr };
	};

} // namespace ra

