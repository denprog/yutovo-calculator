#ifndef AST_H
#define AST_H

#include "stdafx.h"
#include "integer.h"
#include "real.h"
#include "rational.h"
#include "utils.h"

namespace yutovo_calculator
{

namespace unicode = boost::spirit::unicode;

template<typename Number>
struct IdentifierNode;
template<typename Number>
struct UnaryOperationNode;
template<typename Number>
struct OperationNode;
template<typename Number>
struct PostfixOperationNode;
template<typename Number>
struct MixedDivivsionNode;
template<typename Number>
struct ImplicitStringMulNode;
template<typename Number>
struct ImplicitDivMulNode;
template<typename Number>
struct ImplicitMulDivNode;
template<typename Number>
struct VariableNode;
template<typename Number>
struct UnitNode;
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
	
	ElementId id;
	int pos; //position in a line
	int line; //line number
};

//Identifier node.
template<typename Number>
struct IdentifierNode : public ExpressionPosition
{
	std::u32string name;
	std::u32string subscript;
};

//Unary operation node.
template<typename Number>
struct UnaryOperationNode : public ExpressionPosition
{
	typedef boost::variant<
		Number, 
		boost::recursive_wrapper<IdentifierNode<Number>>, 
		boost::recursive_wrapper<UnaryOperationNode<Number>>, 
		boost::recursive_wrapper<OperationNode<Number>>, 
		boost::recursive_wrapper<PostfixOperationNode<Number>>, 
		boost::recursive_wrapper<MixedDivivsionNode<Number>>, 
		boost::recursive_wrapper<ImplicitStringMulNode<Number>>,
		boost::recursive_wrapper<ImplicitDivMulNode<Number>>,
		boost::recursive_wrapper<ImplicitMulDivNode<Number>>,
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
		boost::recursive_wrapper<PostfixOperationNode<Number>>, 
		boost::recursive_wrapper<MixedDivivsionNode<Number>>, 
		boost::recursive_wrapper<ImplicitStringMulNode<Number>>,
		boost::recursive_wrapper<ImplicitDivMulNode<Number>>,
		boost::recursive_wrapper<ImplicitMulDivNode<Number>>,
		boost::recursive_wrapper<FunctionCallNode<Number>>, 
		boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
		boost::recursive_wrapper<ExpressionNode<Number>>> 
		Operand;
	
	char op;
	Operand operand;
};

//Postfix operation node.
template<typename Number>
struct PostfixOperationNode : ExpressionPosition
{
	typedef boost::variant<
		Number, 
		boost::recursive_wrapper<IdentifierNode<Number>>, 
		boost::recursive_wrapper<UnaryOperationNode<Number>>, 
		boost::recursive_wrapper<OperationNode<Number>>, 
		boost::recursive_wrapper<PostfixOperationNode<Number>>, 
		boost::recursive_wrapper<MixedDivivsionNode<Number>>, 
		boost::recursive_wrapper<ImplicitStringMulNode<Number>>,
		boost::recursive_wrapper<ImplicitDivMulNode<Number>>,
		boost::recursive_wrapper<ImplicitMulDivNode<Number>>,
		boost::recursive_wrapper<FunctionCallNode<Number>>, 
		boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
		boost::recursive_wrapper<ExpressionNode<Number>>> 
		Operand;
	
	char op;
	Operand operand;
};

template<typename Number>
struct MixedDivivsionNode : ExpressionPosition
{
	Number left;
	Number numerator;
	Number denominator;
};

template<typename Number>
struct ImplicitStringMulNode : ExpressionPosition
{
	Number left;
	IdentifierNode<Number> identifier;
};

template<typename Number>
struct ImplicitDivMulNode : ExpressionPosition
{
	ExpressionNode<Number> upper;
	ExpressionNode<Number> lower;
	IdentifierNode<Number> identifier;
};

template<typename Number>
struct ImplicitMulDivNode : ExpressionPosition
{
	Number before;
	ExpressionNode<Number> upper;
	ExpressionNode<Number> lower;
};

