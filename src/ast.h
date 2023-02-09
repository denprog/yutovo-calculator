#ifndef AST_H
#define AST_H

#include "stdafx.h"
#include "integer.h"
#include "real.h"
#include "rational.h"

namespace yutovo_calculator
{
	template<typename Number>
	struct IdentifierNode;
	template<typename Number>
	struct UnaryOperationNode;
	template<typename Number>
	struct OperationNode;
	template<typename Number>
	struct VariableNode;
	template<typename Number>
	struct FunctionCallNode;
	template<typename Number>
	struct NoFencesFunctionCallNode;
	template<typename Number>
	struct ExpressionNode;

	//Expression position.
	struct ExpressionPosition
	{
		ExpressionPosition()
		{
			pos = -1;
			line = -1;
		}
		
		int pos; //position in a line
		int line; //line number
	};
	
	//Identifier node.
	template<typename Number>
	struct IdentifierNode : public ExpressionPosition
	{
		std::string name;
	};

	//Unary operation node.
	template<typename Number>
	struct UnaryOperationNode
	{
		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>, 
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>> 
			Operand;
		
		char op;
		Operand operand;
	};
	
	//Operation node.
	template<typename Number>
	struct OperationNode : ExpressionPosition
	{
		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>, 
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>> 
			Operand;
		
		char op;
		Operand operand;
	};
	
	//Variable node.
	template<typename Number>
	struct VariableNode
	{
		IdentifierNode<Number> name;
		ExpressionNode<Number> expression;
	};
	
	//Function node.
	template<typename Number>
	struct FunctionNode
	{
		IdentifierNode<Number> name;
		std::list<IdentifierNode<Number>> arguments;
		ExpressionNode<Number> return_expression;
	};

	//Definition node.
	template<typename Number>
	struct DefinitionNode
	{
		typedef boost::variant<
			boost::recursive_wrapper<VariableNode<Number>>, 
			boost::recursive_wrapper<FunctionNode<Number>>> 
			Definition;
		
		Definition definition;
	};
	
	//Function call node.
	template<typename Number>
	struct FunctionCallNode : ExpressionPosition
	{
		IdentifierNode<Number> name;
		std::list<ExpressionNode<Number>> arguments;
	};

	template<typename Number>
	struct FunctionParamNode : ExpressionPosition
	{
		typedef Number value_type;
		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>>
			Operand;
		
		Operand op;
	};

	template<typename Number>
	struct NoFencesFunctionCallNode : ExpressionPosition
	{
		IdentifierNode<Number> name;
		std::list<ExpressionNode<Number>> arguments;
		FunctionParamNode<Number> last_argument;
	};

	template<typename Number>
	struct ExpressionNode : ExpressionPosition
	{
		typedef Number value_type;
		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>,
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>>
			Operand;
		
		Operand first;
		std::list<OperationNode<Number>> rest;
	};
	
	//Script node.
	template<typename Number>
	struct ScriptNode
	{
		typedef Number value_type;
		typedef boost::variant<
			boost::recursive_wrapper<DefinitionNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>> 
			Operand;
		
		std::list<Operand> list;
	};
};

//Integer adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Integer>, 
	(char, op)(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Integer>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::OperationNode<yutovo_calculator::Integer>, 
	(char, op)(yutovo_calculator::OperationNode<yutovo_calculator::Integer>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Integer>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionCallNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>>, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::NoFencesFunctionCallNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>>, arguments)
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Integer>, last_argument))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionParamNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Integer>::Operand, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>::Operand, first)
	(std::list<yutovo_calculator::OperationNode<yutovo_calculator::Integer>>, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ScriptNode<yutovo_calculator::Integer>, 
	(std::list<yutovo_calculator::ScriptNode<yutovo_calculator::Integer>::Operand>, list))

//Real adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Real>, 
	(char, op)(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::OperationNode<yutovo_calculator::Real>, 
	(char, op)(yutovo_calculator::OperationNode<yutovo_calculator::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Real>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Real>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionCallNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Real>>, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::NoFencesFunctionCallNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Real>>, arguments)
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Real>, last_argument))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionParamNode<yutovo_calculator::Real>, 
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Real>::Operand, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>::Operand, first)
	(std::list<yutovo_calculator::OperationNode<yutovo_calculator::Real>>, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ScriptNode<yutovo_calculator::Real>, 
	(std::list<yutovo_calculator::ScriptNode<yutovo_calculator::Real>::Operand>, list))

//Rational adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Rational>, 
	(char, op)(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Rational>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::OperationNode<yutovo_calculator::Rational>, 
	(char, op)(yutovo_calculator::OperationNode<yutovo_calculator::Rational>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Rational>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionCallNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>>, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::NoFencesFunctionCallNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>>, arguments)
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Rational>, last_argument))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionParamNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::FunctionParamNode<yutovo_calculator::Rational>::Operand, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>::Operand, first)
	(std::list<yutovo_calculator::OperationNode<yutovo_calculator::Rational>>, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ScriptNode<yutovo_calculator::Rational>, 
	(std::list<yutovo_calculator::ScriptNode<yutovo_calculator::Rational>::Operand>, list))

#endif
