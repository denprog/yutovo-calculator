/*
 * Yutovo Calculator
 * Copyright (C) 2022-2025 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "export.h"

namespace yutovo_calculator
{

//Export

template<>
void Export::AddVariable<Integer>(const VariableNode<Integer>& var)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(variables_integer.begin(), variables_integer.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_integer.end())
    {
        VariableNode<Integer> v(var);
        v.exported = true;
        variables_integer.push_back(v);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Real>(const VariableNode<Real>& var)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(variables_real.begin(), variables_real.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_real.end())
    {
        VariableNode<Real> v(var);
        v.exported = true;
        variables_real.push_back(v);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Rational>(const VariableNode<Rational>& var)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(variables_rational.begin(), variables_rational.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_rational.end())
    {
        VariableNode<Rational> v(var);
        v.exported = true;
        variables_rational.push_back(v);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Complex>(const VariableNode<Complex>& var)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(variables_complex.begin(), variables_complex.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_complex.end())
    {
        VariableNode<Complex> v(var);
        v.exported = true;
        variables_complex.push_back(v);
        return;
    }
    *it = var;
}

template<>
void Export::AddVariable<Array<Real>>(const VariableNode<Array<Real>>& var)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(variables_array_real.begin(), variables_array_real.end(), 
        [var](auto& v)
        {
            return var.name == v.name;
        });
    if (it == variables_array_real.end())
    {
        VariableNode<Array<Real>> v(var);
        v.exported = true;
        variables_array_real.push_back(v);
        return;
    }
    *it = var;
}

template<>
void Export::AddFunction(const FunctionNode<Integer>& func)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(functions_integer.begin(), functions_integer.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_integer.end())
    {
        FunctionNode<Integer> f(func);
        f.exported = true;
        functions_integer.push_back(f);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Real>& func)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(functions_real.begin(), functions_real.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_real.end())
    {
        FunctionNode<Real> f(func);
        f.exported = true;
        functions_real.push_back(f);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Rational>& func)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(functions_rational.begin(), functions_rational.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_rational.end())
    {
        FunctionNode<Rational> f(func);
        f.exported = true;
        functions_rational.push_back(f);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Complex>& func)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(functions_complex.begin(), functions_complex.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_complex.end())
    {
        FunctionNode<Complex> f(func);
        f.exported = true;
        functions_complex.push_back(f);
        return;
    }
    *it = func;
}

template<>
void Export::AddFunction(const FunctionNode<Array<Real>>& func)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(functions_array_real.begin(), functions_array_real.end(), 
        [func](auto& v)
        {
            return func.name == v.name;
        });
    if (it == functions_array_real.end())
    {
        FunctionNode<Array<Real>> f(func);
        f.exported = true;
        functions_array_real.push_back(f);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
void Export::AddUnit(const CustomUnit<Array<Real>>& unit)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    auto it = std::find_if(units_array_real.begin(), units_array_real.end(), 
        [unit](auto& v)
        {
            return unit.name == v.name;
        });
    if (it == units_array_real.end())
    {
        units_array_real.push_back(unit);
        return;
    }
    *it = unit;
}

template<>
VariableNode<Integer>* Export::FindVariable<Integer>(const std::u32string& name, const std::u32string& subscript)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& var : variables_complex)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
VariableNode<Array<Real>>* Export::FindVariable<Array<Real>>(const std::u32string& name, const std::u32string& subscript)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& var : variables_array_real)
    {
        if (var.name.name == name && var.name.subscript == subscript)
            return &var;
    }
    return nullptr;
}

template<>
FunctionNode<Integer>* Export::FindFunction<Integer>(const std::u32string& name)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& func : functions_complex)
    {
        if (func.name.name == name)
            return &func;
    }
    return nullptr;
}

template<>
FunctionNode<Array<Real>>* Export::FindFunction<Array<Real>>(const std::u32string& name)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& func : functions_array_real)
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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
CustomUnit<Array<Real>>* Export::FindUnit<Array<Real>>(const std::u32string& name, const std::u32string& system)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& unit : units_array_real)
    {
        if (unit.name == name && (unit.system == system || (unit.system == U"SI" && system.empty())))
            return &unit;
    }
    return nullptr;
}

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Real>>& units)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
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
    std::scoped_lock<std::mutex> lock(export_mutex);
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

template<>
void Export::GetUnits(const std::u32string& system, std::vector<CustomUnit<Array<Real>>>& units)
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    for (auto& unit : units_array_real)
    {
        if (unit.system == system)
            units.push_back(unit);
    }
}

void Export::Clear()
{
    std::scoped_lock<std::mutex> lock(export_mutex);
    variables_integer.clear();
    variables_real.clear();
    variables_rational.clear();
    variables_complex.clear();
    variables_array_real.clear();

    functions_integer.clear();
    functions_real.clear();
    functions_rational.clear();
    functions_complex.clear();
    functions_array_real.clear();

    units_real.clear();
    units_rational.clear();
    units_array_real.clear();
}

}
