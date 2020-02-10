#pragma once

#include "ra/operation.hpp"
#include "sql/row.hpp"

namespace ra
{

	template <typename Relation, typename... Attributes>
	class projection : public unary<Relation>
	{
		//using output_type = sql::row<decltype(sql::col<Attributes>(input_type{}))...>;
		using output_type = sql::row<Attributes...>;
	public:
		static auto next()
		{
			output_type dest{};
			fold<Attributes...>(dest, Relation::next());
			
			return dest;
		}

	private:
		template <typename Attr, typename... Attrs>
		static constexpr void fold(output_type& dest, unary<Relation>::input_type const& src)
		{
			sql::set<Attr::name>(dest, sql::get<Attr::name>(src));

			if constexpr (sizeof...(Attrs) != 0)
			{
				fold<Attrs...>(dest, src);
			}
		}
	};

} // namespace ra
