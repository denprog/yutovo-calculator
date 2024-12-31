#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "parser_exception.h"

namespace yutovo_calculator
{
	template<typename Exception>
	struct ErrorHandler
	{
		ErrorHandler(LogicalId _id, std::u32string::iterator _first, std::u32string::iterator _last, ParserExceptionCode _ex_id) : 
			id(_id),
			first(_first), 
			last(_last),
			ex_id(_ex_id)
		{
		}
		
		void operator()(std::u32string::iterator p) const
		{
			int i = 0, j = 0;
			std::u32string::iterator k = first;
			
			while (k != p)
			{
				char ch = *k;
				if (ch == '\r')
				{
					++k;
					if (ch == '\n')
						++k;
					++j;
				}
				
				++k;
				++i;
			}
			
			throw Exception(id, ex_id, i, j);
		}

		template<typename>
		struct result
		{
			typedef void type;
		};

		typedef void result_type;
		std::u32string::iterator first;
		std::u32string::iterator last;
		LogicalId id;
		ParserExceptionCode ex_id;
	};
}

#endif
