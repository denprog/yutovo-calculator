#include "export.h"

namespace yutovo_calculator
{

//Export

template<>
void Export::AddVariable<Integer>(const VariableNode<Integer>& var)
{
    auto it = std::find_if(variables_integer.begin(), variables_integer.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_integer.end())
    {
        variables_integer.push_back(var);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Real>(const VariableNode<Real>& var)
{
    auto it = std::find_if(variables_real.begin(), variables_real.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_real.end())
    {
        variables_real.push_back(var);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Rational>(const VariableNode<Rational>& var)
{
    auto it = std::find_if(variables_rational.begin(), variables_rational.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_rational.end())
    {
        variables_rational.push_back(var);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Complex>(const VariableNode<Complex>& var)
{
    auto it = std::find_if(variables_complex.begin(), variables_complex.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_complex.end())
    {
        variables_complex.push_back(var);
        return;
    }
    *it = var;
}

template<>
void Export::AddFunction(const FunctionNode<Integer>& func)
{
    auto it = std::find_if(functions_integer.begin(), functions_integer.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_integer.end())
    {
        functions_integer.push_back(func);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Real>& func)
{
    auto it = std::find_if(functions_real.begin(), functions_real.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_real.end())
    {
        functions_real.push_back(func);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Rational>& func)
{
    auto it = std::find_if(functions_rational.begin(), functions_rational.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_rational.end())
    {
        functions_rational.push_back(func);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Complex>& func)
{
    auto it = std::find_if(functions_complex.begin(), functions_complex.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_complex.end())
    {
        functions_complex.push_back(func);
        return;
    }
    *it = func;
}

template<>
void Export::AddUnit(const CustomUnit<Integer>& unit)
{
}

template<>
void Export::AddUnit(const CustomUnit<Real>& unit)
{
    auto it = std::find_if(units_real.begin(), units_real.end(), 
        [unit](auto& v)
        {
            return unit.name == v.name;
        });
    if (it == units_real.end())
    {
        units_real.push_back(unit);
        return;
    }
    *it = unit;
}

template<>
void Export::AddUnit(const CustomUnit<Rational>& unit)
{
    auto it = std::find_if(units_rational.begin(), units_rational.end(), 
        [unit](auto& v)
        {
            return unit.name == v.name;
        });
    if (it == units_rational.end())
    {
        units_rational.push_back(unit);
        return;
    }
    *it = unit;
}

template<>
void Export::AddUnit(const CustomUnit<Complex>& unit)
{
}

}
