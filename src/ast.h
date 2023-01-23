#ifndef AST_H
#define AST_H

#include "stdafx.h"
#include "integer.h"
#include "real.h"
#include "rational.h"

namespace yutovo_calc
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
			boost::recursive_wrapper<IdentifierNode<Number> >, 
			boost::recursive_wrapper<UnaryOperationNode<Number> >, 
			boost::recursive_wrapper<OperationNode<Number> >, 
			boost::recursive_wrapper<FunctionCallNode<Number> >, 
			boost::recursive_wrapper<ExpressionNode<Number> > > 
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
			boost::recursive_wrapper<IdentifierNode<Number> >, 
			boost::recursive_wrapper<UnaryOperationNode<Number> >, 
			boost::recursive_wrapper<OperationNode<Number> >, 
			boost::recursive_wrapper<FunctionCallNode<Number> >, 
			boost::recursive_wrapper<ExpressionNode<Number> > > 
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
		std::list<IdentifierNode<Number> > arguments;
		ExpressionNode<Number> returnExpression;
	};

	//Definition node.
	template<typename Number>
	struct DefinitionNode
	{
		typedef boost::variant<
			boost::recursive_wrapper<VariableNode<Number> >, 
			boost::recursive_wrapper<FunctionNode<Number> > > 
			Definition;
		
		Definition definition;
	};
	
	//Function call node.
	template<typename Number>
	struct FunctionCallNode : ExpressionPosition
	{
		IdentifierNode<Number> name;
		std::list<ExpressionNode<Number> > arguments;
	};
	
	//Expression node.
	template<typename Number>
	struct ExpressionNode : ExpressionPosition
	{
		typedef Number value_type;
		typedef boost::variant<
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number> >, 
			boost::recursive_wrapper<UnaryOperationNode<Number> >, 
			boost::recursive_wrapper<OperationNode<Number> >,
			boost::recursive_wrapper<FunctionCallNode<Number> >, 
			boost::recursive_wrapper<ExpressionNode<Number> > >
			Operand;
		
		Operand first;
		std::list<OperationNode<Number> > rest;
	};
	
	//Script node.
	template<typename Number>
	struct ScriptNode
	{
		typedef Number value_type;
		typedef boost::variant<
			boost::recursive_wrapper<DefinitionNode<Number> >, 
			boost::recursive_wrapper<ExpressionNode<Number> > > 
			Operand;
		
		std::list<Operand> list;
	};
};

//Integer adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::UnaryOperationNode<yutovo_calc::Integer>, 
	(char, op)(yutovo_calc::UnaryOperationNode<yutovo_calc::Integer>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::OperationNode<yutovo_calc::Integer>, 
	(char, op)(yutovo_calc::OperationNode<yutovo_calc::Integer>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::VariableNode<yutovo_calc::Integer>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Integer>, name)(yutovo_calc::ExpressionNode<yutovo_calc::Integer>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionNode<yutovo_calc::Integer>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Integer>, name)
	(std::list<yutovo_calc::IdentifierNode<yutovo_calc::Integer> >, arguments)
	//(yutovo_calc::ExpressionNode<yutovo_calc::Integer>, expression)
	(yutovo_calc::ExpressionNode<yutovo_calc::Integer>, returnExpression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::DefinitionNode<yutovo_calc::Integer>, 
	(yutovo_calc::DefinitionNode<yutovo_calc::Integer>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::IdentifierNode<yutovo_calc::Integer>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionCallNode<yutovo_calc::Integer>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Integer>, name)
	(std::list<yutovo_calc::ExpressionNode<yutovo_calc::Integer> >, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ExpressionNode<yutovo_calc::Integer>, 
	(yutovo_calc::ExpressionNode<yutovo_calc::Integer>::Operand, first)(std::list<yutovo_calc::OperationNode<yutovo_calc::Integer> >, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ScriptNode<yutovo_calc::Integer>, 
	(std::list<yutovo_calc::ScriptNode<yutovo_calc::Integer>::Operand>, list))

//Real adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::UnaryOperationNode<yutovo_calc::Real>, 
	(char, op)(yutovo_calc::UnaryOperationNode<yutovo_calc::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::OperationNode<yutovo_calc::Real>, 
	(char, op)(yutovo_calc::OperationNode<yutovo_calc::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::VariableNode<yutovo_calc::Real>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Real>, name)(yutovo_calc::ExpressionNode<yutovo_calc::Real>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionNode<yutovo_calc::Real>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Real>, name)
	(std::list<yutovo_calc::IdentifierNode<yutovo_calc::Real> >, arguments)
	//(yutovo_calc::ExpressionNode<yutovo_calc::Real>, expression)
	(yutovo_calc::ExpressionNode<yutovo_calc::Real>, returnExpression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::DefinitionNode<yutovo_calc::Real>, 
	(yutovo_calc::DefinitionNode<yutovo_calc::Real>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::IdentifierNode<yutovo_calc::Real>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionCallNode<yutovo_calc::Real>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Real>, name)
	(std::list<yutovo_calc::ExpressionNode<yutovo_calc::Real> >, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ExpressionNode<yutovo_calc::Real>, 
	(yutovo_calc::ExpressionNode<yutovo_calc::Real>::Operand, first)(std::list<yutovo_calc::OperationNode<yutovo_calc::Real> >, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ScriptNode<yutovo_calc::Real>, 
	(std::list<yutovo_calc::ScriptNode<yutovo_calc::Real>::Operand>, list))

//Rational adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::UnaryOperationNode<yutovo_calc::Rational>, 
	(char, op)(yutovo_calc::UnaryOperationNode<yutovo_calc::Rational>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::OperationNode<yutovo_calc::Rational>, 
	(char, op)(yutovo_calc::OperationNode<yutovo_calc::Rational>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::VariableNode<yutovo_calc::Rational>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Rational>, name)(yutovo_calc::ExpressionNode<yutovo_calc::Rational>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionNode<yutovo_calc::Rational>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Rational>, name)
	(std::list<yutovo_calc::IdentifierNode<yutovo_calc::Rational> >, arguments)
	//(yutovo_calc::ExpressionNode<yutovo_calc::Rational>, expression)
	(yutovo_calc::ExpressionNode<yutovo_calc::Rational>, returnExpression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::DefinitionNode<yutovo_calc::Rational>, 
	(yutovo_calc::DefinitionNode<yutovo_calc::Rational>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::IdentifierNode<yutovo_calc::Rational>, 
	(std::string, name))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::FunctionCallNode<yutovo_calc::Rational>, 
	(yutovo_calc::IdentifierNode<yutovo_calc::Rational>, name)
	(std::list<yutovo_calc::ExpressionNode<yutovo_calc::Rational> >, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ExpressionNode<yutovo_calc::Rational>, 
	(yutovo_calc::ExpressionNode<yutovo_calc::Rational>::Operand, first)(std::list<yutovo_calc::OperationNode<yutovo_calc::Rational> >, rest))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calc::ScriptNode<yutovo_calc::Rational>, 
	(std::list<yutovo_calc::ScriptNode<yutovo_calc::Rational>::Operand>, list))

#endif
