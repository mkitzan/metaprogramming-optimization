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
			output_type dest{};
			input_type src{ Relation::next() };
			fold<Attributes...>(dest, src);
			
			return dest;
		}

		template <typename... Relations>
		static void seed(Relations const&... rs)
		{
			Relation::seed(rs...);
		}

	private:
		template <typename Attr, typename... Attrs>
		static constexpr void fold(output_type& dest, input_type& src)
		{
			sql::set<Attr::name>(dest, sql::get<Attr::name>(src));

			if constexpr (sizeof...(Attrs) != 0)
			{
				fold<Attrs...>(dest, src);
			}
		}
	};

} // namespace ra
