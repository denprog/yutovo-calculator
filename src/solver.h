#ifndef SOLVER_H
#define SOLVER_H

#include "ast.h"
#include "script.h"
#include "utils.h"
#include <chrono>

namespace yutovo_calculator
{

using namespace std::chrono;

typedef Integer (*IntegerUnaryFunc)(const Integer &num);
typedef Integer (*IntegerBinaryFunc)(const Integer& num1, const Integer& num2);
typedef Integer (*IntegerStringFunc)(const std::u32string& str);
typedef Integer (*IntegerVariable)();

typedef Real (*RealUnaryFunc)(const Real& num);
typedef Real (*RealBinaryFunc)(const Real& num1, const Real& num2);
typedef Real (*RealTrigonometricFunc)(const Real& num);
typedef Real (*RealPrecisionVariable)(const int precision);

typedef Complex (*ComplexUnaryFunc)(const Complex& num, int& res_pos);
typedef Complex (*ComplexBinaryFunc)(const Complex& num1, const Complex& num2, int& res_pos);
typedef Complex (*ComplexTrigonometricFunc)(const Complex& num, int& res_pos);
typedef Complex (*ComplexPrecisionVariable)(const int precision, AngleMeasure angle_measure);
typedef Complex (*ComplexFunc)(const Complex& num);

typedef Rational (*RationalUnaryFunc)(const Rational &num);
typedef Rational (*RationalBinaryFunc)(const Rational& num1, const Rational& num2);
typedef Rational (*RationalVariable)();

typedef std::vector<std::u32string> Dependencies;

template<typename Number>
struct CustomUnit
{
    CustomUnit(const LogicalId _id, const std::u32string _name, const std::u32string _system, Number _value, bool _buildin) :
        id(_id),
        name(_name),
        system(_system),
        value(_value),
        buildin(_buildin)
    {
        if (system.empty())
            system = U"SI";
    }

    bool Cast(Number& val) const
    {
        Number _val = val;
        Number res_val = val;
        Unit _val_unit = val.unit;
        Unit res_unit = value.unit;
        int power = 0;
        bool f = true;

        while (f)
        {
            for (auto& u : res_unit.unit)
            {
                auto it = std::find_if(_val.unit.unit.begin(), _val.unit.unit.end(), 
                    [u](const auto& p)
                    {
                        if (p.first != u.first)
                            return false;
                        if (p.second > 0 && u.second > 0)
                            return p.second >= u.second;
                        if (p.second < 0 && u.second < 0)
                            return p.second <= u.second;
                        return false;
                    });
                if (it == _val.unit.unit.end())
                {
                    f = false;
                    break;
                }
                it->second -= u.second;
            }
            if (f)
            {
                ++power;
                for (size_t i = 0; i < _val.unit.unit.size(); ++i)
                {
                    auto& p = _val.unit.unit[i];
                    if (p.second == 0)
                        _val.unit.unit.erase(_val.unit.unit.begin() + i--);
                }
                res_val = _val;
            }
        }

        if (power == 0)
        {
            //try to find the backward variant of the unit
            f = true;
            _val = val;
            while (f)
            {
                for (auto& u : res_unit.unit)
                {
                    auto it = std::find_if(_val.unit.unit.begin(), _val.unit.unit.end(), 
                        [u](const auto& p)
                        {
                            if (p.first != u.first)
                                return false;
                            if (p.second < 0 && u.second > 0)
                                return abs(p.second) >= u.second;
                            if (p.second > 0 && u.second < 0)
                                return p.second >= abs(u.second);
                            return false;
                        });
                    if (it == _val.unit.unit.end())
                    {
                        f = false;
                        break;
                    }
                    it->second += u.second;
                }
                if (f)
                {
                    --power;
                    for (size_t i = 0; i < _val.unit.unit.size(); ++i)
                    {
                        auto& p = _val.unit.unit[i];
                        if (p.second == 0)
                            _val.unit.unit.erase(_val.unit.unit.begin() + i--);
                    }
                    res_val = _val;
                }
            }
        }

        if (power == 0)
            return false;

        res_val.unit.unit.insert(res_val.unit.unit.begin(), std::make_pair(name, power));
        auto u = res_val.unit;
        if (power > 0)
            res_val = res_val / pow(value, power);
        else
            res_val = res_val * pow(value, -power);
        u.system = system;
        u.description = U"";
        res_val.SetUnit(u);
        if (_val_unit == res_val.unit)
            return false;
        val = res_val;
        return true;
    }

    LogicalId id;
    std::u32string name;
    std::u32string system;
    Number value;
    bool buildin;
    std::u32string description;
};

template<typename Number>
struct SolverSymbols
{
    typedef pair<std::u32string, Number> TempVariable;

    //build-in functions' typedefs
    typedef Number (*UnaryFunction)(const Number& num);
    typedef Number (*BinaryFunction)(const Number& num1, const Number& num2);
    typedef Number (*StringFunction)(const std::u32string& str);
    typedef Number (*TrigonometricFunction)(const Number& num);
    typedef Number (*ComplexUnaryFunction)(const Number& num, int& res_pos);
    typedef Number (*ComplexTrigonometricFunction)(const Number& num, int& res_pos);
    typedef Number (*ComplexBinaryFunction)(const Number& num1, const Number& num2, int& res_pos);

