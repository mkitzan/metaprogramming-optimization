#pragma once

#include <type_traits>

#include "cexpr/string.hpp"
#include "sql/row.hpp"

namespace sql
{

	template <cexpr::string... Columns>
	struct index
	{
		template <typename Row>
		struct comp
		{
			//template <std::enable_if<std::is_same<Row, void>::value, Row>::type>
			bool operator()(Row const& left, Row const& right) const
			{
				return compare<Columns...>(left, right);
			}
		
		private:
			//template <cexpr::string Col, cexpr::string... Cols, std::enable_if<std::is_same<Row, void>::value, Row>::type>
			template <cexpr::string Col, cexpr::string... Cols>
			bool compare(Row const& left, Row const& right) const
			{
				auto const& l{ get<Col>(left) };
				auto const& r{ get<Col>(right) };

				if constexpr (sizeof...(Cols) != 0)
				{
					if (l == r)
					{
						return compare<Cols...>(left, right);
					}					
				}
				
				return l < r;
			}
		};
	};

} // namespace sql

