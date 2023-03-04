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
	struct Expression : qi::grammar<std::u32string::iterator, ExpressionNode<Number>(), unicode::space_type>
	{
		Expression(std::u32string& expr);

		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>, 
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>>
			Operand;
		
		qi::rule<std::u32string::iterator, ExpressionNode<Number>(), unicode::space_type> expression, term, addition, multiplication;
		qi::rule<std::u32string::iterator, Operand(), unicode::space_type> unary;
		qi::rule<std::u32string::iterator, OperationNode<Number>(), unicode::space_type> multiply;
		qi::rule<std::u32string::iterator, UnaryOperationNode<Number>(), unicode::space_type> unary_operation;
		qi::rule<std::u32string::iterator, IdentifierNode<Number>(), unicode::space_type> identifier;
		qi::rule<std::u32string::iterator, FunctionCallNode<Number>(), unicode::space_type> function_call;
		qi::rule<std::u32string::iterator, NoFencesFunctionCallNode<Number>(), unicode::space_type> no_fences_function_call;
		qi::rule<std::u32string::iterator, FunctionParamNode<Number>(), unicode::space_type> function_param;
		qi::rule<std::u32string::iterator, std::u32string(), unicode::space_type> str, exp_number;
		qi::rule<std::u32string::iterator, std::u32string(), unicode::space_type> name;
		qi::rule<std::u32string::iterator, Number(), unicode::space_type> number;
	};
};

#endif