    typedef boost::variant<UnaryFunction, BinaryFunction, StringFunction, ComplexUnaryFunction, ComplexTrigonometricFunction, 
        ComplexBinaryFunction> BuiltinFunction;
    typedef boost::variant<TrigonometricFunction, ComplexTrigonometricFunction> BuiltinTrigonometricFunction;
    
    //build-in variables' typedefs
    typedef Number (*Variable)();
    typedef Number (*PrecisionVariable)(const int precision);
    typedef boost::variant<Variable, PrecisionVariable, ComplexPrecisionVariable> BuiltinVariable;

    mutable deque<TempVariable> temp_variables;
    mutable deque<VariableNode<Number>> variables; //user variables
    mutable vector<FunctionNode<Number>> functions; //user functions

    map<std::u32string, BuiltinFunction> buildin_functions;
    map<std::u32string, BuiltinTrigonometricFunction> trigonometric_functions;
    map<std::u32string, BuiltinVariable> buildin_variables;
    std::vector<Unit> buildin_units;
    std::vector<CustomUnit<Number>> units;

    std::u32string last_unit_system;
    bool buildin_elements = false;
};

template<typename Number>
struct Solver : public boost::static_visitor<Number>
{
    typedef typename std::list<ExpressionNode<Number> >::const_iterator ExpressionNodesIter;		
    typedef typename std::list<IdentifierNode<Number> >::const_iterator IdentifierNodesIter;
    
    typedef typename SolverSymbols<Number>::TempVariable TempVariable;
    typedef typename SolverSymbols<Number>::UnaryFunction UnaryFunction;
    typedef typename SolverSymbols<Number>::BinaryFunction BinaryFunction;
    typedef typename SolverSymbols<Number>::StringFunction StringFunction;
    typedef typename SolverSymbols<Number>::TrigonometricFunction TrigonometricFunction;
    typedef typename SolverSymbols<Number>::BuiltinFunction BuiltinFunction;
    typedef typename SolverSymbols<Number>::BuiltinTrigonometricFunction BuiltinTrigonometricFunction;
    typedef typename SolverSymbols<Number>::ComplexUnaryFunction ComplexUnaryFunction;
    typedef typename SolverSymbols<Number>::ComplexTrigonometricFunction ComplexTrigonometricFunction;
    typedef typename SolverSymbols<Number>::ComplexBinaryFunction ComplexBinaryFunction;
    
    typedef typename SolverSymbols<Number>::PrecisionVariable PrecisionVariable;
    typedef typename SolverSymbols<Number>::BuiltinVariable BuiltinVariable;
    typedef typename SolverSymbols<Number>::Variable Variable;
    
    std::shared_ptr<SolverSymbols<Number>> symbols;
    
    Solver(int _precision, AngleMeasure _default_angle_measure, std::u32string _im = U"i", Number _left_value = Number(), 
        std::shared_ptr<SolverSymbols<Number>> _symbols = nullptr) :
        precision(_precision),
        default_angle_measure(_default_angle_measure),
        left_value(_left_value),
        symbols(_symbols),
        im(_im)
    {
        if (!symbols)
            symbols.reset(new SolverSymbols<Number>());
    }

    Solver(int _precision, AngleMeasure _default_angle_measure, AngleMeasure _result_angle_measure, int _default_notation, std::u32string _im, 
        Number _left_value = Number(), std::shared_ptr<SolverSymbols<Number>> _symbols = nullptr) :
        precision(_precision),
        default_angle_measure(_default_angle_measure),
        result_angle_measure(_result_angle_measure),
        default_notation(_default_notation),
        left_value(_left_value),
        symbols(_symbols),
        im(_im)
    {
        if (!symbols)
            symbols.reset(new SolverSymbols<Number>());
    }
    
    Number operator()(Number n) const
    {
        n.SetPrecision(precision);
        return n;
    }

    Number operator()(NumberNode<Number> const& op) const;

    Number operator()(ExpressionNode<Number> const& expr) const
    {
        //calculate all the expression's nodes
        Number res = boost::apply_visitor(*this, expr.first);
        BOOST_FOREACH(typename OperationNode<Number>::Operand const& op, expr.rest)
        {
            CheckBreak(parser_context);
            
            Solver<Number> solver(precision, default_angle_measure, result_angle_measure, default_notation, im, res, symbols);
            solver.parser_context = parser_context;
            solver.id = id;
            solver.cast_units = cast_units;
            solver.max_cast_unit_size = max_cast_unit_size;
            solver.SetDependencies(dependencies);
            res = boost::apply_visitor(solver, op);
        }
        
        return res;
    }
    
    Number operator()(DefinitionNode<Number> const& op) const
    {
        //pass the definition to the special functor
        boost::apply_visitor(*this, op.definition);
        return Number();
    }

    Number operator()(VariableNode<Number> const& op) const
    {
        //store the variable
        AddVariable(op);
        return Number();
    }

