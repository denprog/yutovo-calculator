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

template<>
VariableNode<Integer>* Export::FindVariable<Integer>(const std::u32string& name, const std::u32string& subscript)
{
    for (auto& var : variables_integer)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
VariableNode<Real>* Export::FindVariable<Real>(const std::u32string& name, const std::u32string& subscript)
{
    for (auto& var : variables_real)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
VariableNode<Rational>* Export::FindVariable<Rational>(const std::u32string& name, const std::u32string& subscript)
{
    for (auto& var : variables_rational)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
VariableNode<Complex>* Export::FindVariable<Complex>(const std::u32string& name, const std::u32string& subscript)
{
    for (auto& var : variables_complex)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
FunctionNode<Integer>* Export::FindFunction<Integer>(const std::u32string& name)
{
    for (auto& func : functions_integer)
    {
        if (func.name.name == name)
            return &func;
    }
    return nullptr;
}

template<>
FunctionNode<Real>* Export::FindFunction<Real>(const std::u32string& name)
{
    for (auto& func : functions_real)
    {
        if (func.name.name == name)
            return &func;
    }
    return nullptr;
}

template<>
FunctionNode<Rational>* Export::FindFunction<Rational>(const std::u32string& name)
{
    for (auto& func : functions_rational)
    {
        if (func.name.name == name)
            return &func;
    }
    return nullptr;
}

template<>
FunctionNode<Complex>* Export::FindFunction<Complex>(const std::u32string& name)
{
    for (auto& func : functions_complex)
    {
        if (func.name.name == name)
            return &func;
    }
    return nullptr;
}

template<>
CustomUnit<Integer>* Export::FindUnit<Integer>(const std::u32string& name, const std::u32string& system)
{
    return nullptr;
}

template<>
CustomUnit<Real>* Export::FindUnit<Real>(const std::u32string& name, const std::u32string& system)
{
    for (auto& unit : units_real)
    {
        if (unit.name == name && (unit.system == system || (unit.system == U"SI" && system.empty())))
            return &unit;
    }
    return nullptr;
}

template<>
CustomUnit<Rational>* Export::FindUnit<Rational>(const std::u32string& name, const std::u32string& system)
{
    for (auto& unit : units_rational)
    {
        if (unit.name == name && (unit.system == system || (unit.system == U"SI" && system.empty())))
            return &unit;
    }
    return nullptr;
}

template<>
CustomUnit<Complex>* Export::FindUnit<Complex>(const std::u32string& name, const std::u32string& system)
{
    return nullptr;
}

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Real>>& units)
{
    for (auto& unit : units_real)
    {
        if (unit.system == system)
            units.push_back(unit);
    }
}

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Integer>>& units)
{
}

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Rational>>& units)
{
    for (auto& unit : units_rational)
    {
        if (unit.system == system)
            units.push_back(unit);
    }
}

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Complex>>& units)
{
}

void Export::Clear()
{
    variables_integer.clear();
    variables_real.clear();
    variables_rational.clear();
    variables_complex.clear();

    functions_integer.clear();
    functions_real.clear();
    functions_rational.clear();
    functions_complex.clear();

    units_real.clear();
    units_rational.clear();
}

}
