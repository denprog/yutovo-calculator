#ifndef DEFINITION_H
#define DEFINITION_H

#include "ast.h"
#include "expression.h"

namespace yutovo_calculator
{

//Spirit grammar for definitions
template<typename Number>
struct Definition : qi::grammar<std::u32string::iterator, DefinitionNode<Number>(), unicode::space_type>
{
	Definition(ElementId id, std::u32string& expr, Solver<Number>* solver) : 
		Definition::base_type(definition), 
		expression(id, expr, solver), 
		return_expression(id, expr, solver)
	{
		using unicode::char_;
		using boost::spirit::qi::lit;
		using boost::spirit::qi::raw;
		using boost::spirit::qi::lexeme;
		using unicode::alnum;
		using unicode::alpha;
		using boost::spirit::qi::on_error;
		using boost::spirit::qi::fail;
		qi::_1_type _1;
		qi::_3_type _3;
		
		//definition is a variable or a function or a unit
		definition = variable | unit | function;
		
		//function definition
		function = identifier >> '(' >> argument_list >> ')' >> ('=' > expression);
		
		//function's argument list
		argument_list = -(identifier % ',');
		
		//variable definition
		variable = identifier >> ('=' > expression);

		//unit definition
		unit = identifier >> ('~' > expression);

		//identifier is a name with on optional subscript
		identifier = name >> -('{' > (+char_("0-9") | name) > '}');
		
		//name is a letter-numeric std::u32string with an letter in the beginning
		name = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

		// BOOST_SPIRIT_DEBUG_NODE(definition);
		// BOOST_SPIRIT_DEBUG_NODE(function);
		// BOOST_SPIRIT_DEBUG_NODE(variable);
		// BOOST_SPIRIT_DEBUG_NODE(unit);
		// BOOST_SPIRIT_DEBUG_NODE(name);
		// BOOST_SPIRIT_DEBUG_NODE(identifier);
		// BOOST_SPIRIT_DEBUG_NODE(argument_list);

		//work out the exceptions
		on_error<fail>(definition, 
			boost::phoenix::function<ErrorHandler<SyntaxException>>(ErrorHandler<SyntaxException>(id, expr.begin(), expr.end(), SyntaxError))(_3));
	}

	qi::rule<std::u32string::iterator, DefinitionNode<Number>(), unicode::space_type> definition;
	qi::rule<std::u32string::iterator, FunctionNode<Number>(), unicode::space_type> function;
	qi::rule<std::u32string::iterator, VariableNode<Number>(), unicode::space_type> variable;
	qi::rule<std::u32string::iterator, UnitNode<Number>(), unicode::space_type> unit;
	qi::rule<std::u32string::iterator, std::u32string(), unicode::space_type> name;
	qi::rule<std::u32string::iterator, IdentifierNode<Number>(), unicode::space_type> identifier;
	qi::rule<std::u32string::iterator, std::list<IdentifierNode<Number> >(), unicode::space_type> argument_list;
	
	Expression<Number> expression;
	Expression<Number> return_expression;
};

};

#endif
