/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "symbolic.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <vector>
#include <numeric>
#include <map>
#include <optional>
#include "real.h"
#include "rational.h"
#include "complex.h"
#include "utils.h"
#include "giac_utils.h"

namespace yutovo_calculator
{

//Symbolic

template<>
bool Symbolic<Real>::IsNegativeInfinityNumber(const Real& num)
{
    return num.IsInfinity() && num.GetSign();
}

template<>
giac::gen Symbolic<Real>::ToExpression(const Real& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    std::string s = yutovo_calculator::ToBasicString(num.ToString());
    return ParseGen(s, &context);
}

template<>
giac::gen Symbolic<Rational>::ToExpression(const Rational& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    std::string s = yutovo_calculator::ToBasicString(num.ToString());
    return ParseGen(s, &context);
}

template<>
giac::gen Symbolic<Complex>::ToExpression(const Complex& num) const
{
    if (!num.GetRe().unit.IsEmpty() || !num.GetIm().unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);
    std::string re = yutovo_calculator::ToBasicString(num.GetRe().ToString());
    std::string im = yutovo_calculator::ToBasicString(num.GetIm().ToString());
    std::string expr;
    if (im[0] == '-')
        expr = "(" + re + im + "*i)";
    else
        expr = "(" + re + "+" + im + "*i)";
    return ParseGen(expr, &context);
}

template<>
std::string Symbolic<Real>::ToStdString(int exp, Language language) const
{
    if (!expr)
        return {};

    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    if (e.is_integer())
    {
        std::string int_str = e.print(&context);
        if (int_str.size() < 6)
            return AddDotIfInteger(RealNumberStr(int_str, precision, exp));
        return int_str;
    }
    if (!e.is_integer() && e.type != giac::_REAL && e.type != giac::_DOUBLE_ && !HasUnknownSymbol(e))
        e = giac::evalf(e, 1, &context);
    std::string s = e.print(&context);
    //convention: ln(0) is +infinity, not -infinity
    if ((s == "-inf" || s == "-infinity" || s == "-oo") && !explicit_negative_infinity)
        s = "oo";
    //real sqrt of -infinity is undefined
    if (expr->type == giac::_SYMB && expr->_SYMBptr->sommet == giac::at_sqrt)
    {
        giac::gen arg = giac::eval(expr->_SYMBptr->feuille, 1, &context);
        std::string arg_str = arg.print(&context);
        if (arg_str.find("-inf") != std::string::npos || arg_str.find("-infinity") != std::string::npos || arg_str.find("-oo") != std::string::npos)
            s = "nan";
    }

    if (e.is_integer())
    {
        std::string int_str = e.print(&context);
        if (int_str.size() < 6)
            return AddDotIfInteger(RealNumberStr(int_str, precision, exp));
        return int_str;
    }

    FormatContext ctx{FormatContext::Real, precision, exp, language, false};
    return FormatGiacString(std::move(s), ctx);
}

template<>
std::u32string Symbolic<Real>::ToString(int exp, Language language) const
{
    return ToUtfString(ToStdString(exp, language));
}

template<>
std::string Symbolic<Rational>::ToStdString(int exp, Language language) const
{
    if (!expr)
        return {};
    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    if (e.is_integer())
        return e.print(&context);
    std::string s = e.print(&context);

    FormatContext ctx{FormatContext::Rational, precision, exp, language, false};
    return FormatGiacString(std::move(s), ctx);
}

template<>
std::u32string Symbolic<Rational>::ToString(int exp, Language language) const
{
    return ToUtfString(ToStdString(exp, language));
}

template<>
std::string Symbolic<Complex>::ToStdString(int exp, Language language) const
{
    if (!expr)
        return {};
    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    if (!e.is_integer() && e.type != giac::_REAL && e.type != giac::_DOUBLE_ &&
        e.type != giac::_CPLX && !HasUnknownSymbol(e))
    {
        e = giac::evalf(e, 1, &context);
    }
    if (e.is_integer())
        return e.print(&context);
    std::string s = e.print(&context);

    s = Symbolic<Real>::ReplaceSqrtSymbol(s);
    char imag_unit = (language == Language::Russian) ? 'j' : 'i';
    s = Symbolic<Real>::ReplaceImaginaryUnit(s, imag_unit);

    FormatContext ctx{FormatContext::Complex, precision, exp, language, false};
    std::string result = FormatGiacString(std::move(s), ctx);
    result = StripStandaloneIntegerDot(result);
    result = Symbolic<Real>::ReplaceUnitImaginary(result);
    return result;
}

template<>
std::u32string Symbolic<Complex>::ToString(int exp, Language language) const
{
    return ToUtfString(ToStdString(exp, language));
}

template<>
std::string Symbolic<Real>::ToJson(int exp, Language language) const
{
    if (!expr)
        return {};
    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    std::string s = e.print(&context);
    FormatContext ctx{FormatContext::Real, precision, exp, language, true};
    GiacExpression ast = BuildFormattedAst(std::move(s), ctx);
    return ExprToJson(ast, 45, ctx);
}

template<>
std::string Symbolic<Rational>::ToJson(int exp, Language language) const
{
    if (!expr)
        return {};
    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    std::string s = e.print(&context);
    FormatContext ctx{FormatContext::Rational, precision, exp, language, true};
    GiacExpression ast = BuildFormattedAst(std::move(s), ctx);
    return ExprToJson(ast, 46, ctx);
}

template<>
std::string Symbolic<Complex>::ToJson(int exp, Language language) const
{
    if (!expr)
        return {};
    giac::decimal_digits(std::max(1, precision + 10), &context);
    giac::gen e = giac::eval(*expr, 1, &context);
    std::string s = e.print(&context);
    s = Symbolic<Real>::ReplaceSqrtSymbol(s);
    char imag_unit = (language == Language::Russian) ? 'j' : 'i';
    s = Symbolic<Real>::ReplaceImaginaryUnit(s, imag_unit);

    FormatContext ctx{FormatContext::Complex, precision, exp, language, true};
    GiacExpression ast = BuildFormattedAst(std::move(s), ctx);
    return ExprToJson(ast, 47, ctx);
}

template class Symbolic<Rational>;
template class Symbolic<Complex>;

}
