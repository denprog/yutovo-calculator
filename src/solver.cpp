#include "stdafx.h"
#include "solver.h"

namespace yutovo_calculator
{

//Solver

template<>
Real Solver<Real>::operator()(UnitNode<Real> const& op) const
{
	//store the unit
	AddUnit(op);
	return Real();
}

template<>
Rational Solver<Rational>::operator()(UnitNode<Rational> const& op) const
{
	AddUnit(op);
	return Rational();
}

template<>
Integer Solver<Integer>::operator()(UnitNode<Integer> const& op) const
{
	throw SyntaxException(op.id, SyntaxError, U"Identifier '" + op.name.name + U"' not found", op.pos, op.line);
}

template<>
Integer Solver<Integer>::operator()(UnaryOperationNode<Integer> const& op) const
{
	Integer right = boost::apply_visitor(*this, op.operand);
	switch (op.op)
	{
	case '+':
		return right;
	case '-':
		return -right;
	case '!':
		return !right;
	}
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Real Solver<Real>::operator()(UnaryOperationNode<Real> const& op) const
{
	Real right = boost::apply_visitor(*this, op.operand);
	switch (op.op)
	{
	case '+':
		return right;
	case '-':
		return -right;
	}
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Rational Solver<Rational>::operator()(UnaryOperationNode<Rational> const& op) const
{
	Rational right = boost::apply_visitor(*this, op.operand);
	switch (op.op)
	{
	case '+':
		return right;
	case '-':
		return -right;
	}
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Integer Solver<Integer>::operator()(OperationNode<Integer> const& op) const
{
	Integer right = boost::apply_visitor(*this, op.operand);
	
	try
	{
		//calculate the operation
		switch (op.op)
		{
		case '+':
			return left_value + right;
		case '-':
			return left_value - right;
		case '*':
			return left_value * right;
		case '/':
			return left_value / right;
		case '&':
			return left_value & right;
		case '|':
			return left_value | right;
		case '^':
			return left_value ^ right;
		}
	}
	catch (MathException e)
	{
		throw MathException(e.id, e.ex_id, op.pos, op.line);
	}
	
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Real Solver<Real>::operator()(OperationNode<Real> const& op) const
{
	Real right = boost::apply_visitor(*this, op.operand);
	
	try
	{
		//calculate the operation
		switch (op.op)
		{
		case '+':
			return left_value + right;
		case '-':
			return left_value - right;
		case '*':
			return left_value * right;
		case '/':
			return left_value / right;
		}
	}
	catch (MathException e)
	{
		throw MathException(e.id, e.ex_id, op.pos, op.line);
	}
	
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Rational Solver<Rational>::operator()(OperationNode<Rational> const& op) const
{
	Rational right = boost::apply_visitor(*this, op.operand);
	
	try
	{
		//calculate the operation
		switch (op.op)
		{
		case '+':
			return left_value + right;
		case '-':
			return left_value - right;
		case '*':
			return left_value * right;
		case '/':
			return left_value / right;
		}
	}
	catch (MathException e)
	{
		throw MathException(e.id, e.ex_id, op.pos, op.line);
	}
	
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Integer Solver<Integer>::operator()(PostfixOperationNode<Integer> const& op) const
{
	Integer left = boost::apply_visitor(*this, op.operand);
	switch (op.op)
	{
	case '!':
		return fact(left);
	}
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Real Solver<Real>::operator()(PostfixOperationNode<Real> const& op) const
{
	Real left = boost::apply_visitor(*this, op.operand);
	switch (op.op)
	{
	case '!':
		return fact(left);
	}
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Rational Solver<Rational>::operator()(PostfixOperationNode<Rational> const& op) const
{
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

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
	BuiltinFunction* func = FindBuiltinFunction(op.name.name);
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

	TrigonometricFunction* t_func = FindTrigonometricFunction(op.name.name);
	if (t_func)
	{
		try
		{
			UnaryFunction u = boost::get<UnaryFunction>(*t_func);
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
	}
	
	//find in the build-in functions		
	BuiltinFunction* func = FindBuiltinFunction(op.name.name);
	if (func)
	{
		try
		{
			UnaryFunction u = boost::get<UnaryFunction>(*func);
			if (op.arguments.size() != 1)
				throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
			
			ExpressionNodesIter iter = op.arguments.begin();
			Real arg = (*this)(*iter);
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
	BuiltinFunction* func = FindBuiltinFunction(op.name.name);
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
Integer Solver<Integer>::operator()(FunctionCallStringNode<Integer> const& op) const
{
	AddDependency(op.name.name);

	Integer res;

	BuiltinFunction* func = FindBuiltinFunction(op.name.name);
	if (func)
	{
		try
		{
			StringFunction u = boost::get<StringFunction>(*func);
			return (*u)(op.argument);
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
Real Solver<Real>::operator()(FunctionCallStringNode<Real> const& op) const
{
	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
}

template<>
Rational Solver<Rational>::operator()(FunctionCallStringNode<Rational> const& op) const
{
	//there is no such a function		
	throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
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
	BuiltinFunction* func = FindBuiltinFunction(op.name.name);
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
	VariableNode<Integer>* v = FindVariable(op.name, op.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Integer res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	BuiltinVariable* var = FindBuiltinVariable(op.name);
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
	VariableNode<Real>* v = FindVariable(op.name, op.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Real res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	//find in the build-in variables
	BuiltinVariable* var = FindBuiltinVariable(op.name);
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

	if (op.subscript.empty() || op.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			return Real(precision, *unit);
		}
	}

	auto* custom_unit = FindUnit(op.name, op.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		return custom_unit->value;
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
	VariableNode<Rational>* v = FindVariable(op.name, op.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Rational res = (*this)(v->expression);
		id = _id;
		return res;
	}
	
	BuiltinVariable* var = FindBuiltinVariable(op.name);
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

	if (op.subscript.empty() || op.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			return Rational(*unit);
		}
	}

	auto* custom_unit = FindUnit(op.name, op.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		return custom_unit->value;
	}

	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.line);
	
	return 0;
}

template<>
Integer Solver<Integer>::operator()(ImplicitStringMulNode<Integer> const& op) const
{
	AddDependency(op.identifier.name);

	TempVariable* t = FindTempVariable(op.identifier.name);
	if (t)
		return op.left * t->second;
	
	//find in user defined variables
	VariableNode<Integer>* v = FindVariable(op.identifier.name, op.identifier.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Integer res = (*this)(v->expression);
		id = _id;
		return op.left * res;
	}
	
	BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
	if (var)
	{
		try
		{
			IntegerVariable v = boost::get<Variable>(*var);
			return op.left * (*v)();
		}
		catch (boost::bad_get)
		{
		}
	}
	
	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.line);
	
	return 0;
}

template<>
Real Solver<Real>::operator()(ImplicitStringMulNode<Real> const& op) const
{
	AddDependency(op.identifier.name);

	TempVariable* t = FindTempVariable(op.identifier.name);
	if (t)
		return op.left * t->second;
	
	//find in the user defined variables
	VariableNode<Real>* v = FindVariable(op.identifier.name, op.identifier.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Real res = (*this)(v->expression);
		id = _id;
		return op.left * res;
	}
	
	//find in the build-in variables
	BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
	if (var)
	{
		try
		{
			RealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
			return op.left * (*v)(precision);
		}
		catch (boost::bad_get)
		{
		}
	}

	if (op.identifier.subscript.empty() || op.identifier.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.identifier.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			return op.left * Real(precision, *unit);
		}
	}

	auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		return op.left * custom_unit->value;
	}

	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.line);
	
	return Real();
}

template<>
Rational Solver<Rational>::operator()(ImplicitStringMulNode<Rational> const& op) const
{
	AddDependency(op.identifier.name);

	TempVariable* t = FindTempVariable(op.identifier.name);
	if (t)
		return op.left * t->second;
	
	//find in user defined variables
	VariableNode<Rational>* v = FindVariable(op.identifier.name, op.identifier.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Rational res = (*this)(v->expression);
		id = _id;
		return op.left * res;
	}
	
	BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
	if (var)
	{
		try
		{
			RationalVariable v = boost::get<Variable>(*var);
			return op.left * (*v)();
		}
		catch (boost::bad_get)
		{
		}
	}

	if (op.identifier.subscript.empty() || op.identifier.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.identifier.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			return op.left * Rational(*unit);
		}
	}

	auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		return op.left * custom_unit->value;
	}

	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.line);

	return Rational();
}

template<>
Integer Solver<Integer>::operator()(ImplicitDivMulNode<Integer> const& op) const
{
	throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Real Solver<Real>::operator()(ImplicitDivMulNode<Real> const& op) const
{
	AddDependency(op.identifier.name);

	TempVariable* t = FindTempVariable(op.identifier.name);
	if (t)
	{
		Real arg1 = (*this)(op.upper);
		Real arg2 = (*this)(op.lower);
		return (arg1 / arg2) * t->second;
	}
	
	//find in the user defined variables
	VariableNode<Real>* v = FindVariable(op.identifier.name, op.identifier.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Real res = (*this)(v->expression);
		id = _id;
		Real arg1 = (*this)(op.upper);
		Real arg2 = (*this)(op.lower);
		return (arg1 / arg2) * res;
	}
	
	//find in the build-in variables
	BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
	if (var)
	{
		try
		{
			RealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
			Real arg1 = (*this)(op.upper);
			Real arg2 = (*this)(op.lower);
			return (arg1 / arg2) * (*v)(precision);
		}
		catch (boost::bad_get)
		{
		}
	}

	if (op.identifier.subscript.empty() || op.identifier.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.identifier.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			Real arg1 = (*this)(op.upper);
			Real arg2 = (*this)(op.lower);
			return (arg1 / arg2) * Real(precision, *unit);
		}
	}

	auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		Real arg1 = (*this)(op.upper);
		Real arg2 = (*this)(op.lower);
		return (arg1 / arg2) * custom_unit->value;
	}

	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.line);
	
	return Real();
}

template<>
Rational Solver<Rational>::operator()(ImplicitDivMulNode<Rational> const& op) const
{
	AddDependency(op.identifier.name);

	TempVariable* t = FindTempVariable(op.identifier.name);
	if (t)
	{
		Rational arg1 = (*this)(op.upper);
		Rational arg2 = (*this)(op.lower);
		return (arg1 / arg2) * t->second;
	}
	
	//find in the user defined variables
	VariableNode<Rational>* v = FindVariable(op.identifier.name, op.identifier.subscript);
	if (v)
	{
		ElementId _id = id;
		id = v->id;
		Rational res = (*this)(v->expression);
		id = _id;
		Rational arg1 = (*this)(op.upper);
		Rational arg2 = (*this)(op.lower);
		return (arg1 / arg2) * res;
	}
	
	//find in the build-in variables
	BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
	if (var)
	{
		try
		{
			RationalVariable v = boost::get<Variable>(*var);
			Rational arg1 = (*this)(op.upper);
			Rational arg2 = (*this)(op.lower);
			return (arg1 / arg2) * (*v)();
		}
		catch (boost::bad_get)
		{
		}
	}

	if (op.identifier.subscript.empty() || op.identifier.subscript == U"SI")
	{
		Unit* unit = FindBuiltinUnit(op.identifier.name);
		if (unit)
		{
			symbols->last_unit_system = U"SI";
			Rational arg1 = (*this)(op.upper);
			Rational arg2 = (*this)(op.lower);
			return (arg1 / arg2) * Rational(*unit);
		}
	}

	auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
	if (custom_unit)
	{
		symbols->last_unit_system = custom_unit->system;
		Rational arg1 = (*this)(op.upper);
		Rational arg2 = (*this)(op.lower);
		return (arg1 / arg2) * custom_unit->value;
	}

	//there is no such an identifier
	throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.line);
	
	return Rational();
}

template<>
Integer Solver<Integer>::operator()(ImplicitMulNode<Integer> const& op) const
{
	Integer arg1 = (*this)(op.before);
	Integer arg2 = (*this)(op.inside_braces);
	return arg1 * arg2;
}

template<>
Real Solver<Real>::operator()(ImplicitMulNode<Real> const& op) const
{
	Real arg1 = (*this)(op.before);
	Real arg2 = (*this)(op.inside_braces);
	return arg1 * arg2;
}

template<>
Rational Solver<Rational>::operator()(ImplicitMulNode<Rational> const& op) const
{
	Rational arg1 = (*this)(op.before);
	Rational arg2 = (*this)(op.inside_braces);
	return arg1 * arg2;
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

template<>
Real Solver<Real>::GetSuitableUnit(const ElementId _id, const Real& val, const std::u32string& system, const bool buildin) const
{
	return GetSuitableUnitImpl(_id, val, system, buildin);
}

template<>
Rational Solver<Rational>::GetSuitableUnit(const ElementId _id, const Rational& val, const std::u32string& system, const bool buildin) const
{
	return GetSuitableUnitImpl(_id, val, system, buildin);
}

template<>
Integer Solver<Integer>::GetSuitableUnit(const ElementId _id, const Integer& val, const std::u32string& system, const bool buildin) const
{
	return val;
}

template<>
Real Solver<Real>::CastToUnit(const ElementId id, const Real& val, const Unit& unit) const
{
	return CastToUnitImpl(id, val, unit);
}

template<>
Rational Solver<Rational>::CastToUnit(const ElementId id, const Rational& val, const Unit& unit) const
{
	return CastToUnitImpl(id, val, unit);
}

template<>
Integer Solver<Integer>::CastToUnit(const ElementId id, const Integer& val, const Unit& unit) const
{
	throw MathException(id, ParserExceptionCode::CannotCastToUnit);
}

}
