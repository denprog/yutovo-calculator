#ifndef SOLVER_H
#define SOLVER_H

#include "ast.h"
#include "script.h"
#include "utils.h"

namespace yutovo_calculator
{
	typedef Integer (*IntegerBinaryFunc)(const Integer& num1, const Integer& num2);
	typedef Real (*RealUnaryFunc)(const Real& num1);
	typedef Real (*RealBinaryFunc)(const Real& num1, const Real& num2);
	typedef Real (*RealTrigonometricFunc)(const Real& num1, const AngleMeasure angle_measure);
	
	typedef Integer (*IntegerVariable)();
	typedef Real (*RealPrecisionVariable)(const int precision);
	typedef Rational (*RationalVariable)();

	typedef std::vector<std::u32string> Dependencies;

	template<typename Number>
	struct SolverSymbols
	{
		typedef pair<std::u32string, Number> TempVariable;

		//build-in functions' typedefs
		typedef Number (*UnaryFunction)(const Number& num);
		typedef Number (*BinaryFunction)(const Number& num1, const Number& num2);
		typedef Number (*TrigonometricFunction)(const Number& num1, const AngleMeasure angle_measure);
		typedef boost::variant<UnaryFunction, BinaryFunction, TrigonometricFunction> BuildinFunction;
		
		//build-in variables' typedefs
		typedef Number (*Variable)();
		typedef Number (*PrecisionVariable)(const int precision);
		typedef boost::variant<Variable, PrecisionVariable> BuildinVariable;

		mutable deque<TempVariable> temp_variables; ///< The temporary variables
		mutable deque<VariableNode<Number>> variables;	///< The variables
		mutable vector<FunctionNode<Number>> functions; ///< The functions

		map<std::u32string, BuildinFunction> buildin_functions; ///< The buildin functions
		map<std::u32string, BuildinVariable> buildin_variables; ///< The buildin variables
	};
		
	template<typename Number>
	struct Solver : public boost::static_visitor<Number>
	{
		typedef typename std::list<ExpressionNode<Number> >::const_iterator ExpressionNodesIter;		
		typedef typename std::list<IdentifierNode<Number> >::const_iterator IdentifierNodesIter;
		
		typedef typename SolverSymbols<Number>::TempVariable TempVariable;
		typedef typename SolverSymbols<Number>::UnaryFunction UnaryFunction;
		typedef typename SolverSymbols<Number>::BinaryFunction BinaryFunction;
		typedef typename SolverSymbols<Number>::TrigonometricFunction TrigonometricFunction;
		typedef typename SolverSymbols<Number>::BuildinFunction BuildinFunction;
		
		typedef typename SolverSymbols<Number>::PrecisionVariable PrecisionVariable;
		typedef typename SolverSymbols<Number>::BuildinVariable BuildinVariable;
		typedef typename SolverSymbols<Number>::Variable Variable;
		
		SolverSymbols<Number>* symbols;
		
		Solver(int _precision, Number _left_value = Number(), SolverSymbols<Number>* _symbols = NULL);
		
		void SetPrecision(int prec)
		{
			precision = prec;
		}
		
		//Visitor for Number.
		Number operator()(Number n) const
		{
			n.SetPrecision(precision);
			return n;
		}

		//Visitor for ExpressionNode.
		Number operator()(ExpressionNode<Number> const& expr) const
		{
			//calculate all the expression's nodes
			Number res = boost::apply_visitor(*this, expr.first);
			BOOST_FOREACH(typename OperationNode<Number>::Operand const& op, expr.rest)
			{
				Solver<Number> solver(precision, res, symbols);
				solver.id = id;
				solver.SetDependencies(dependencies);
				res = boost::apply_visitor(solver, op);
			}
			
			return res;
		}
		
		//Visitor for DefinitionNode.
		Number operator()(DefinitionNode<Number> const& op) const
		{
			//pass the definition to the special functor
			boost::apply_visitor(*this, op.definition);
			return Number();
		}

		//Visitor for VariableNode.
		Number operator()(VariableNode<Number> const& op) const
		{
			//store the variable
			AddVariable(op);
			return Number();
		}
		
		//Visitor for FunctionNode.
		Number operator()(FunctionNode<Number> const& op) const
		{
			//store the function
			AddFunction(op);
			return Number();
		}

		//Visitor for UnaryOperationNode.
		Number operator()(UnaryOperationNode<Number> const& op) const
		{
			Number right = boost::apply_visitor(*this, op.operand);
			switch (op.op)
			{
			case '+':
				return right;
			case '-':
				return -right;
			}
			
			return Number();
		}

		//Visitor for OperationNode.
		Number operator()(OperationNode<Number> const& op) const
		{
			Number right = boost::apply_visitor(*this, op.operand);
			
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
			
			return Number();
		}

		Number operator()(FunctionCallNode<Number> const& op) const;

		Number operator()(FunctionParamNode<Number> const& expr) const
		{
			return boost::apply_visitor(*this, expr.op);
		}

		Number operator()(NoFencesFunctionCallNode<Number> const& op) const;

		Number operator()(IdentifierNode<Number> const& op) const;
		
