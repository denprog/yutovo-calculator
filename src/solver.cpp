#include "stdafx.h"
#include "solver.h"

namespace yutovo_calculator
{

//Solver

template<>
Integer Solver<Integer>::operator()(FunctionCallNode<Integer> const& op) const
{
	AddDependency(op.name.name);

	Integer res;
	
	//find in the user defined functions
	FunctionNode<Integer>* user_func = FindFunction(op);
	if (user_func)
	{
		IdentifierNodesIter funcIter = user_func->arguments.begin();
		for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
		{
			Integer arg = (*this)(*iter);
			PushTempVariable(funcIter->name, arg);
		}

		res = (*this)(user_func->return_expression);
		PopTempVariable(op.arguments.size());
		return res;
	}

	//find in the build-in functions		
	BuildinFunction* func = FindBuildinFunction(op.name.name);
	if (func)
	{
		try
		{
			UnaryFunction u = boost::get<UnaryFunction>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Integer arg = (*this)(*iter);
			return (*u)(arg);
		}
		catch (boost::bad_get)
		{
		}
		
		try
		{
			BinaryFunction b = boost::get<BinaryFunction>(*func);
			if (op.arguments.size() != 2)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Integer arg1 = (*this)(*iter++);
			Integer arg2 = (*this)(*iter);
			return (*b)(arg1, arg2);
		}
		catch (boost::bad_get)
		{
		}
	}

	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
	
	return res;
}

template<>
Real Solver<Real>::operator()(FunctionCallNode<Real> const& op) const
{
	AddDependency(op.name.name);

	Real res;
	
	//find in the user defined functions
	FunctionNode<Real>* user_func = FindFunction(op);
	if (user_func)
	{
		IdentifierNodesIter funcIter = user_func->arguments.begin();
		for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
		{
			Real arg = (*this)(*iter);
			PushTempVariable(funcIter->name, arg);
		}

		res = (*this)(user_func->return_expression);
		PopTempVariable(op.arguments.size());
		return res;
	}
	
	//find in the build-in functions		
	BuildinFunction* func = FindBuildinFunction(op.name.name);
	if (func)
	{
		try
		{
			UnaryFunction u = boost::get<UnaryFunction>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Real arg = (*this)(*iter);
			if (arg.angle_measure == AngleMeasure::None)
				arg.angle_measure = default_angle_measure;
			return (*u)(arg);
		}
		catch (boost::bad_get)
		{
		}
		
		try
		{
			BinaryFunction b = boost::get<BinaryFunction>(*func);
			if (op.arguments.size() != 2)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Real arg1 = (*this)(*iter++);
			Real arg2 = (*this)(*iter);
			return (*b)(arg1, arg2);
		}
		catch (boost::bad_get)
		{
		}
	}

	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
	
	return res;
}

template<>
Rational Solver<Rational>::operator()(FunctionCallNode<Rational> const& op) const
{
	AddDependency(op.name.name);

	Rational res;
	
	//find in the user defined functions
	FunctionNode<Rational>* user_func = FindFunction(op);
	if (user_func)
	{
		IdentifierNodesIter funcIter = user_func->arguments.begin();
		for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
		{
			Rational arg = (*this)(*iter);
			PushTempVariable(funcIter->name, arg);
		}

		res = (*this)(user_func->return_expression);
		PopTempVariable(op.arguments.size());
		return res;
	}

	//find in the build-in functions		
	BuildinFunction* func = FindBuildinFunction(op.name.name);
	if (func)
	{
		try
		{
			UnaryFunction u = boost::get<UnaryFunction>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Rational arg = (*this)(*iter);
			return (*u)(arg);
		}
		catch (boost::bad_get)
		{
		}
		
		try
		{
			BinaryFunction b = boost::get<BinaryFunction>(*func);
			if (op.arguments.size() != 2)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Rational arg1 = (*this)(*iter++);
			Rational arg2 = (*this)(*iter);
			return (*b)(arg1, arg2);
		}
		catch (boost::bad_get)
		{
		}
	}

	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name.name + U"' not found", op.pos, op.line);
	
	return res;
}

template<>
Integer Solver<Integer>::operator()(NoFencesFunctionCallNode<Integer> const& op) const
{
	AddDependency(op.name.name);

	Integer res;
	return res;
}

