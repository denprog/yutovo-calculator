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
#include <symengine/pow.h>
#include <symengine/add.h>
#include <symengine/mul.h>
#include <symengine/printers/strprinter.h>
#include <symengine/derivative.h>
#include <symengine/subs.h>
#include <symengine/simplify.h>
#include <symengine/real_mpfr.h>
#include <symengine/real_double.h>
#include <symengine/complex.h>
#include <symengine/eval.h>
#include <symengine/number.h>
#include <symengine/visitor.h>
#include <symengine/integer.h>
#include <symengine/rational.h>
#include "real.h"
#include "rational.h"
#include "complex.h"

namespace yutovo_calculator
{

//Symbolic

template<>
SymEngine::Expression Symbolic<Real>::ToExpression(const Real& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    SymEngine::mpfr_class mc(num.number);
    return SymEngine::Expression(real_mpfr(mc));
}

template<>
std::string Symbolic<Real>::ToStdString(int exp) const
{
    if (!expr)
        return {};

    auto basic = expr->get_basic();

    if (!SymEngine::free_symbols(*basic).empty())
    {
        std::string s = basic->__str__();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        s = ReplaceRationalNumbers(s, exp, precision);
        s = RemoveNumberParentheses(s);
        s = FormatNumberInExpression(s, exp, precision);
        return ReplacePowerOperator(s);
    }

    if (SymEngine::is_a<SymEngine::Integer>(*basic))
    {
        std::string s = basic->__str__();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

        if (exp < 0)
        {
            int order = static_cast<int>(s.length());
            mpfr_t num;
            mpfr_init2(num, 512);
            mpfr_set_str(num, s.c_str(), 10, MPFR_RNDN);
            char buf[512];
            if (order > exp)
            {
                mpfr_snprintf(buf, 512, "%.*Re", precision, num);
                s = FormatScientific(buf);
            }
            else
            {
                mpfr_snprintf(buf, 512, "%.*Rf", precision, num);
                s = FormatFixed(buf, false);
            }
            mpfr_clear(num);
            if (s.find('.') == std::string::npos)
                s += '.';
        }
        else
        {
            int order = static_cast<int>(s.length());
            mpfr_t num;
            mpfr_init2(num, 512);
            mpfr_set_str(num, s.c_str(), 10, MPFR_RNDN);
            char buf[512];
            if (order > exp)
            {
                mpfr_snprintf(buf, 512, "%.*Re", precision, num);
                s = FormatScientific(buf);
            }
            else
            {
                mpfr_snprintf(buf, 512, "%.*Rf", precision, num);
                s = FormatFixed(buf, false);
                if (order <= precision && s.find('.') == std::string::npos)
                    s += '.';
            }
            mpfr_clear(num);
        }
        return ReplacePowerOperator(s);
    }

    if (SymEngine::is_a<SymEngine::RealMPFR>(*basic))
    {
        auto mpfr_basic = SymEngine::rcp_dynamic_cast<const SymEngine::RealMPFR>(basic);
        mpfr_srcptr value = mpfr_basic->as_mpfr().get_mpfr_t();
        std::string str = basic->__str__();
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        int order = GetDecimalOrder(str);
        if (!str.empty() && str[0] == '-')
            order--;

        char buf[512];
        if (order > exp)
        {
            mpfr_snprintf(buf, 512, "%.*Re", precision, value);
            str = FormatScientific(buf);
        }
        else
        {
            mpfr_snprintf(buf, 512, "%.*Rf", precision, value);
            str = FormatFixed(buf, false);
        }
        if (exp < 0)
        {
            if (str.find('.') == std::string::npos)
                str += '.';
        }
        else
        {
            if (order <= precision && str.find('.') == std::string::npos)
                str += '.';
        }
        return ReplacePowerOperator(str);
    }

    auto evaluated = SymEngine::evalf(*basic, MathHelper::ToBitPrecision(precision), SymEngine::EvalfDomain::Real);

    if (SymEngine::is_a<SymEngine::Integer>(*evaluated))
    {
        std::string s = evaluated->__str__();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        int order = static_cast<int>(s.length());
        if (exp < 0)
        {
            if (s.find('.') == std::string::npos)
                s += '.';
        }
        else
        {
            if (order <= precision && s.find('.') == std::string::npos)
                s += '.';
        }
        return ReplacePowerOperator(s);
    }

    auto mpfr = SymEngine::rcp_dynamic_cast<const SymEngine::RealMPFR>(evaluated);
    if (!mpfr.is_null())
    {
        mpfr_srcptr value = mpfr->as_mpfr().get_mpfr_t();
        std::string str = evaluated->__str__();
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        int order = GetDecimalOrder(str);
        if (!str.empty() && str[0] == '-')
            order--;
        char buf[512];
        if (order > exp)
        {
            mpfr_snprintf(buf, 512, "%.*Re", precision, value);
            std::string s = FormatScientific(buf);
            return ReplacePowerOperator(s);
        }
        else
        {
            mpfr_snprintf(buf, 512, "%.*Rf", precision, value);
            std::string s = FormatFixed(buf, false);
            if (exp < 0)
            {
                if (s.find('.') == std::string::npos)
                    s += '.';
            }
            else
            {
                if (order <= precision && s.find('.') == std::string::npos)
                    s += '.';
            }
            return ReplacePowerOperator(s);
        }
    }

    auto real_double = SymEngine::rcp_dynamic_cast<const SymEngine::RealDouble>(evaluated);
    if (!real_double.is_null())
    {
        double value = real_double->as_double();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        std::string s = oss.str();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        s = FormatFixed(s, exp == precision);
        int order = GetDecimalOrder(s);
        if (!s.empty() && s[0] == '-')
            order--;
        if (exp < 0)
        {
            if (s.find('.') == std::string::npos)
                s += '.';
        }
        else
        {
            if (order <= precision && s.find('.') == std::string::npos)
                s += '.';
        }
        return ReplacePowerOperator(s);
    }

    std::string s = evaluated->__str__();
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    return ReplacePowerOperator(s);
}

template<>
std::u32string Symbolic<Real>::ToString(int exp) const
{
    return ToUtfString(ToStdString(exp));
}

}
