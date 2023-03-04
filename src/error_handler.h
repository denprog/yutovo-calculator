#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "parser_exception.h"

namespace yutovo_calculator
{
	template<typename Exception>
	struct ErrorHandler
	{
		ErrorHandler(std::u32string::iterator _first, std::u32string::iterator _last, ParserExceptionCode _id) : first(_first), last(_last)
		{
			id = _id;
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
			
			throw Exception(id, i, j);
		}

		template<typename>
		struct result
		{
			typedef void type;
		};

		typedef void result_type;
		std::u32string::iterator first;
		std::u32string::iterator last;
		ParserExceptionCode id;
	};
}

#endif
