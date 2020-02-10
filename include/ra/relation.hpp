#pragma once

#include <exception>
#include <type_traits>

namespace ra
{

	template <typename Schema>
	class relation
	{
	public:
		static auto const& next()
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
		
		template <typename Input, typename... Inputs>
		static void seed(Input const& r, Inputs... rs)
		{
			if constexpr (std::is_same<Input, Schema>::value)
			{
				curr = r.begin();
				begin = r.begin();
				end = r.end();
			}
			else
			{
				seed(rs...);
			}
		}

		static void reset()
		{
			curr = begin;
		}

	private:
		static Schema::const_iterator curr;
		static Schema::const_iterator begin;
		static Schema::const_iterator end;
	};

	template <typename Schema>
	Schema::const_iterator relation<Schema>::curr{};

	template <typename Schema>
	Schema::const_iterator relation<Schema>::begin{};

	template <typename Schema>
	Schema::const_iterator relation<Schema>::end{};

} // namespace ra