		//The beginning of the solving.
		Number operator()(ScriptNode<Number> const& script, ElementId _id, int _precision = -1) const
		{
			if (script.list.empty())
				throw SyntaxException(id, ParserExceptionCode::ExpressionExpected);
			
			id = _id;

			if (_precision != -1)
				precision = _precision;
			
			Number res;
			//calculate all the script nodes
			BOOST_FOREACH(typename ScriptNode<Number>::Operand const& op, script.list)
			{
				res = boost::apply_visitor(*this, op);
			}
			
			return res;
		}

		//Push a temporary variable.
		void PushTempVariable(const std::u32string& name, Number& value) const
		{
			symbols->temp_variables.push_back(TempVariable(name, value));
		}
		
		//Pop a number of the temporary variables.
		void PopTempVariable(int count = 1) const
		{
			for (int i = 0; i < count; ++i)
				symbols->temp_variables.pop_back();
		}
		
		//Find a temporary variable.
		TempVariable* FindTempVariable(const std::u32string& name) const
		{
			for (int i = symbols->temp_variables.size() - 1; i >= 0; --i)
			{
				if (symbols->temp_variables[i].first == name)
					return &symbols->temp_variables[i];
			}
			
			return NULL;
		}

		void AddVariable(VariableNode<Number> const& var) const
		{
			var.id = id;
			symbols->variables.push_back(var);
		}

		//Find a variable.
		VariableNode<Number>* FindVariable(const std::u32string& name) const
		{
			for (int i = symbols->variables.size() - 1; i >= 0; --i)
			{
				if (symbols->variables[i].name.name == name)
					return &symbols->variables[i];
			}
			
			return NULL;
		}
		
		//Add a function.
		void AddFunction(FunctionNode<Number> const& func) const
		{
			for (int i = 0; i < (int)symbols->functions.size(); ++i)
			{
				if (symbols->functions[i].name.name == func.name.name)
				{
					symbols->functions[i] = func;
					return;
				}
			}

			symbols->functions.push_back((FunctionNode<Number>&)func);
		}
		
		void AddBuildinFunction(const u_char* name, UnaryFunction& func)
		{
			symbols->buildin_functions[std::u32string(name)] = func;
		}

		void AddBuildinFunction(const char* name, UnaryFunction& func)
		{
			symbols->buildin_functions[ToUtfString(name)] = func;
		}

		void AddBuildinFunction(const u_char* name, BinaryFunction& func)
		{
			symbols->buildin_functions[std::u32string(name)] = func;
		}

		void AddBuildinFunction(const char* name, BinaryFunction& func)
		{
			symbols->buildin_functions[ToUtfString(name)] = func;
		}

		void AddBuildinFunction(const u_char* name, TrigonometricFunction& func)
		{
			symbols->buildin_functions[std::u32string(name)] = func;
		}

		void AddBuildinFunction(const char* name, TrigonometricFunction& func)
		{
			symbols->buildin_functions[ToUtfString(name)] = func;
		}

		//Find a buildin function.
		BuildinFunction* FindBuildinFunction(const std::u32string& name) const
		{
			typename map<std::u32string, BuildinFunction>::const_iterator iter = symbols->buildin_functions.find(name);
			if (iter == symbols->buildin_functions.end())
				return NULL;
			return (BuildinFunction*)&(*iter).second;
		}

		void AddBuildinVariable(const u_char* name, PrecisionVariable& var)
		{
			symbols->buildin_variables[std::u32string(name)] = var;
		}

		void AddBuildinVariable(const char* name, PrecisionVariable& var)
		{
			symbols->buildin_variables[ToUtfString(name)] = var;
		}

		//Find a buildin variable.
		BuildinVariable* FindBuildinVariable(const std::u32string& name) const
		{
			typename map<std::u32string, BuildinVariable>::const_iterator iter = symbols->buildin_variables.find(name);
			if (iter == symbols->buildin_variables.end())
				return NULL;
			return (BuildinVariable*)&(*iter).second;
		}

		bool RemoveIdentifier(ElementId id, const std::u32string& name)
		{
			auto var_it = symbols->variables.erase(std::remove_if(symbols->variables.begin(), symbols->variables.end(), 
				[name](auto& var)
				{
					return var.name.name == name;
				}), 
				symbols->variables.end());
			if (var_it != symbols->variables.end())
				return true;

			auto func_it = symbols->functions.erase(std::remove_if(symbols->functions.begin(), symbols->functions.end(), 
				[name](auto& func)
				{
					return func.name.name == name;
				}), 
				symbols->functions.end());
			return func_it != symbols->functions.end();
		}

		void SetDependencies(Dependencies* _dependencies)
		{
			dependencies = _dependencies;
		}

	private:
		void AddDependency(const std::u32string& name) const
		{
			if (std::find(dependencies->begin(), dependencies->end(), name) == dependencies->end())
				dependencies->push_back(name);
		}
		
		mutable ElementId id;
		mutable int precision;
		Number left_value; //left solved value
		mutable Dependencies* dependencies = nullptr;
	};
};

#endif
