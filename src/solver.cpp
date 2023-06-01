#include "stdafx.h"
#include "solver.h"

namespace yutovo_calculator
{

//Solver

template<>
Solver<Integer>::Solver(int _precision, Integer _left_value, std::shared_ptr<SolverSymbols<Integer>> _symbols) : 
	precision(_precision),
	left_value(_left_value),
	symbols(_symbols)
{
	if (!symbols)
		symbols.reset(new SolverSymbols<Integer>());
}

template<>
Solver<Real>::Solver(int _precision, Real _left_value, std::shared_ptr<SolverSymbols<Real>> _symbols) : 
	precision(_precision),
	left_value(_left_value),
	symbols(_symbols)
{
	if (!symbols)
		symbols.reset(new SolverSymbols<Real>());
}

template<>
Solver<Rational>::Solver(int _precision, Rational _left_value, std::shared_ptr<SolverSymbols<Rational>> _symbols) : 
	precision(_precision),
	left_value(_left_value),
	symbols(_symbols)
{
	if (!symbols)
		symbols.reset(new SolverSymbols<Rational>());
}

//Visitor for FunctionCallNode<Integer>
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
		for (ExpressionNodesIter callIter = op.arguments.begin(); callIter != op.arguments.end(); ++callIter, ++funcIter)
		{
			Integer arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Integer arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Integer arg1 = (*this)(*callIter++);
			Integer arg2 = (*this)(*callIter);
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

//Visitor for FunctionCallNode<Real>
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
		for (ExpressionNodesIter callIter = op.arguments.begin(); callIter != op.arguments.end(); ++callIter, ++funcIter)
		{
			Real arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Real arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Real arg1 = (*this)(*callIter++);
			Real arg2 = (*this)(*callIter);
			return (*b)(arg1, arg2);
		}
		catch (boost::bad_get)
		{
		}
		
		try
		{
			RealTrigonometricFunc t = boost::get<RealTrigonometricFunc>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Real arg = (*this)(*callIter);
			return (*t)(arg, RADIAN);
		}
		catch (boost::bad_get)
		{
		}
	}

	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
	
	return res;
}

//Visitor for FunctionCallNode<Rational>
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
		for (ExpressionNodesIter callIter = op.arguments.begin(); callIter != op.arguments.end(); ++callIter, ++funcIter)
		{
			Rational arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Rational arg = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Rational arg1 = (*this)(*callIter++);
			Rational arg2 = (*this)(*callIter);
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
			
			ExpressionNodesIter callIter = op.arguments.begin();
			Real arg1 = (*this)(*callIter++);
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

//Visitor for IdentifierNode<Integer>
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

//Visitor for IdentifierNode<Real>
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

//Visitor for IdentifierNode<Rational>
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

}
