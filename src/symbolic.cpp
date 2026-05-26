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
#include <symengine/pow.h>
#include <symengine/add.h>
#include <symengine/symengine_exception.h>
#include <symengine/mul.h>
#include <symengine/printers/strprinter.h>
#include <symengine/derivative.h>
#include <symengine/subs.h>
#include <symengine/simplify.h>
#include <symengine/real_mpfr.h>
#include <symengine/real_double.h>
#include <symengine/complex.h>
#include <symengine/complex_double.h>
#include <symengine/eval.h>
#include <symengine/number.h>
#include <symengine/visitor.h>
#include <symengine/integer.h>
#include <symengine/rational.h>
#include "real.h"
#include "rational.h"
#include "complex.h"
#include "utils.h"

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

    if (HasNan(*basic))
        return "nan";

    if (!SymEngine::free_symbols(*basic).empty() || HasInfinityOrNan(*basic))
    {
        std::string s = basic->__str__();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        if (s.find("zoo") != std::string::npos)
            s = "zoo";
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        s = ReplaceRationalNumbers(s, exp, precision);
        s = RemoveNumberParentheses(s);
        s = FormatNumberInExpression(s, exp, precision);
        std::string result;
        for (size_t i = 0; i < s.size(); )
        {
            if (std::isdigit(static_cast<unsigned char>(s[i])))
            {
                size_t j = i;
                while (j < s.size() && std::isdigit(static_cast<unsigned char>(s[j])))
                    ++j;
                bool skip = false;
                if (j < s.size() && (s[j] == '.' || s[j] == 'e' || s[j] == 'E'))
                    skip = true;
                if (i > 0 && s[i - 1] == '.')
                    skip = true;
                if (i > 0 && (s[i - 1] == 'e' || s[i - 1] == 'E'))
                    skip = true;
                if (i > 1 && (s[i - 2] == 'e' || s[i - 2] == 'E') && (s[i - 1] == '+' || s[i - 1] == '-'))
                    skip = true;
                result += s.substr(i, j - i);
                if (!skip)
                    result += '.';
                i = j;
            }
            else
            {
                result += s[i];
                ++i;
            }
        }
        s = result;
        s = ReplaceAll(s, "inf.", "∞");
        s = ReplaceAll(s, "inf", "∞");
        s = ReplaceAll(s, "zoo", "∞");
        s = ReplaceAll(s, "oo", "∞");
        return ReplacePowerOperator(s);
    }

    if (SymEngine::is_a<SymEngine::Integer>(*basic))
    {
        std::string s = basic->__str__();
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

        if (exp < 0)
        {
            mpfr_t num;
            mpfr_init2(num, 512);
            mpfr_set_str(num, s.c_str(), 10, MPFR_RNDN);
            s = FormatFixed(MpfrFormat("%.*Rf", precision, num), false);
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
            if (order > exp && exp >= 0)
            {
                s = FormatScientific(MpfrFormat("%.*Re", precision, num));
            }
            else
            {
                s = FormatFixed(MpfrFormat("%.*Rf", precision, num), false);
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
        if (mpfr_inf_p(value))
            return mpfr_sgn(value) > 0 ? "∞" : "-∞";
        if (mpfr_nan_p(value))
            return "nan";
        std::string str = basic->__str__();
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        int order = GetDecimalOrder(str);
        if (!str.empty() && str[0] == '-')
            order--;

        std::string fixed_str = MpfrFormat("%.*Rf", precision, value);
        bool is_zero_fixed = IsEffectivelyZeroFixedStr(FormatFixed(fixed_str, false));
        if ((order > exp && exp >= 0) || (mpfr_zero_p(value) == 0 && is_zero_fixed))
        {
            str = FormatScientific(MpfrFormat("%.*Re", precision, value));
        }
        else
        {
            str = FormatFixed(fixed_str, false);
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

    SymEngine::RCP<const SymEngine::Basic> evaluated;
    try
    {
        evaluated = SymEngine::evalf(*basic, MathHelper::ToBitPrecision(precision), SymEngine::EvalfDomain::Real);
    }
    catch (const SymEngine::DivisionByZeroError&)
    {
        throw MathException(LogicalId{}, DivisionByZero);
    }
    catch (const SymEngine::DomainError&)
    {
        throw MathException(LogicalId{}, ArgumentIsOver);
    }
    catch (const SymEngine::ParseError&)
    {
        throw MathException(LogicalId{}, SyntaxError);
    }
    catch (const SymEngine::NotImplementedError&)
    {
        throw MathException(LogicalId{}, NotImplemented);
    }
    catch (const SymEngine::SerializationError&)
    {
        throw MathException(LogicalId{}, NotImplemented);
    }
    catch (const std::exception&)
    {
        throw MathException(LogicalId{}, NotImplemented);
    }

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
        std::string fixed_str = MpfrFormat("%.*Rf", precision, value);
        bool is_zero_fixed = IsEffectivelyZeroFixedStr(FormatFixed(fixed_str, false));
        if ((order > exp && exp >= 0) || (mpfr_zero_p(value) == 0 && is_zero_fixed))
        {
            std::string s = FormatScientific(MpfrFormat("%.*Re", precision, value));
            return ReplacePowerOperator(s);
        }
        else
        {
            std::string s = FormatFixed(fixed_str, false);
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
        if (std::isinf(value))
            return value > 0.0 ? "∞" : "-∞";
        if (std::isnan(value))
            return "nan";
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        std::string fixed_str = oss.str();
        fixed_str.erase(std::remove(fixed_str.begin(), fixed_str.end(), ' '), fixed_str.end());
        int order = GetDecimalOrder(fixed_str);
        if (!fixed_str.empty() && fixed_str[0] == '-')
            order--;
        bool is_zero_fixed = IsEffectivelyZeroFixedStr(FormatFixed(fixed_str, false));
        std::string s;
        if ((order > exp && exp >= 0) || (value != 0.0 && is_zero_fixed))
        {
            oss.str("");
            oss.clear();
            oss << std::scientific << std::setprecision(precision) << value;
            s = oss.str();
            s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
            s = FormatScientific(s);
        }
        else
        {
            s = FormatFixed(fixed_str, exp == precision);
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
        }
        return ReplacePowerOperator(s);
    }

    std::string s = evaluated->__str__();
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    s = ReplaceAll(s, "inf.", "∞");
    s = ReplaceAll(s, "inf", "∞");
    s = ReplaceAll(s, "zoo", "∞");
    s = ReplaceAll(s, "oo", "∞");
    return ReplacePowerOperator(s);
}

template<>
std::u32string Symbolic<Real>::ToString(int exp) const
{
    return ToUtfString(ToStdString(exp));
}

template<>
SymEngine::Expression Symbolic<Rational>::ToExpression(const Rational& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);
    return SymEngine::Expression(ToBasicString(num.ToString()));
}

template<>
std::string Symbolic<Rational>::ToStdString(int exp) const
{
    if (!expr)
        return {};
    auto basic = expr->get_basic();
    if (HasNan(*basic))
        return "nan";
    std::string s = basic->__str__();
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    if (s.find("zoo") != std::string::npos)
        s = "zoo";
    return ReplacePowerOperator(ReplaceAll(ReplaceAll(ReplaceAll(ReplaceAll(s, "inf.", "∞"), "inf", "∞"), "zoo", "∞"), "oo", "∞"));
}

template<>
std::u32string Symbolic<Rational>::ToString(int exp) const
{
    return ToUtfString(ToStdString(exp));
}

template<>
SymEngine::Expression Symbolic<Complex>::ToExpression(const Complex& num) const
{
    if (!num.GetRe().unit.IsEmpty() || !num.GetIm().unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);
    return SymEngine::Expression(std::complex<double>(static_cast<double>(num.GetRe()), static_cast<double>(num.GetIm())));
}

template<>
std::string Symbolic<Complex>::ToStdString(int exp) const
{
    if (!expr)
        return {};
    auto basic = expr->get_basic();
    if (HasNan(*basic))
        return "nan";
    if (SymEngine::free_symbols(*basic).empty() && !HasInfinityOrNan(*basic))
    {
        SymEngine::RCP<const SymEngine::Basic> evaluated;
        try
        {
            evaluated = SymEngine::evalf(*basic, MathHelper::ToBitPrecision(precision));
        }
        catch (const SymEngine::DivisionByZeroError&)
        {
            throw MathException(LogicalId{}, DivisionByZero);
        }
        catch (const SymEngine::DomainError&)
        {
            throw MathException(LogicalId{}, ArgumentIsOver);
        }
        catch (const SymEngine::ParseError&)
        {
            throw MathException(LogicalId{}, SyntaxError);
        }
        catch (const SymEngine::NotImplementedError&)
        {
            throw MathException(LogicalId{}, NotImplemented);
        }
        catch (const SymEngine::SerializationError&)
        {
            throw MathException(LogicalId{}, NotImplemented);
        }
        catch (const std::exception&)
        {
            throw MathException(LogicalId{}, NotImplemented);
        }
        
        if (SymEngine::is_a<SymEngine::ComplexDouble>(*evaluated))
        {
            auto cd = SymEngine::rcp_dynamic_cast<const SymEngine::ComplexDouble>(evaluated);
            auto re = cd->real_part();
            auto im = cd->imaginary_part();
            if (im->is_zero())
            {
                Symbolic<Real> temp(precision);
                *temp.expr = SymEngine::Expression(re);
                return RemoveInsignificantPoint(temp.ToStdString(exp));
            }
            Symbolic<Real> re_sym(precision);
            *re_sym.expr = SymEngine::Expression(re);
            std::string re_str = re_sym.ToStdString(exp);
            if (re->is_zero())
            {
                Symbolic<Real> im_sym(precision);
                *im_sym.expr = SymEngine::Expression(im);
                return im_sym.ToStdString(exp) + "*i";
            }
            Symbolic<Real> im_sym(precision);
            *im_sym.expr = SymEngine::Expression(im);
            std::string im_str = im_sym.ToStdString(exp);
            if (!im_str.empty() && im_str[0] == '-')
                return re_str + im_str + "*i";
            return re_str + "+" + im_str + "*i";
        }
        Symbolic<Real> temp(precision);
        *temp.expr = SymEngine::Expression(evaluated);
        return RemoveInsignificantPoint(temp.ToStdString(exp));
    }

    std::string s = basic->__str__();
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    if (s.find("zoo") != std::string::npos)
        s = "zoo";
    std::replace(s.begin(), s.end(), 'I', 'i');
    s = ReplaceRationalNumbers(s, std::numeric_limits<int>::max(), precision);
    s = RemoveNumberParentheses(s);
    s = FormatNumberInExpression(s, exp, precision);
    std::string result;
    for (size_t i = 0; i < s.size(); )
    {
        if (std::isdigit(static_cast<unsigned char>(s[i])))
        {
            size_t j = i;
            while (j < s.size() && std::isdigit(static_cast<unsigned char>(s[j])))
                ++j;
            bool skip = false;
            if (j < s.size() && (s[j] == '.' || s[j] == 'e' || s[j] == 'E'))
                skip = true;
            if (i > 0 && s[i - 1] == '.')
                skip = true;
            if (i > 0 && (s[i - 1] == 'e' || s[i - 1] == 'E'))
                skip = true;
            if (i > 1 && (s[i - 2] == 'e' || s[i - 2] == 'E') && (s[i - 1] == '+' || s[i - 1] == '-'))
                skip = true;
            result += s.substr(i, j - i);
            if (!skip)
                result += '.';
            i = j;
        }
        else
        {
            result += s[i];
            ++i;
        }
    }
    return RemoveInsignificantPoint(ReplacePowerOperator(ReplaceAll(ReplaceAll(ReplaceAll(ReplaceAll(result, "inf.", "∞"), "inf", "∞"), "zoo", "∞"), "oo", "∞")));
}

template<>
std::u32string Symbolic<Complex>::ToString(int exp) const
{
    return ToUtfString(ToStdString(exp));
}

template<>
std::string Symbolic<Real>::ToJson(int exp) const
{
    if (!expr)
        return {};
    auto basic = expr->get_basic();
    std::string str = basic->__str__();
    if (str.find("zoo") != std::string::npos)
        return JsonResultWrapper(45, JsonCodeRow({JsonCodeString("∞")}));
    std::string content = BasicToJson(*basic, precision, exp, JsonNumberFormat::REAL);
    return JsonResultWrapper(45, content); // SYMBOLIC_REAL_RESULT
}

template<>
std::string Symbolic<Rational>::ToJson(int exp) const
{
    if (!expr)
        return {};
    auto basic = expr->get_basic();
    std::string str = basic->__str__();
    if (str.find("zoo") != std::string::npos)
        return JsonResultWrapper(46, JsonCodeRow({JsonCodeString("∞")}));
    std::string content = BasicToJson(*basic, precision, exp, JsonNumberFormat::RATIONAL);
    return JsonResultWrapper(46, content); // SYMBOLIC_RATIONAL_RESULT
}

template<>
std::string Symbolic<Complex>::ToJson(int exp) const
{
    if (!expr)
        return {};
    auto basic = expr->get_basic();
    std::string str = basic->__str__();
    if (str.find("zoo") != std::string::npos)
        return JsonResultWrapper(47, JsonCodeRow({JsonCodeString("∞")}));
    std::string content = BasicToJson(*basic, precision, exp, JsonNumberFormat::COMPLEX);
    return JsonResultWrapper(47, content); // SYMBOLIC_COMPLEX_RESULT
}

template class Symbolic<Rational>;
template class Symbolic<Complex>;

}
