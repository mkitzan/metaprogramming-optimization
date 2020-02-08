#pragma once

#include "sql/row.hpp"

namespace ra
{

	template <typename Relation, typename... Attributes>
	struct projection
	{
		using input_type = decltype(Relation::next());
		//using output_type = sql::row<decltype(sql::col<Attributes>(input_type{}))...>;
		using output_type = sql::row<Attributes...>;

		static constexpr auto next()
		{
			return fold(output_type{}, Relation::next());
		}

		template <typename... Relations>
		static void seed(Relations... rs)
		{
			Relation::seed(rs...);
		}

	private:
		static inline constexpr bool last()
		{
			return sizeof...(Attributes) == 0;
		}

		template <typename Attr, typename... Attrs>
		static constexpr auto fold(output_type&& dest, input_type&& src)
		{
			dest.set(sql::get<Attr::name>(src));

			if constexpr (!last())
			{
				return fold<Attrs...>(dest, src);
			}
			else
			{
				return dest;
			}
		}
	};

} // namespace ra
