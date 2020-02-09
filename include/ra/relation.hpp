#pragma once

#include <exception>
#include <type_traits>

namespace ra
{

	template <typename Schema>
	struct relation
	{
		using output_type = Schema::row_type;

		static auto next()
		{
			if (curr != end)
			{
				return *curr++;
			}
			else
			{
				throw std::exception{};
			}
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

		static Schema::const_iterator curr;
		static Schema::const_iterator end;
	};

	template <typename Schema>
	Schema::const_iterator relation<Schema>::curr{};

	template <typename Schema>
	Schema::const_iterator relation<Schema>::end{};

} // namespace ra

