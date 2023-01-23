#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "parser_exception.h"

namespace yutovo_calc
{
	template<typename Exception>
	struct ErrorHandler
	{
		ErrorHandler(std::string::iterator _first, std::string::iterator _last, ParserExceptionCode _id) : first(_first), last(_last)
		{
			id = _id;
		}
		
		void operator()(std::string::iterator p) const
		{
			int i = 0, j = 0;
			std::string::iterator k = first;
			
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
		std::string::iterator first;
		std::string::iterator last;
		ParserExceptionCode id;
	};
}

#endif
