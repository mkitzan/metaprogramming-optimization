#pragma once

#include "sql/row.hpp"

namespace ra
{

	template <typename Relation, typename... Attributes>
	struct projection
	{
		using output_type = sql::row<Attributes...>;
		using input_type = decltype(Relation::next());

		static constexpr auto next()
		{
			return fold(output_type{}, Relation::next());
		}

	private:
		static inline constexpr bool last()
		{
			return sizeof...(Attributes) == 0;
		}

		template <typename Attr, typename... Attrs>
		static constexpr auto fold(output_type&& dest, input_type&& src)
		{
			dest.set(sql::get<Attr>(src));

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
