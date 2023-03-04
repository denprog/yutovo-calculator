#ifndef SCRIPT_H
#define SCRIPT_H

#include "ast.h"
#include "expression.h"
#include "definition.h"

using namespace std;

namespace yutovo_calculator
{
	/**
	 * Spirit grammar for main script.
	 */
	template<typename Number>
	struct Script : qi::grammar<std::u32string::iterator, ScriptNode<Number>(), unicode::space_type>
	{
		/**
		 * Constructor.
		 * @param [in] expr The expression.
		 */
		Script(std::u32string& expr) : Script::base_type(script), definition(expr), expression(expr)
		{
			script = 
				*(
					(definition >> ";") | 
					(expression >> ";")
				);
		}

		Definition<Number> definition;
    	Expression<Number> expression;
		qi::rule<std::u32string::iterator, ScriptNode<Number>(), unicode::space_type> script;
	};
};

#endif
