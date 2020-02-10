#pragma once

#include "ra/operation.hpp"

namespace ra
{
	
	template <typename Relation, typename Predicate>
	class selection : public unary<Relation>
	{
		using output_type = input_type;
	public:
		static auto next()
		{
			auto row{ Relation::next() };

			while(!Predicate::eval(row))
			{
				row = Relation::next();
			}

			return row;
		}
	};

} // namespace ra