    Number operator()(UnitNode<Number> const& op) const;

    Number operator()(FunctionNode<Number> const& op) const
    {
        //store the function
        AddFunction(op);
        return Number();
    }

    Number operator()(UnaryOperationNode<Number> const& op) const;

    Number operator()(OperationNode<Number> const& op) const;

    Number operator()(PostfixOperationNode<Number> const& op) const;

    Number operator()(MixedDivivsionNode<Number> const& op) const
    {
        return (*this)(op.left) + (*this)(op.numerator) / (*this)(op.denominator);
    }

    Number operator()(FunctionCallNode<Number> const& op) const;

    Number operator()(FunctionCallStringNode<Number> const& op) const;

    Number operator()(FunctionParamNode<Number> const& expr) const
    {
        return boost::apply_visitor(*this, expr.op);
    }

    Number operator()(NoFencesFunctionCallNode<Number> const& op) const;

    Number operator()(IdentifierNode<Number> const& op) const;

    Number operator()(ImplicitStringMulNode<Number> const& op) const;

    Number operator()(ImplicitDivMulNode<Number> const& op) const;

    Number operator()(ImplicitMulNode<Number> const& op) const
    {
        Number arg1 = (*this)(op.before);
        Number arg2 = (*this)(op.inside_braces);
        return arg1 * arg2;
    }

    Number operator()(ImplicitFractionMulNode<Number> const& op) const
    {
        Number arg1 = (*this)(op.numerator);
        Number arg2 = (*this)(op.denominator);
        return arg1 / arg2 * (*this)(op.identifier);
    }

    Number operator()(ImplicitFunctionMulNode<Number> const& op) const
    {
        return (*this)(op.left) * (*this)(op.function_call);
    }

    Number operator()(ImplicitPostFunctionMulNode<Number> const& op) const
    {
        return (*this)(op.function_call) * (*this)(op.right);
    }

    Number operator()(CompareNode<Number> const& op) const
    {
        Number left = (*this)(op.left);
        Number right = (*this)(op.right);
        if (op.sign == U"<")
            return left < right ? Number(precision, 1) : Number(precision, 0);
        else if (op.sign == U">")
            return left > right ? Number(precision, 1) : Number(precision, 0);
        else if (op.sign == U"<=")
            return left <= right ? Number(precision, 1) : Number(precision, 0);
        else if (op.sign == U">=")
            return left >= right ? Number(precision, 1) : Number(precision, 0);
        else if (op.sign == U"==")
            return left == right ? Number(precision, 1) : Number(precision, 0);
        else if (op.sign == U"<>")
            return left != right ? Number(precision, 1) : Number(precision, 0);
        throw SyntaxException(op.id, ArgumentIsOver, op.pos, op.line);
    }

    Number operator()(LoopNode<Number> const& op) const
    {
        Number counter = (*this)(op.counter.expression);
        PushTempVariable(op.counter.name.name, counter);
        Number counter_max;
        Number res;
        try
        {
            counter_max = (*this)(op.counter_max);
            res = (*this)(op.loop_var.expression);
        }
        catch (...)
        {
            PopTempVariables(1);
            throw;
        }
        PushTempVariable(op.loop_var.name.name, res);
        while (counter_max != 0)
        {
            CheckBreak(parser_context);

            try
            {
                res = (*this)(op.loop_expression.expression);
                SetTempVariable(op.loop_expression.name.name, res);
                counter = (*this)(op.counter_increment.expression);
                SetTempVariable(op.counter.name.name, counter);
                counter_max = (*this)(op.counter_max);
            }
            catch (...)
            {
                PopTempVariables(2);
                throw;
            }
        }
        PopTempVariables(2);
        return res;
    }

