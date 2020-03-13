#pragma once

#include "ra/operation.hpp"

namespace ra
{

	template <typename Predicate, typename Input>
	class selection : public ra::unary<Input>
	{
		using input_type = typename ra::unary<Input>::input_type;
	public:
		using output_type = input_type;		

		static auto& next()
		{
			auto& output_row = Input::next();

			while(!Predicate::eval(output_row))
			{
				output_row = Input::next();
			}

			return output_row;
		}
	};

} // namespace ra