template<>
Real Solver<Real>::operator()(NoFencesFunctionCallNode<Real> const& op) const
{
	AddDependency(op.name.name);

	Real res;
	
	//find in the build-in functions		
	BuildinFunction* func = FindBuildinFunction(op.name.name);
	if (func)
	{
		try
		{
			BinaryFunction b = boost::get<BinaryFunction>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Real arg1 = (*this)(*iter++);
			Real arg2 = (*this)(op.last_argument);
			return (*b)(arg1, arg2);
		}
		catch (boost::bad_get)
		{
		}
	}

	//there is no such a function
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name.name + U"' not found", op.pos, op.line);
	
	return res;
}

template<>
Rational Solver<Rational>::operator()(NoFencesFunctionCallNode<Rational> const& op) const
{
	AddDependency(op.name.name);

	Rational res;
	return res;
}

template<>
Integer Solver<Integer>::operator()(IdentifierNode<Integer> const& op) const
{
	AddDependency(op.name);

	TempVariable* t = FindTempVariable(op.name);
	if (t)
		return t->second;
	
	//find in user defined variables
	VariableNode<Integer>* v = FindVariable(op.name);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Integer res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	BuildinVariable* var = FindBuildinVariable(op.name);
	if (var)
	{
		try
		{
			IntegerVariable v = boost::get<Variable>(*var);
			return (*v)();
		}
		catch (boost::bad_get)
		{
		}
	}
	
	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.line);
	
	return 0;
}

template<>
Real Solver<Real>::operator()(IdentifierNode<Real> const& op) const
{
	AddDependency(op.name);

	TempVariable* t = FindTempVariable(op.name);
	if (t)
		return t->second;
	
	//find in the user defined variables
	VariableNode<Real>* v = FindVariable(op.name);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Real res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	//find in the build-in variables
	BuildinVariable* var = FindBuildinVariable(op.name);
	if (var)
	{
		try
		{
			RealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
			return (*v)(precision);
		}
		catch (boost::bad_get)
		{
		}
	}
	
	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.line);
	
	return Real();
}

template<>
Rational Solver<Rational>::operator()(IdentifierNode<Rational> const& op) const
{
	AddDependency(op.name);

	TempVariable* t = FindTempVariable(op.name);
	if (t)
		return t->second;
	
	//find in user defined variables
	VariableNode<Rational>* v = FindVariable(op.name);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Rational res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	BuildinVariable* var = FindBuildinVariable(op.name);
	if (var)
	{
		try
		{
			RationalVariable v = boost::get<Variable>(*var);
			return (*v)();
		}
		catch (boost::bad_get)
		{
		}
	}
	
	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.line);
	
	return 0;
}

template<>
Real Solver<Real>::operator()(ScriptNode<Real> const& script, ElementId _id, AngleMeasure _default_angle_measure, 
	AngleMeasure _result_angle_measure, int _precision) const
{
	if (script.list.empty())
		throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected);
	
	id = _id;

	default_angle_measure = _default_angle_measure;
	result_angle_measure = _result_angle_measure;
	precision = _precision;
	
	Real res;
	//calculate all the script nodes
	BOOST_FOREACH(typename ScriptNode<Real>::Operand const& op, script.list)
	{
		res = boost::apply_visitor(*this, op);
	}
	
	if (res.angle_measure != AngleMeasure::None)
	{
		switch (result_angle_measure)
		{
		case AngleMeasure::Radian:
			return res.ToRadian();
		case AngleMeasure::Degree:
			return res.ToDegree();
		case AngleMeasure::Grad:
			return res.ToGrad();
		}
	}
	return res;
}

template<>
Integer Solver<Integer>::operator()(ScriptNode<Integer> const& script, ElementId _id, AngleMeasure _default_angle_measure, 
	AngleMeasure _result_angle_measure, int _precision) const
{
	if (script.list.empty())
		throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected);
	
	id = _id;
	
	Integer res;

	//calculate all the script nodes
	BOOST_FOREACH(typename ScriptNode<Integer>::Operand const& op, script.list)
	{
		res = boost::apply_visitor(*this, op);
	}

	return res;
}

template<>
Rational Solver<Rational>::operator()(ScriptNode<Rational> const& script, ElementId _id, AngleMeasure _default_angle_measure, 
	AngleMeasure _result_angle_measure, int _precision) const
{
	if (script.list.empty())
		throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected);
	
	id = _id;
	
	Rational res;

	//calculate all the script nodes
	BOOST_FOREACH(typename ScriptNode<Rational>::Operand const& op, script.list)
	{
		res = boost::apply_visitor(*this, op);
	}

	return res;
}

}