    //The beginning of the solving.
    Number operator()(ScriptNode<Number> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
        AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const;

    void PushTempVariable(const std::u32string& name, Number& value) const
    {
        symbols->temp_variables.push_back(TempVariable(name, value));
    }

    void SetTempVariable(const std::u32string& name, Number& value) const
    {
        auto it = std::find_if(symbols->temp_variables.begin(), symbols->temp_variables.end(), 
            [name](TempVariable& var)
            {
                return var.first == name;
            });
        if (it != symbols->temp_variables.end())
            it->second = value;
    }

    void PopTempVariables(int count) const
    {
        for (int i = 0; i < count && !symbols->temp_variables.empty(); ++i)
            symbols->temp_variables.pop_back();
    }

    void ClearTempVariables()
    {
        symbols->temp_variables.clear();
    }

    TempVariable* FindTempVariable(const std::u32string& name) const
    {
        for (int i = symbols->temp_variables.size() - 1; i >= 0; --i)
        {
            if (symbols->temp_variables[i].first == name)
                return &symbols->temp_variables[i];
        }
        
        return nullptr;
    }

    void AddVariable(VariableNode<Number> const& var) const
    {
        int j = -1;
        LogicalId min_same_name_id;
        for (size_t i = 0; i < symbols->variables.size(); ++i)
        {
            auto& v = symbols->variables[i];
            if (v.id == id)
            {
                v = var;
                v.id = id;
                return;
            }
            if (v.name.name == var.name.name)
            {
                if (IsLess(v.id, id))
                    j = i + 1;
                else
                {
                    if (!min_same_name_id.empty())
                    {
                        if (IsLess(v.id, min_same_name_id))
                        {
                            min_same_name_id = v.id;
                            j = i;
                        }
                    }
                    else
                    {
                        min_same_name_id = v.id;
                        j = i;
                    }
                }
            }
        }
        var.id = id;
        if (j == -1)
            symbols->variables.push_back(var);
        else
            symbols->variables.insert(symbols->variables.begin() + j, var);

        (*this)(var.expression); //for adding dependencies
    }

    void AddUnit(UnitNode<Number> const& unit) const
    {
        auto it = std::find_if(symbols->units.begin(), symbols->units.end(), 
            [id = id](const CustomUnit<Number>& u)
            {
                return u.id == id;
            });
        if (it != symbols->units.end())
            symbols->units.erase(it);

        AddDependency(unit.name.name);
        Number res = (*this)(unit.expression);
        auto c = CustomUnit<Number>(id, unit.name.name, unit.name.subscript, res, symbols->buildin_elements);
        c.description = unit.name.description;
        symbols->units.emplace_back(c);
    }

    VariableNode<Number>* FindVariable(const std::u32string& name, const std::u32string& subscript) const
    {
        for (int i = symbols->variables.size() - 1; i >= 0; --i)
        {
            auto& var = symbols->variables[i];
            if (IsLess(var.id, id) && var.name.name == name && var.name.subscript == subscript)
                return &var;
        }
        return nullptr;
    }

    FunctionNode<Number>* FindFunction(FunctionCallNode<Number> const& op) const
    {
        for (int i = (int)symbols->functions.size() - 1; i >= 0; --i)
        {
            auto& func = symbols->functions[i];
            if (IsLess(func.id, id) && func.name.name == op.name.name)
            {
                if (func.arguments.size() != op.arguments.size())
                    throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
                return &func;
            }
        }
        return nullptr;
    }

    void AddFunction(FunctionNode<Number> const& func) const
    {
        int j = -1;
        LogicalId min_same_name_id;
        for (size_t i = 0; i < symbols->functions.size(); ++i)
        {
            auto& f = symbols->functions[i];
            if (f.id == id)
            {
                f = func;
                f.id = id;
                return;
            }
            if (f.name.name == func.name.name)
            {
                if (IsLess(f.id, id))
                    j = i + 1;
                else
                {
                    if (!min_same_name_id.empty())
                    {
                        if (IsLess(f.id, min_same_name_id))
                        {
                            min_same_name_id = f.id;
                            j = i;
                        }
                    }
                    else
                    {
                        min_same_name_id = f.id;
                        j = i;
                    }
                }
            }
        }
        func.id = id;
        if (j == -1)
            symbols->functions.push_back((FunctionNode<Number>&)func);
        else
            symbols->functions.insert(symbols->functions.begin() + j, (FunctionNode<Number>&)func);

        //parse for adding dependencies
        Number arg;
        for (IdentifierNodesIter iter = func.arguments.begin(); iter != func.arguments.end(); ++iter)
            PushTempVariable(iter->name, arg);
        (*this)(func.return_expression);
        PopTempVariables(func.arguments.size());
    }

    void AddBuiltinFunction(const char* name, UnaryFunction& func)
    {
        symbols->buildin_functions[ToUtfString(name)] = func;
    }

    void AddBuiltinFunction(const char* name, BinaryFunction& func)
    {
        symbols->buildin_functions[ToUtfString(name)] = func;
    }

    void AddBuiltinFunction(const char* name, StringFunction& func)
    {
        symbols->buildin_functions[ToUtfString(name)] = func;
    }

    void AddTrigonometricFunction(const char* name, TrigonometricFunction& func)
    {
        symbols->trigonometric_functions[ToUtfString(name)] = func;
    }

    void AddBuiltinFunction(const char* name, ComplexUnaryFunction& func)
    {
        symbols->buildin_functions[ToUtfString(name)] = func;
    }

    void AddTrigonometricFunction(const char* name, ComplexTrigonometricFunction& func)
    {
        symbols->trigonometric_functions[ToUtfString(name)] = func;
    }

    void AddBuiltinFunction(const char* name, ComplexBinaryFunction& func)
    {
        symbols->buildin_functions[ToUtfString(name)] = func;
    }

    BuiltinFunction* FindBuiltinFunction(const std::u32string& name) const
    {
        typename map<std::u32string, BuiltinFunction>::const_iterator iter = symbols->buildin_functions.find(name);
        if (iter == symbols->buildin_functions.end())
            return nullptr;
        return (BuiltinFunction*)&(*iter).second;
    }

    BuiltinTrigonometricFunction* FindTrigonometricFunction(const std::u32string& name) const
    {
        typename map<std::u32string, BuiltinTrigonometricFunction>::const_iterator iter = symbols->trigonometric_functions.find(name);
        if (iter == symbols->trigonometric_functions.end())
            return nullptr;
        return (BuiltinTrigonometricFunction*)&(*iter).second;
    }

    void AddBuiltinVariable(const char* name, PrecisionVariable& var)
    {
        symbols->buildin_variables[ToUtfString(name)] = var;
    }

    void AddBuiltinVariable(const char* name, ComplexPrecisionVariable& var)
    {
        symbols->buildin_variables[ToUtfString(name)] = var;
    }

    BuiltinVariable* FindBuiltinVariable(const std::u32string& name) const
    {
        typename map<std::u32string, BuiltinVariable>::const_iterator iter = symbols->buildin_variables.find(name);
        if (iter == symbols->buildin_variables.end())
            return nullptr;
        return (BuiltinVariable*)&(*iter).second;
    }

    bool RemoveIdentifier(LogicalId id, const std::u32string& name)
    {
        auto var_it = symbols->variables.erase(std::remove_if(symbols->variables.begin(), symbols->variables.end(), 
            [id, name](auto& var)
            {
                return var.name.name == name && var.id == id;
            }), 
            symbols->variables.end());
        if (var_it != symbols->variables.end())
            return true;

        auto func_it = symbols->functions.erase(std::remove_if(symbols->functions.begin(), symbols->functions.end(), 
            [id, name](auto& func)
            {
                return func.name.name == name && func.id == id;;
            }), 
            symbols->functions.end());
        if (func_it != symbols->functions.end())
            return true;

        auto unit_it = symbols->units.erase(std::remove_if(symbols->units.begin(), symbols->units.end(), 
            [id, name](auto& unit)
            {
                return unit.name == name && unit.id == id;;
            }), 
            symbols->units.end());
        return unit_it != symbols->units.end();
    }

    bool RemoveIdentifier(LogicalId id)
    {
        auto var_it = symbols->variables.erase(std::remove_if(symbols->variables.begin(), symbols->variables.end(), 
            [id](auto& var)
            {
                return var.id == id;
            }), 
            symbols->variables.end());
        if (var_it != symbols->variables.end())
            return true;

        auto func_it = symbols->functions.erase(std::remove_if(symbols->functions.begin(), symbols->functions.end(), 
            [id](auto& func)
            {
                return func.id == id;;
            }), 
            symbols->functions.end());
        if (func_it != symbols->functions.end())
            return true;

        auto unit_it = symbols->units.erase(std::remove_if(symbols->units.begin(), symbols->units.end(), 
            [id](auto& unit)
            {
                return unit.id == id;;
            }), 
            symbols->units.end());
        return unit_it != symbols->units.end();
    }

    void AddBuiltinUnit(const Unit& unit)
    {
        symbols->buildin_units.push_back(unit);
    }

    void ResetBuildinUnits()
    {
        symbols->buildin_units.clear();
    }

    void RemoveUserIdentifiers()
    {
        //user identifiers have ids' positions >= 0
        symbols->units.erase(std::remove_if(symbols->units.begin(), symbols->units.end(), 
            [](auto& unit)
            {
                for (int p : unit.id)
                {
                    if (p < 0)
                        return false;
                }
                return true;
            }), 
            symbols->units.end());

        symbols->variables.erase(std::remove_if(symbols->variables.begin(), symbols->variables.end(), 
            [](auto& var)
            {
                for (int p : var.id)
                {
                    if (p < 0)
                        return false;
                }
                return true;
            }), 
            symbols->variables.end());

        symbols->functions.erase(std::remove_if(symbols->functions.begin(), symbols->functions.end(), 
            [](auto& func)
            {
                for (int p : func.id)
                {
                    if (p < 0)
                        return false;
                }
                return true;
            }), 
            symbols->functions.end());
    }

    Unit* FindBuiltinUnit(const std::u32string& name) const
    {
        auto iter = std::find_if(symbols->buildin_units.begin(), symbols->buildin_units.end(), 
            [name](Unit& unit)
            {
                return unit == name;
            });
        if (iter == symbols->buildin_units.end())
            return nullptr;
        return &(*iter);
    }

    CustomUnit<Number>* FindUnit(const std::u32string& name, const std::u32string& system) const
    {
        CustomUnit<Number>* res = nullptr;
        LogicalId unit_id;
        auto _system = system;
        if (_system.empty())
            _system = U"SI";
        for (int i = symbols->units.size() - 1; i >= 0; --i)
        {
            CustomUnit<Number>& unit = symbols->units[i];
            auto s = unit.system;
            if (s.empty())
                s = U"SI";
            if (_system != s)
                continue;
            if (unit.buildin && symbols->buildin_elements && !IsLess(unit.id, id))
                continue;
            if (!unit.buildin && !symbols->buildin_elements && !IsLess(unit.id, id))
                continue;
            if (!unit.buildin && symbols->buildin_elements)
                continue;
            if (unit.name == name)
            {
                if (unit.buildin || IsLess(unit.id, id))
                {
                    if (!unit_id.empty())
                    {
                        if (unit.buildin || IsLess(unit.id, unit_id))
                            continue;
                    }
                    unit_id = unit.id;
                    res = &unit;
                }
            }
        }
        return res;
    }

    void GetCastUnits(const LogicalId _id, const Number& val, std::vector<Unit>& _cast_units)
    {
        if (val.unit.IsEmpty())
            return;

        GetCastUnits(_id, val, U"SI", _cast_units);
        GetCastUnits(_id, val, U"rus", _cast_units);
        std::sort(_cast_units.begin(), _cast_units.end());
    }

    void GetCastUnits(const LogicalId _id, const Number& val, const std::u32string& system, std::vector<Unit>& _cast_units)
    {
        auto it = cast_units.find(system);
        if (it != cast_units.end())
        {
            auto& v = it->second;
            for (auto& n : v)
                _cast_units.push_back(n.unit);
            return;
        }

        GetCastUnitsImpl(_id, val, system, _cast_units);
    }

    void GetCastUnits(const LogicalId _id, const Number& val, const std::u32string& system, std::vector<Number>& _cast_units) const
    {
        auto it = cast_units.find(system);
        if (it != cast_units.end())
        {
            _cast_units = it->second;
            return;
        }

        GetCastUnitsImpl(_id, val, system, _cast_units);

        cast_units[system] = _cast_units;
    }

    void GetCastUnitsImpl(const LogicalId _id, const Number& val, const std::u32string& system, std::vector<Unit>& _cast_units)
    {
        if (val.unit.IsEmpty())
            return;

        CheckBreak(parser_context);

        if (val.unit.system == system)
        {
            //add the cast unit if all of its parts have the same system
            size_t i = 0;
            for (; i < val.unit.unit.size(); ++i)
            {
                auto& u = val.unit.unit[i];
                if (system == U"SI" && FindBuiltinUnit(u.first))
                    continue;
                if (FindUnit(u.first, system) == nullptr)
                    break;
            }
            if (i == val.unit.unit.size())
            {
                Unit _unit = val.unit;
                _unit.Sort();
                _cast_units.push_back(_unit);
            }
        }

        for (size_t i = 0; i < symbols->units.size(); ++i)
        {
            CustomUnit<Number>& custom_unit = symbols->units[i];
            if (!custom_unit.buildin && !IsLess(custom_unit.id, _id))
                continue;
            Number t = val;
            if (custom_unit.system == system)
            {
                if (custom_unit.Cast(t))
                {
                    if (system != U"SI")
                    {
                        //check all the unit parts are present in this system
                        size_t j = 0;
                        for (; j < t.unit.unit.size(); ++j)
                        {
                            std::pair<std::u32string, int>& p = t.unit.unit[j];
                            if (std::find_if(symbols->units.begin(), symbols->units.end(), 
                                [system, p](const CustomUnit<Number>& c)
                                {
                                    return c.system == system && c.name == p.first;
                                }
                                ) == symbols->units.end())
                            {
                                break;
                            }
                        }
                        if (j != t.unit.unit.size())
                            continue;
                    }
                    
                    if (t.unit.unit.size() <= max_cast_unit_size)
                        GetCastUnitsImpl(_id, t, system, _cast_units);
                    else
                    {
                        Unit _unit = t.unit;
                        _unit.Sort();
                        _cast_units.push_back(_unit);
                    }
                }
            }
        }
    }

    void GetCastUnitsImpl(const LogicalId _id, const Number& val, const std::u32string& system, std::vector<Number>& _cast_units) const
    {
        if (val.unit.IsEmpty())
            return;

        CheckBreak(parser_context);

        if (val.unit.system == system)
            _cast_units.push_back(val);
        for (size_t i = 0; i < symbols->units.size(); ++i)
        {
            CustomUnit<Number>& custom_unit = symbols->units[i];
            if (!custom_unit.buildin && !IsLess(custom_unit.id, _id))
                continue;
            if (custom_unit.system == system)
            {
                Number t = val;
                if (custom_unit.Cast(t))
                {
                    if (t.unit.unit.size() <= max_cast_unit_size)
                        GetCastUnitsImpl(_id, t, system, _cast_units);
                    else
                        _cast_units.push_back(t);
                }
            }
        }
    }

    Number GetSuitableUnit(const LogicalId _id, const Number& val, const std::u32string& system, const bool buildin) const;

    Number CastToUnit(const LogicalId id, const Number& val, const Unit& unit) const;

    void ClearCastUnits()
    {
        cast_units.clear();
    }

    void ListBuiltinVariables(std::vector<std::u32string>& variables)
    {
        for (auto& v : symbols->buildin_variables)
            variables.push_back(v.first);
    }

    void ListBuiltinFunctions(std::vector<std::u32string>& functions)
    {
        for (auto& p : symbols->buildin_functions)
            functions.push_back(p.first);
        for (auto& p : symbols->trigonometric_functions)
            functions.push_back(p.first);
    }

    void ListUserVariables(std::vector<std::pair<std::u32string, std::u32string>>& variables)
    {
        for (auto& var : symbols->variables)
            variables.push_back(std::make_pair(var.name.name, var.name.description));
    }

    void ListUserFunctions(std::vector<std::u32string>& functions)
    {
        for (auto& func : symbols->functions)
            functions.push_back(func.name.name);
    }

    void ListBuiltinUnits(std::vector<std::pair<std::u32string, std::u32string>>& units)
    {
        for (auto& u : symbols->buildin_units)
            units.push_back(std::make_pair(u.ToString(false), u.description));
    }

    void ListUserUnits(std::vector<CustomUnit<Number>>& units)
    {
        for (auto& u : symbols->buildin_units)
        {
            CustomUnit<Number> c(LogicalId{0, 0, 1}, u.unit[0].first, u.system, Number(precision, u), true);
            c.description = u.description;
            units.push_back(c);
        }
        units.insert(units.end(), symbols->units.begin(), symbols->units.end());
    }

    void SetDependencies(Dependencies* _dependencies)
    {
        dependencies = _dependencies;
    }

    void SetDefaultNotation(Notation notation)
    {
        switch (notation)
        {
        case Notation::Binary:
            default_notation = 2;
            break;
        case Notation::Octal:
            default_notation = 8;
            break;
        case Notation::Decimal:
            default_notation = 10;
            break;
        case Notation::Hexadecimal:
            default_notation = 16;
            break;
        default:
            throw MathException(ParserExceptionCode::ArgumentIsOver);
        }
    }

    void SetMaxCastUnitSize(int _max_cast_unit_size)
    {
        max_cast_unit_size = _max_cast_unit_size;
    }

    void SetParserContext(ParserContext* _context)
    {
        parser_context = _context;
    }

public:
    ParserContext* parser_context = nullptr;

    mutable std::u32string im;
    mutable int res_pos = 0;

private:
    Number GetSuitableUnitImpl(const LogicalId _id, const Number& val, const std::u32string& system, const bool buildin) const
    {
        if (val.unit.IsEmpty())
            return val;
        
        //check all the custom units to be suitable for the current one
        Number res = val;
        int m = val.ToString(10, 10, false).length();
        int s = val.unit.unit.size();
        int p1 = val.unit.GetPower();

        std::vector<Number> _cast_units;
        auto it = cast_units.find(system);
        if (it != cast_units.end())
            _cast_units = it->second;
        else
            GetCastUnits(_id, val, system, _cast_units);

        for (Number& c : _cast_units)
        {
            CheckBreak(parser_context);

            if (c.unit.unit.size() <= s) //a unit should have minimal size
            {
                size_t m2 = c.ToString(abs(c.GetExp()) + 1, abs(c.GetExp()) + 1, false).length();
                if (c.unit.unit.size() == 1 && c.unit.unit[0].second == 1) //choose the simplest one
                {
                    if (s > 1 || m2 < m || res.unit.unit[0].second < 1 || (m2 == m && (c < res || res < 1) && c >= 1))
                    {
                        res = c;
                        m = m2;
                        s = c.unit.unit.size();
                    }
                    continue;
                }

                //result string should have minimal length and should have the simpliest unit
                if (m2 > m)
                    continue;
                if (m2 == m)
                {
                    if (p1 < 0 && c.unit.GetPower() < 0)
                        continue;
                    if (c.unit.unit.size() > s)
                        continue;
                    int base_units1 = 0, base_units2 = 0;
                    for (auto& u : res.unit.unit)
                    {
                        if (FindBuiltinUnit(u.first))
                            ++base_units1;
                    }
                    for (auto& u : c.unit.unit)
                    {
                        if (FindBuiltinUnit(u.first))
                            ++base_units2;
                    }
                    if (res.unit.unit.size() == c.unit.unit.size())
                    {
                        if (base_units1 > base_units2)
                        {
                            if (c.unit.unit.size() == 1 && res.unit.unit[0].second < 0)
                            {
                                res = c;
                                m = m2;
                                s = c.unit.unit.size();
                            }
                            continue;
                        }
                        if ((res < 1 || c < 1) && 1 / res < 1 / c)
                            continue;
                    }
                }
                else
                {
                    if (res.unit.unit.size() == c.unit.unit.size())
                    {
                        if (res.unit.GetPower() > c.unit.GetPower())
                            continue;
                    }
                }

                if (res.unit.unit.size() == c.unit.unit.size())
                {
                    auto res_str = res.ToString(10, 10, false);
                    auto c_str = c.ToString(10, 10, false);
                    if (res_str == c_str)
                    {
                        //choose more convinient unit, which has to be before the current one
                        size_t i;
                        for (i = 0; i < c.unit.unit.size(); ++i)
                        {
                            auto& u1 = c.unit.unit[i];
                            auto& u2 = res.unit.unit[i];
                            auto* unit1 = FindUnit(u1.first, c.unit.system);
                            auto* unit2 = FindUnit(u2.first, res.unit.system);
                            if (unit1 && unit2 && IsLess(unit2->id, unit1->id))
                                break;
                        }
                        if (i < c.unit.unit.size())
                            continue;
                    }
                    else if (res_str.length() < c_str.length())
                        continue;
                }

                res = c;
                m = m2;
                s = c.unit.unit.size();
            }
        }

        cast_units[system] = _cast_units;

        return res;
    }

    Number CastToUnitImpl(const LogicalId id, const Number& val, const Unit& unit) const
    {
        if (val.unit == unit)
            return val;

        //cast to base units
        Number u(precision, 1);
        for (auto& p : unit.unit)
        {
            CheckBreak(parser_context);

            Number unit_val;
            if (unit.system == U"SI")
            {
                Unit* base_unit = FindBuiltinUnit(p.first);
                if (base_unit)
                    unit_val = Number(precision, *base_unit);
            }
            if (unit_val.unit.IsEmpty())
            {
                CustomUnit<Number>* custom_unit = FindUnit(p.first, unit.system);
                if (!custom_unit)
                    throw MathException(id, ParserExceptionCode::UnknownIdentifier);
                unit_val = custom_unit->value;
            }

            if (p.second == 1)
                u *= unit_val;
            else if (p.second == -1)
                u /= unit_val;
            else if (p.second > 1)
                u *= pow(unit_val, p.second);
            else
                u /= pow(unit_val, -p.second);
        }

        if (u.unit != val.unit)
            throw MathException(id, ParserExceptionCode::CannotCastToUnit);

        Number res = val / u;
        res.unit = unit;
        return res;
    }

    void AddDependency(const std::u32string& name) const
    {
        if (std::find(dependencies->begin(), dependencies->end(), name) == dependencies->end())
            dependencies->push_back(name);
    }

    Number FunctionCall(FunctionCallNode<Number> const& op) const
    {
        CheckBreak(parser_context);

        AddDependency(op.name.name);

        Number res;
        
        try
        {
            //find in the user defined functions
            FunctionNode<Number>* user_func = FindFunction(op);
            if (user_func)
            {
                IdentifierNodesIter funcIter = user_func->arguments.begin();
                for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
                {
                    Number arg = (*this)(*iter);
                    PushTempVariable(funcIter->name, arg);
                }

                res = (*this)(user_func->return_expression);
                PopTempVariables(op.arguments.size());
                return res;
            }

            BuiltinTrigonometricFunction* t_func = FindTrigonometricFunction(op.name.name);
            if (t_func)
            {
                try
                {
                    TrigonometricFunction u = boost::get<TrigonometricFunction>(*t_func);
                    if (op.arguments.size() != 1)
                        throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
                    
                    ExpressionNodesIter iter = op.arguments.begin();
                    Number arg = (*this)(*iter);
                    if (arg.GetAngleMeasure() == AngleMeasure::None)
                        arg.SetAngleMeasure(default_angle_measure);
                    return (*u)(arg);
                }
                catch (boost::bad_get)
                {
                }

                try
                {
                    ComplexTrigonometricFunction u = boost::get<ComplexTrigonometricFunction>(*t_func);
                    if (op.arguments.size() != 1)
                        throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
                    
                    ExpressionNodesIter iter = op.arguments.begin();
                    Number arg = (*this)(*iter);
                    if (arg.GetAngleMeasure() == AngleMeasure::None)
                        arg.SetAngleMeasure(default_angle_measure);
                    return (*u)(arg, res_pos);
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
                    Number arg = (*this)(*iter);
                    return (*u)(arg);
                }
                catch (boost::bad_get)
                {
                }

                try
                {
                    ComplexUnaryFunction u = boost::get<ComplexUnaryFunction>(*func);
                    if (op.arguments.size() != 1)
                        throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
                    
                    ExpressionNodesIter iter = op.arguments.begin();
                    Number arg = (*this)(*iter);
                    if (arg.GetAngleMeasure() == AngleMeasure::None)
                        arg.SetAngleMeasure(default_angle_measure);
                    return (*u)(arg, res_pos);
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
                    Number arg1 = (*this)(*iter++);
                    Number arg2 = (*this)(*iter);
                    return (*b)(arg1, arg2);
                }
                catch (boost::bad_get)
                {
                }

                try
                {
                    ComplexBinaryFunction b = boost::get<ComplexBinaryFunction>(*func);
                    if (op.arguments.size() != 2)
                        throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.line);
                    
                    ExpressionNodesIter iter = op.arguments.begin();
                    Number arg1 = (*this)(*iter++);
                    Number arg2 = (*this)(*iter);
                    return (*b)(arg1, arg2, res_pos);
                }
                catch (boost::bad_get)
                {
                }
            }
        }
        catch (const MathException& e)
        {
            throw MathException(op.id, e.ex_id, op.pos, op.line);
        }

        //there is no such a function		
        throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.line);
        
        return res;
    }

    Number NoFencesFunctionCall(NoFencesFunctionCallNode<Number> const& op) const
    {
        AddDependency(op.name.name);

        Number res;
        
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
                Number arg1 = (*this)(*iter++);
                Number arg2 = (*this)(op.last_argument);
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

    friend class Expression<Number>;

    mutable LogicalId id;
    mutable int precision;
    mutable AngleMeasure default_angle_measure;
    mutable AngleMeasure result_angle_measure;
    mutable int default_notation = 10;
    Number left_value; //left solved value
    mutable Dependencies* dependencies = nullptr;
    mutable std::map<std::u32string, std::vector<Number>> cast_units;

    int max_cast_unit_size = 2; //max size of each unit in the cast vector
};

};

#endif
