#ifndef DEFINITION_H
#define DEFINITION_H

#include "ast.h"
#include "expression.h"

namespace yutovo_calculator
{
	/**
	 * Spirit grammar for definitions.
	 */
	template<typename Number>
	struct Definition : qi::grammar<std::string::iterator, DefinitionNode<Number>(), qi::space_type>
	{
		/**
		 * Constructor.
		 * @param [in] expr The expression.
		 */
		Definition(std::string& expr) : Definition::base_type(definition), expression(expr), return_expression(expr)
		{
			using boost::spirit::qi::lit;
			using boost::spirit::qi::raw;
			using boost::spirit::qi::lexeme;
			using boost::spirit::qi::alnum;
			using boost::spirit::qi::alpha;
			using boost::spirit::qi::on_error;
			using boost::spirit::qi::fail;
			
			//the definition is a variable or a function
			definition = 
				variable | 
				function;
			
			//the function definition
			function = 
				identifier >> 
				'(' >> argument_list >> ')' >> 
				(lit('=') >  
				'{' > "return" > return_expression > ';' > '}');
			
			//the function's argument list
			argument_list = 
				-(identifier % ',');
			
			//the variable definition
			variable = 
				identifier >> 
				('=' > expression);
			
			//identifier is a name
			identifier = 
				name;
			
			//name is a letter-numeric std::string with an letter in the beginning
			name = 
				raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

			//BOOST_SPIRIT_DEBUG_NODE(definition);
			//BOOST_SPIRIT_DEBUG_NODE(argument_list);
			//BOOST_SPIRIT_DEBUG_NODE(identifier);
			//BOOST_SPIRIT_DEBUG_NODE(name);
		}

		qi::rule<std::string::iterator, DefinitionNode<Number>(), qi::space_type> definition;
		qi::rule<std::string::iterator, FunctionNode<Number>(), qi::space_type> function;
		qi::rule<std::string::iterator, VariableNode<Number>(), qi::space_type> variable;
		qi::rule<std::string::iterator, std::string(), qi::space_type> name;
		qi::rule<std::string::iterator, IdentifierNode<Number>(), qi::space_type> identifier;
		qi::rule<std::string::iterator, std::list<IdentifierNode<Number> >(), qi::space_type> argument_list;
		//qi::rule<std::string::iterator, ExpressionNode<Number>(), qi::space_type> body;
		Expression<Number> expression;
		Expression<Number> return_expression;
	};
};

#endif
