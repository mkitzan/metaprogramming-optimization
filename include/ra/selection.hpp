#pragma once

#include "ra/operation.hpp"

namespace ra
{
	
	template <typename Predicate, typename Input>
	class selection : public unary<Input>
	{
	public:
		using output_type = unary<Input>::input_type;		

		static auto next()
		{
			auto row{ Input::next() };

			while(!Predicate::eval(row))
			{
				row = Input::next();
			}

			return row;
		}
	};

} // namespace ra
