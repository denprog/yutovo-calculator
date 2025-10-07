/*
 * Yutovo Calculator
 * Copyright (C) 2022-2025 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "stdafx.h"
#include "solver.h"

namespace yutovo_calculator
{

//Solver

template<>
Integer Solver<Integer>::operator()(NumberNode<Integer> const& op) const
{
    return Integer(op.number, default_notation);
}

template<>
Real Solver<Real>::operator()(NumberNode<Real> const& op) const
{
    NumberNode<Real> _op = op;
    return Real(_op.number);
}

template<>
Rational Solver<Rational>::operator()(NumberNode<Rational> const& op) const
{
    NumberNode<Rational> _op = op;
    auto p = _op.number.find(U'.');
    if (p != std::string::npos)
    {
        std::u32string d = U"1";
        for (int i = 0; i < _op.number.length() - p - 1; ++i)
            d += U"0";
        return Rational(_op.number.substr(0, p)) + Rational(_op.number.substr(p + 1, _op.number.length() - p - 1)) / Rational(d);
    }
    return Rational(_op.number);
}

template<>
Complex Solver<Complex>::operator()(NumberNode<Complex> const& op) const
{
    NumberNode<Complex> _op = op;
    return Complex(_op.number);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(NumberNode<Array<Real>> const& op) const
{
    NumberNode<Array<Real>> _op = op;
    return Array<Real>(_op.number);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(LineGraphNode<Array<Real>> const& op) const
{
    Array<Real> res;
    Array<Real> x;
    PushTempVariable(op.identifier.name, x);
    Array<Real> x_right, y_bottom, y_top;
    Array<Real> inc;
    Array<Real> start_pos, end_pos;
    try
    {
        x = (*this)(op.x_left);
        SetTempVariable(op.identifier.name, x);
        x_right = (*this)(op.x_right);
        y_bottom = (*this)(op.y_bottom);
        y_top = (*this)(op.y_top);
        start_pos = (*this)(op.start_pos);
        end_pos = (*this)(op.end_pos);
        inc = (x_right - x) / ((*this)(op.points_count));
    }
    catch (...)
    {
        PopTempVariables(1);
        throw;
    }

    //the first items are bounds of the graph
    res.Add(x);
    res.Add(x_right);
    res.Add(y_bottom);
    res.Add(y_top);

    //next items are points of the graph x, y
    while (x < x_right)
    {
        try
        {
            Array<Real> y = (*this)(op.expression);
            res.Add(x);
            res.Add(y);
            x += inc;
            SetTempVariable(op.identifier.name, x);
        }
        catch (TimeExceedException)
        {
            PopTempVariables(1);
            throw;
        }
        catch (BreakException)
        {
            PopTempVariables(1);
            throw;
        }
        catch (SyntaxException)
        {
            PopTempVariables(1);
            throw;
        }
        catch (...)
        {
            Real nan;
            nan.SetNaN();
            res.Add(x);
            res.Add(nan);
            x += inc;
            SetTempVariable(op.identifier.name, x);
        }
    }
    PopTempVariables(1);
    return res;
}

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
Array<Real> Solver<Array<Real>>::operator()(UnitNode<Array<Real>> const& op) const
{
    //store the unit
    AddUnit(op);
    return Array<Real>();
}

template<>
Integer Solver<Integer>::operator()(UnitNode<Integer> const& op) const
{
    throw SyntaxException(op.id, SyntaxError, U"Identifier '" + op.name.name + U"' not found", op.pos, op.name.name.length(), op.line);
}

// template<>
// Array<Real> Solver<Array<Real>>::operator()(UnitNode<Array<Real>> const& op) const
// {
//     //store the unit
//     AddUnit(op);
//     return Array<Real>();
// }

template<>
Integer Solver<Integer>::operator()(UnaryOperationNode<Integer> const& op) const
{
    CheckBreak(parser_context);

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
    throw SyntaxException(op.id, SyntaxError, op.pos, 1, op.line);
}

template<>
Real Solver<Real>::operator()(UnaryOperationNode<Real> const& op) const
{
    CheckBreak(parser_context);

    Real right = boost::apply_visitor(*this, op.operand);
    switch (op.op)
    {
    case '+':
        return right;
    case '-':
        return -right;
    }
    throw SyntaxException(op.id, SyntaxError, op.pos, 1, op.line);
}

template<>
Rational Solver<Rational>::operator()(UnaryOperationNode<Rational> const& op) const
{
    CheckBreak(parser_context);

    Rational right = boost::apply_visitor(*this, op.operand);
    switch (op.op)
    {
    case '+':
        return right;
    case '-':
        return -right;
    }
    throw SyntaxException(op.id, SyntaxError, op.pos, 1, op.line);
}

template<>
Complex Solver<Complex>::operator()(UnaryOperationNode<Complex> const& op) const
{
    CheckBreak(parser_context);

    Complex right = boost::apply_visitor(*this, op.operand);
    switch (op.op)
    {
    case '+':
        return right;
    case '-':
        return -right;
    }
    throw SyntaxException(op.id, SyntaxError, op.pos, 1, op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(UnaryOperationNode<Array<Real>> const& op) const
{
    CheckBreak(parser_context);

    Array<Real> right = boost::apply_visitor(*this, op.operand);
    switch (op.op)
    {
    case '+':
        return right;
    case '-':
        return -right;
    }
    throw SyntaxException(op.id, SyntaxError, op.pos, 1, op.line);
}

template<>
Integer Solver<Integer>::operator()(OperationNode<Integer> const& op) const
{
    CheckBreak(parser_context);

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
    CheckBreak(parser_context);

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
        case '%':
            return left_value * (float)0.01 * right;
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
    CheckBreak(parser_context);

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
        case '%':
            return left_value / 100 * right;
        }
    }
    catch (MathException e)
    {
        throw MathException(e.id, e.ex_id, op.pos, op.line);
    }
    
    throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Complex Solver<Complex>::operator()(OperationNode<Complex> const& op) const
{
    CheckBreak(parser_context);

    Complex right = boost::apply_visitor(*this, op.operand);
    
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
        case '%':
            return left_value * (float)0.01 * right;
        }
    }
    catch (MathException e)
    {
        throw MathException(e.id, e.ex_id, op.pos, op.line);
    }
    
    throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(OperationNode<Array<Real>> const& op) const
{
    Array<Real> right = boost::apply_visitor(*this, op.operand);
    
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
        case '%':
            return left_value * (float)0.01 * right;
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
Complex Solver<Complex>::operator()(PostfixOperationNode<Complex> const& op) const
{
    Complex left = boost::apply_visitor(*this, op.operand);
    switch (op.op)
    {
    case '!':
        return fact(left);
    }
    throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Integer Solver<Integer>::operator()(FunctionCallNode<Integer> const& op) const
{
    AddDependency(op.name);

    Integer res;
    
    try
    {
        //find in the user defined functions
        FunctionNode<Integer>* user_func = FindFunction(op);
        if (user_func)
        {
            IdentifierNodesIter funcIter = user_func->arguments.begin();
            for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
            {
                CheckBreak(parser_context);

                Integer arg = (*this)(*iter);
                PushTempVariable(funcIter->name, arg);
            }

            bool _exported_id = exported_id;
            exported_id = user_func->exported;
            res = (*this)(user_func->return_expression);
            exported_id = _exported_id;
            PopTempVariables(op.arguments.size());
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
                    throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.name.name.length(), op.line);
                
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
                    throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.name.name.length(), op.line);
                
                ExpressionNodesIter iter = op.arguments.begin();
                Integer arg1 = (*this)(*iter++);
                Integer arg2 = (*this)(*iter);
                return (*b)(arg1, arg2);
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
    throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.name.name.length(), op.line);
    
    return res;
}

template<>
Real Solver<Real>::operator()(FunctionCallNode<Real> const& op) const
{
    return FunctionCall(op);
}

template<>
Rational Solver<Rational>::operator()(FunctionCallNode<Rational> const& op) const
{
    AddDependency(op.name);

    Rational res;
    
    try
    {
        //find in the user defined functions
        FunctionNode<Rational>* user_func = FindFunction(op);
        if (user_func)
        {
            IdentifierNodesIter funcIter = user_func->arguments.begin();
            for (ExpressionNodesIter iter = op.arguments.begin(); iter != op.arguments.end(); ++iter, ++funcIter)
            {
                CheckBreak(parser_context);

                Rational arg = (*this)(*iter);
                PushTempVariable(funcIter->name, arg);
            }

            bool _exported_id = exported_id;
            exported_id = user_func->exported;
            res = (*this)(user_func->return_expression);
            exported_id = _exported_id;
            PopTempVariables(op.arguments.size());
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
                    throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.name.name.length(), op.line);
                
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
                    throw SyntaxException(op.id, WrongArgumentsCount, U"Wrong arguments count in '" + op.name.name + U"'", op.pos, op.name.name.length(), op.line);
                
                ExpressionNodesIter iter = op.arguments.begin();
                Rational arg1 = (*this)(*iter++);
                Rational arg2 = (*this)(*iter);
                return (*b)(arg1, arg2);
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
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name.name + U"' not found", op.pos, op.name.name.length(), op.line);
    
    return res;
}

template<>
Complex Solver<Complex>::operator()(FunctionCallNode<Complex> const& op) const
{
    return FunctionCall(op);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(FunctionCallNode<Array<Real>> const& op) const
{
    return FunctionCall(op);
}

template<>
Integer Solver<Integer>::operator()(FunctionCallStringNode<Integer> const& op) const
{
    AddDependency(op.name);

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
    throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.name.name.length(), op.line);
    
    return res;
}

template<>
Real Solver<Real>::operator()(FunctionCallStringNode<Real> const& op) const
{
    //there is no such a function		
    throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.name.name.length(), op.line);
}

template<>
Rational Solver<Rational>::operator()(FunctionCallStringNode<Rational> const& op) const
{
    //there is no such a function		
    throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.name.name.length(), op.line);
}

template<>
Complex Solver<Complex>::operator()(FunctionCallStringNode<Complex> const& op) const
{
    //there is no such a function		
    throw SyntaxException(op.id, UnknownIdentifier, op.pos, op.name.name.length(), op.line);
}

template<>
VariableNode<Array<Real>>* Solver<Array<Real>>::FindVariable(const std::u32string& name, const std::u32string& subscript) const
{
    for (int i = (int)symbols->variables.size() - 1; i >= 0; --i)
    {
        auto& var = symbols->variables[i];
        if (IsLess(var.id, id) && var.name.name == name)
        {
            if (var.name.subscript == subscript)
                return &var;
            if (!var.name.subscript.empty())
                continue;
            return &var;
        }
    }

    if (FindTempVariable(subscript))
        return FindExportVariable(name, U"");
    return FindExportVariable(name, subscript);
}

template<>
Integer Solver<Integer>::operator()(NoFencesFunctionCallNode<Integer> const& op) const
{
    AddDependency(op.name);

    Integer res;
    return res;
}

template<>
Real Solver<Real>::operator()(NoFencesFunctionCallNode<Real> const& op) const
{
    return NoFencesFunctionCall(op);
}

template<>
Rational Solver<Rational>::operator()(NoFencesFunctionCallNode<Rational> const& op) const
{
    AddDependency(op.name);

    Rational res;
    return res;
}

template<>
Complex Solver<Complex>::operator()(NoFencesFunctionCallNode<Complex> const& op) const
{
    return NoFencesFunctionCall(op);
}

template<>
Integer Solver<Integer>::operator()(IdentifierNode<Integer> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op);

    TempVariable* t = FindTempVariable(op.name);
    if (t)
        return t->second;
    
    //find in user defined variables
    VariableNode<Integer>* v = FindVariable(op.name, op.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Integer res = (*this)(v->expression);
        id = _id;
        exported_id = _exported_id;
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
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.name.length(), op.line);
    
    return 0;
}

template<>
Real Solver<Real>::operator()(IdentifierNode<Real> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op);

    TempVariable* t = FindTempVariable(op.name);
    if (t)
        return t->second;
    
    //find in the user defined variables
    VariableNode<Real>* v = FindVariable(op.name, op.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Real res = (*this)(v->expression);
        id = _id;
        exported_id = _exported_id;
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
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.name.length(), op.line);
}

template<>
Rational Solver<Rational>::operator()(IdentifierNode<Rational> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op);

    TempVariable* t = FindTempVariable(op.name);
    if (t)
        return t->second;
    
    //find in user defined variables
    VariableNode<Rational>* v = FindVariable(op.name, op.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Rational res = (*this)(v->expression);
        id = _id;
        exported_id = _exported_id;
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
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.name.length(), op.line);
}

template<>
Complex Solver<Complex>::operator()(IdentifierNode<Complex> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op);

    TempVariable* t = FindTempVariable(op.name);
    if (t)
        return t->second;
    
    //find in the user defined variables
    VariableNode<Complex>* v = FindVariable(op.name, op.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Complex res = (*this)(v->expression);
        id = _id;
        exported_id = _exported_id;
        return res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.name);
    if (var)
    {
        try
        {
            ComplexPrecisionVariable v = boost::get<ComplexPrecisionVariable>(*var);
            return (*v)(precision, default_angle_measure);
        }
        catch (boost::bad_get)
        {
        }
    }

    if (op.name == im)
        return Complex(MathHelper::ToBitPrecision(precision), 0, 1);

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.name.length(), op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(IdentifierNode<Array<Real>> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op);

    TempVariable* t = FindTempVariable(op.name);
    if (t)
        return t->second;
    
    //find in the user defined variables
    VariableNode<Array<Real>>* v = FindVariable(op.name, op.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Array<Real> res = (*this)(v->expression);
        if (!op.subscript.empty() && v->name.subscript != op.subscript)
        {
            VariableNode<Array<Real>>* s = FindVariable(op.subscript, U"");
            if (s)
            {
                Array<Real> s_res = (*this)(s->expression);
                if (s_res.Size() == 1)
                {
                    int index = (int)(s_res.Get(0));
                    if (res.Size() > index && index >= 0)
                    {
                        Real r = res.Get(index);
                        res.Clear();
                        res.Add(r);
                    }
                    else
                        throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
                }
                else
                    throw SyntaxException(op.id, IncorrectOperation, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
            }
            else
            {
                TempVariable* t = FindTempVariable(op.subscript);
                if (t)
                {
                    Array<Real> s_res = t->second;
                    if (s_res.Size() == 1)
                    {
                        int index = (int)(s_res.Get(0));
                        if (res.Size() > index && index >= 0)
                        {
                            Real r = res.Get(index);
                            res.Clear();
                            res.Add(r);
                        }
                        else
                            throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
                    }
                    else
                        throw SyntaxException(op.id, IncorrectOperation, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
                }
                else
                {
                    try
                    {
                        int index = FromUtfString(op.subscript);
                        if (res.Size() > index && index >= 0)
                        {
                            Real r = res.Get(index);
                            res.Clear();
                            res.Add(r);
                        }
                        else
                            throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
                    }
                    catch (const std::exception& e)
                    {
                        throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.name + U"' is over", op.pos, op.name.length(), op.line);
                    }
                }
            }
        }
        id = _id;
        exported_id = _exported_id;
        return res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.name);
    if (var)
    {
        try
        {
            ArrayRealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
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
            return Array<Real>(precision, *unit);
        }
    }

    auto* custom_unit = FindUnit(op.name, op.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        return custom_unit->value;
    }

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.name + U"' not found", op.pos, op.name.length(), op.line);
}

template<>
Integer Solver<Integer>::operator()(ImplicitStringMulNode<Integer> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
        return (*this)(op.left) * t->second;
    
    //find in user defined variables
    VariableNode<Integer>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Integer res = (*this)(v->expression);
        id = _id;
        return (*this)(op.left) * res;
    }
    
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            IntegerVariable v = boost::get<Variable>(*var);
            return (*this)(op.left) * (*v)();
        }
        catch (boost::bad_get)
        {
        }
    }
    
    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Real Solver<Real>::operator()(ImplicitStringMulNode<Real> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
        return (*this)(op.left) * t->second;
    
    //find in the user defined variables
    VariableNode<Real>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Real res = (*this)(v->expression);
        id = _id;
        return (*this)(op.left) * res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            RealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
            return (*this)(op.left) * (*v)(precision);
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
            return (*this)(op.left) * Real(precision, *unit);
        }
    }

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        return (*this)(op.left) * custom_unit->value;
    }

    Real val;
    if (FindBuiltinIdentifier(op.identifier.name, val))
        return (*this)(op.left) * val;

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Rational Solver<Rational>::operator()(ImplicitStringMulNode<Rational> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
        return (*this)(op.left) * t->second;
    
    //find in user defined variables
    VariableNode<Rational>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Rational res = (*this)(v->expression);
        id = _id;
        return (*this)(op.left) * res;
    }
    
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            RationalVariable v = boost::get<Variable>(*var);
            return (*this)(op.left) * (*v)();
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
            return (*this)(op.left) * Rational(*unit);
        }
    }

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        return (*this)(op.left) * custom_unit->value;
    }

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Complex Solver<Complex>::operator()(ImplicitStringMulNode<Complex> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
        return (*this)(op.left) * t->second;
    
    //find in the user defined variables
    VariableNode<Complex>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Complex res = (*this)(v->expression);
        id = _id;
        return (*this)(op.left) * res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            ComplexPrecisionVariable v = boost::get<ComplexPrecisionVariable>(*var);
            return (*this)(op.left) * (*v)(precision, default_angle_measure);
        }
        catch (boost::bad_get)
        {
        }
    }

    if (op.identifier.name == im)
        return (*this)(op.left) * Complex(MathHelper::ToBitPrecision(precision), 0, 1);

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(ImplicitStringMulNode<Array<Real>> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
        return (*this)(op.left) * t->second;
    
    //find in the user defined variables
    VariableNode<Array<Real>>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Array<Real> res = (*this)(v->expression);
        if (!op.identifier.subscript.empty() && v->name.subscript != op.identifier.subscript)
        {
            try
            {
                int index = FromUtfString(op.identifier.subscript);
                if (res.Size() > index && index >= 0)
                {
                    Real r = res.Get(index);
                    res.Clear();
                    res.Add(r);
                }
                else
                    throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.identifier.name + U"' is over", op.pos, op.identifier.name.length(), op.line);
            }
            catch (const std::exception& e)
            {
                throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.identifier.name + U"' is over", op.pos, op.identifier.name.length(), op.line);
            }
        }
        id = _id;
        return (*this)(op.left) * res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            ArrayRealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
            return (*this)(op.left) * (*v)(precision);
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
            return (*this)(op.left) * Array<Real>(precision, *unit);
        }
    }

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        return (*this)(op.left) * custom_unit->value;
    }

    Array<Real> val;
    if (FindBuiltinIdentifier(op.identifier.name, val))
        return (*this)(op.left) * val;

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Integer Solver<Integer>::operator()(ImplicitDivMulNode<Integer> const& op) const
{
    throw SyntaxException(op.id, SyntaxError, op.pos, op.line);
}

template<>
Real Solver<Real>::operator()(ImplicitDivMulNode<Real> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

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
        LogicalId _id = id;
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

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        Real arg1 = (*this)(op.upper);
        Real arg2 = (*this)(op.lower);
        return (arg1 / arg2) * custom_unit->value;
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

    Real val;
    if (FindBuiltinIdentifier(op.identifier.name, val))
    {
        Real arg1 = (*this)(op.upper);
        Real arg2 = (*this)(op.lower);
        return (arg1 / arg2) * val;
    }

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Rational Solver<Rational>::operator()(ImplicitDivMulNode<Rational> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

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
        LogicalId _id = id;
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

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        Rational arg1 = (*this)(op.upper);
        Rational arg2 = (*this)(op.lower);
        return (arg1 / arg2) * custom_unit->value;
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

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Complex Solver<Complex>::operator()(ImplicitDivMulNode<Complex> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
    {
        Complex arg1 = (*this)(op.upper);
        Complex arg2 = (*this)(op.lower);
        return (arg1 / arg2) * t->second;
    }
    
    //find in the user defined variables
    VariableNode<Complex>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        id = v->id;
        Complex res = (*this)(v->expression);
        id = _id;
        Complex arg1 = (*this)(op.upper);
        Complex arg2 = (*this)(op.lower);
        return (arg1 / arg2) * res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            ComplexPrecisionVariable v = boost::get<ComplexPrecisionVariable>(*var);
            Complex arg1 = (*this)(op.upper);
            Complex arg2 = (*this)(op.lower);
            return (arg1 / arg2) * (*v)(precision, default_angle_measure);
        }
        catch (boost::bad_get)
        {
        }
    }

    if (op.identifier.name == im)
        return Complex(MathHelper::ToBitPrecision(precision), 0, 1);

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(ImplicitDivMulNode<Array<Real>> const& op) const
{
    CheckBreak(parser_context);
    AddDependency(op.identifier);

    TempVariable* t = FindTempVariable(op.identifier.name);
    if (t)
    {
        Array<Real> arg1 = (*this)(op.upper);
        Array<Real> arg2 = (*this)(op.lower);
        return (arg1 / arg2) * t->second;
    }
    
    //find in the user defined variables
    VariableNode<Array<Real>>* v = FindVariable(op.identifier.name, op.identifier.subscript);
    if (v)
    {
        LogicalId _id = id;
        bool _exported_id = exported_id;
        id = v->id;
        exported_id = v->exported;
        Array<Real> res = (*this)(v->expression);
        if (!op.identifier.subscript.empty() && v->name.subscript != op.identifier.subscript)
        {
            try
            {
                int index = FromUtfString(op.identifier.subscript);
                if (res.Size() > index && index >= 0)
                {
                    Real r = res.Get(index);
                    res.Clear();
                    res.Add(r);
                }
                else
                    throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.identifier.name + U"' is over", op.pos, op.identifier.name.length(), op.line);
            }
            catch (const std::exception& e)
            {
                throw SyntaxException(op.id, ArgumentIsOver, U"Index of '" + op.identifier.name + U"' is over", op.pos, op.identifier.name.length(), op.line);
            }
        }
        id = _id;
        exported_id = _exported_id;
        Array<Real> arg1 = (*this)(op.upper);
        Array<Real> arg2 = (*this)(op.lower);
        return (arg1 / arg2) * res;
    }
    
    //find in the build-in variables
    BuiltinVariable* var = FindBuiltinVariable(op.identifier.name);
    if (var)
    {
        try
        {
            ArrayRealPrecisionVariable v = boost::get<PrecisionVariable>(*var);
            Array<Real> arg1 = (*this)(op.upper);
            Array<Real> arg2 = (*this)(op.lower);
            return (arg1 / arg2) * (*v)(precision);
        }
        catch (boost::bad_get)
        {
        }
    }

    auto* custom_unit = FindUnit(op.identifier.name, op.identifier.subscript);
    if (custom_unit)
    {
        symbols->last_unit_system = custom_unit->system;
        Array<Real> arg1 = (*this)(op.upper);
        Array<Real> arg2 = (*this)(op.lower);
        return (arg1 / arg2) * custom_unit->value;
    }

    if (op.identifier.subscript.empty() || op.identifier.subscript == U"SI")
    {
        Unit* unit = FindBuiltinUnit(op.identifier.name);
        if (unit)
        {
            symbols->last_unit_system = U"SI";
            Array<Real> arg1 = (*this)(op.upper);
            Array<Real> arg2 = (*this)(op.lower);
            return (arg1 / arg2) * Array<Real>(precision, *unit);
        }
    }

    Array<Real> val;
    if (FindBuiltinIdentifier(op.identifier.name, val))
    {
        Array<Real> arg1 = (*this)(op.upper);
        Array<Real> arg2 = (*this)(op.lower);
        return (arg1 / arg2) * val;
    }

    //there is no such an identifier
    throw SyntaxException(op.id, UnknownIdentifier, U"Identifier '" + op.identifier.name + U"' not found", op.pos, op.identifier.name.length(), op.line);
}

template<>
Array<Real> Solver<Array<Real>>::operator()(ArrayNode<Array<Real>> const& op) const
{
    Array<Real> res;
    for (const auto& v : op.array)
        res.Add((*this)(v));
    return res;
}

template<>
Real Solver<Real>::operator()(ScriptNode<Real> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
    AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const
{
    if (script.list.empty())
        throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected, -1, 0, -1);
    
    id = _id;
    dependencies = _dependencies;
    default_angle_measure = _default_angle_measure;
    result_angle_measure = _result_angle_measure;
    precision = _precision;

    Real res;
    //calculate all the script nodes
    BOOST_FOREACH(typename ScriptNode<Real>::Operand const& op, script.list)
    {
        CheckBreak(parser_context);
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
        case AngleMeasure::None:
            break;
        }
    }
    return res;
}

template<>
Integer Solver<Integer>::operator()(ScriptNode<Integer> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
    AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const
{
    if (script.list.empty())
        throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected, -1, 0, -1);
    
    id = _id;
    dependencies = _dependencies;
    
    Integer res;

    //calculate all the script nodes
    BOOST_FOREACH(typename ScriptNode<Integer>::Operand const& op, script.list)
    {
        CheckBreak(parser_context);
        res = boost::apply_visitor(*this, op);
    }

    return res;
}

template<>
Rational Solver<Rational>::operator()(ScriptNode<Rational> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
    AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const
{
    if (script.list.empty())
        throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected, -1, 0, -1);
    
    id = _id;
    dependencies = _dependencies;
   
    Rational res;

    //calculate all the script nodes
    BOOST_FOREACH(typename ScriptNode<Rational>::Operand const& op, script.list)
    {
        CheckBreak(parser_context);
        res = boost::apply_visitor(*this, op);
    }

    return res;
}

template<>
Complex Solver<Complex>::operator()(ScriptNode<Complex> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
    AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const
{
    if (script.list.empty())
        throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected, -1, 0, -1);
    
    id = _id;
    dependencies = _dependencies;
    default_angle_measure = _default_angle_measure;
    result_angle_measure = _result_angle_measure;
    precision = _precision;

    Complex res;
    //calculate all the script nodes
    BOOST_FOREACH(typename ScriptNode<Complex>::Operand const& op, script.list)
    {
        CheckBreak(parser_context);
        res = boost::apply_visitor(*this, op);
    }
    
    if (res.GetAngleMeasure() != AngleMeasure::None)
    {
        switch (result_angle_measure)
        {
        case AngleMeasure::Radian:
            return res.ToRadian();
        case AngleMeasure::Degree:
            return res.ToDegree();
        case AngleMeasure::Grad:
            return res.ToGrad();
        case AngleMeasure::None:
            break;
        }
    }
    return res;
}

template<>
Array<Real> Solver<Array<Real>>::operator()(ScriptNode<Array<Real>> const& script, LogicalId _id, AngleMeasure _default_angle_measure, 
    AngleMeasure _result_angle_measure, int _precision, Dependencies* _dependencies) const
{
    if (script.list.empty())
        throw SyntaxException(_id, ParserExceptionCode::ExpressionExpected, -1, 0, -1);
    
    id = _id;
    dependencies = _dependencies;
    default_angle_measure = _default_angle_measure;
    result_angle_measure = _result_angle_measure;
    precision = _precision;

    Array<Real> res;
    //calculate all the script nodes
    BOOST_FOREACH(typename ScriptNode<Array<Real>>::Operand const& op, script.list)
    {
        CheckBreak(parser_context);
        res = boost::apply_visitor(*this, op);
    }
    
    if (res.GetAngleMeasure() != AngleMeasure::None)
    {
        switch (result_angle_measure)
        {
        case AngleMeasure::Radian:
            return res.ToRadian();
        case AngleMeasure::Degree:
            return res.ToDegree();
        case AngleMeasure::Grad:
            return res.ToGrad();
        case AngleMeasure::None:
            break;
        }
    }
    return res;
}

template<>
Real Solver<Real>::GetSuitableUnit(const LogicalId _id, const Real& val, const std::u32string& system, const bool buildin) const
{
    return GetSuitableUnitImpl(_id, val, system, buildin);
}

template<>
Rational Solver<Rational>::GetSuitableUnit(const LogicalId _id, const Rational& val, const std::u32string& system, const bool buildin) const
{
    return GetSuitableUnitImpl(_id, val, system, buildin);
}

template<>
Integer Solver<Integer>::GetSuitableUnit(const LogicalId _id, const Integer& val, const std::u32string& system, const bool buildin) const
{
    return val;
}

template<>
Complex Solver<Complex>::GetSuitableUnit(const LogicalId _id, const Complex& val, const std::u32string& system, const bool buildin) const
{
    throw MathException(id, ParserExceptionCode::CannotCastToUnit);
}

template<>
Real Solver<Real>::CastToUnit(const LogicalId id, const Real& val, const Unit& unit) const
{
    return CastToUnitImpl(id, val, unit);
}

template<>
Rational Solver<Rational>::CastToUnit(const LogicalId id, const Rational& val, const Unit& unit) const
{
    return CastToUnitImpl(id, val, unit);
}

template<>
Integer Solver<Integer>::CastToUnit(const LogicalId id, const Integer& val, const Unit& unit) const
{
    throw MathException(id, ParserExceptionCode::CannotCastToUnit);
}

template<>
Complex Solver<Complex>::CastToUnit(const LogicalId id, const Complex& val, const Unit& unit) const
{
    throw MathException(id, ParserExceptionCode::CannotCastToUnit);
}

}
