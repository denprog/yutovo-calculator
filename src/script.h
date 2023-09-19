#ifndef SCRIPT_H
#define SCRIPT_H

#include "ast.h"
#include "expression.h"
#include "definition.h"
#include "error_handler.h"

using namespace std;

namespace yutovo_calculator
{

//Spirit grammar for main script
template<typename Number>
struct Script : qi::grammar<std::u32string::iterator, ScriptNode<Number>(), unicode::space_type>
{
	Script(ElementId id, std::u32string& expr, Solver<Number>* solver) : 
		Script::base_type(script), 
		definition(id, expr, solver), 
		expression(id, expr, solver)
	{
		using boost::spirit::qi::on_error;
		using boost::spirit::qi::fail;
		qi::_3_type _3;

		script = 
			*(
				(definition > ";") | 
				(expression > ";")
			);

		on_error<fail>(script, 
			boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
	}

	Definition<Number> definition;
	Expression<Number> expression;
	qi::rule<std::u32string::iterator, ScriptNode<Number>(), unicode::space_type> script;
};

};

#endif
