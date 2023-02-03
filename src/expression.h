#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "stdafx.h"
#include "ast.h"
#include "error_handler.h"
#include "annotation.h"

namespace yutovo_calculator
{
	//Spirit grammar for expression.
	template<typename Number>
	struct Expression : qi::grammar<std::string::iterator, ExpressionNode<Number>(), qi::space_type>
	{
		Expression(std::string& expr);

		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number> >, 
			boost::recursive_wrapper<UnaryOperationNode<Number> >, 
			boost::recursive_wrapper<OperationNode<Number> >, 
			boost::recursive_wrapper<FunctionCallNode<Number> >, 
			boost::recursive_wrapper<ExpressionNode<Number> > >
			Operand;
		
		qi::rule<std::string::iterator, ExpressionNode<Number>(), qi::space_type> expression, term, addition, multiplication;
		qi::rule<std::string::iterator, Operand(), qi::space_type> unary;
		qi::rule<std::string::iterator, OperationNode<Number>(), qi::space_type> multiply;
		qi::rule<std::string::iterator, UnaryOperationNode<Number>(), qi::space_type> unary_operation;
		qi::rule<std::string::iterator, IdentifierNode<Number>(), qi::space_type> identifier;
		qi::rule<std::string::iterator, FunctionCallNode<Number>(), qi::space_type> function_call;
		qi::rule<std::string::iterator, std::string(), qi::space_type> str, exp_number;
		qi::rule<std::string::iterator, std::string(), qi::space_type> name;
		qi::rule<std::string::iterator, Number(), qi::space_type> number;
	};
};

#endif
