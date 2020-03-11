#pragma once

#include "ra/operation.hpp"

namespace ra
{
	
	template <typename Predicate, typename Input>
	class selection : public ra::unary<Input>
	{
	public:
		using input_type = ra::unary<Input>::input_type;
		using output_type = input_type;		

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