template<typename Number>
struct VariableNode
{
	mutable ElementId id;
	IdentifierNode<Number> name;
	ExpressionNode<Number> expression;
};

template<typename Number>
struct FunctionNode
{
	mutable ElementId id;
	IdentifierNode<Number> name;
	std::list<IdentifierNode<Number>> arguments;
	ExpressionNode<Number> return_expression;
};

template<typename Number>
struct UnitNode : ExpressionPosition
{
	mutable ElementId id;
	IdentifierNode<Number> name;
	ExpressionNode<Number> expression;
};

template<typename Number>
struct DefinitionNode
{
	typedef boost::variant<
		boost::recursive_wrapper<VariableNode<Number>>, 
		boost::recursive_wrapper<UnitNode<Number>>, 
		boost::recursive_wrapper<FunctionNode<Number>>> 
		Definition;
	
	Definition definition;
};

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
		boost::recursive_wrapper<PostfixOperationNode<Number>>, 
		boost::recursive_wrapper<MixedDivivsionNode<Number>>,
		boost::recursive_wrapper<ImplicitStringMulNode<Number>>,
		boost::recursive_wrapper<ImplicitDivMulNode<Number>>,
		boost::recursive_wrapper<ImplicitMulDivNode<Number>>,
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

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Integer>::Operand, operand)(char, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnitNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Integer>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, 
	(std::u32string, name)
	(std::u32string, subscript))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionCallNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, name)
	(std::list<yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>>, arguments))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitStringMulNode<yutovo_calculator::Integer>,
	(yutovo_calculator::Integer, left)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, identifier))

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

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitDivMulNode<yutovo_calculator::Integer>,
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, lower)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Integer>, identifier))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitMulDivNode<yutovo_calculator::Integer>,
	(yutovo_calculator::Integer, before)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Integer>, lower))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::MixedDivivsionNode<yutovo_calculator::Integer>, 
	(yutovo_calculator::Integer, left)
	(yutovo_calculator::Integer, numerator)
	(yutovo_calculator::Integer, denominator))

//Real adaptors

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Real>, 
	(char, op)(yutovo_calculator::UnaryOperationNode<yutovo_calculator::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::OperationNode<yutovo_calculator::Real>, 
	(char, op)(yutovo_calculator::OperationNode<yutovo_calculator::Real>::Operand, operand))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Real>, 
	(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Real>::Operand, operand)(char, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::MixedDivivsionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::Real, left)
	(yutovo_calculator::Real, numerator)
	(yutovo_calculator::Real, denominator))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitStringMulNode<yutovo_calculator::Real>,
	(yutovo_calculator::Real, left)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, identifier))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitDivMulNode<yutovo_calculator::Real>,
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, lower)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, identifier))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitMulDivNode<yutovo_calculator::Real>,
	(yutovo_calculator::Real, before)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, lower))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnitNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Real>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Real>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Real>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Real>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Real>, 
	(std::u32string, name)
	(std::u32string, subscript))

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

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::PostfixOperationNode<yutovo_calculator::Rational>::Operand, operand)(char, op))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::MixedDivivsionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::Rational, left)
	(yutovo_calculator::Rational, numerator)
	(yutovo_calculator::Rational, denominator))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitStringMulNode<yutovo_calculator::Rational>,
	(yutovo_calculator::Rational, left)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, identifier))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitDivMulNode<yutovo_calculator::Rational>,
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, lower)
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, identifier))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::ImplicitMulDivNode<yutovo_calculator::Rational>,
	(yutovo_calculator::Rational, before)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, upper)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, lower))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::VariableNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::UnitNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::FunctionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, name)
	(std::list<yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>>, arguments)
	(yutovo_calculator::ExpressionNode<yutovo_calculator::Rational>, return_expression))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::DefinitionNode<yutovo_calculator::Rational>, 
	(yutovo_calculator::DefinitionNode<yutovo_calculator::Rational>::Definition, definition))

BOOST_FUSION_ADAPT_STRUCT(yutovo_calculator::IdentifierNode<yutovo_calculator::Rational>, 
	(std::u32string, name)
	(std::u32string, subscript))

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
