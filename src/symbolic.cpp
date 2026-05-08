/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "symbolic.h"
#include <algorithm>
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

namespace yutovo_calculator
{

using namespace SymEngine;

// Symbolic

template<>
SymEngine::Expression Symbolic::ToExpression<Real>(const Real& num) const;
template<>
SymEngine::Expression Symbolic::ToExpression<Complex>(const Complex& num) const;
template<>
SymEngine::Expression Symbolic::ToExpression<Rational>(const Rational& num) const;

Symbolic::Symbolic() : 
    precision(53),
    expr(std::make_unique<Expression>())
{
}

Symbolic::Symbolic(int _precision) : 
    precision(_precision),
    expr(std::make_unique<Expression>())
{
}

Symbolic::Symbolic(int _precision, int num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(num))
{
}

Symbolic::Symbolic(int _precision, float num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(static_cast<double>(num)))
{
}

Symbolic::Symbolic(int _precision, const std::u32string& num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(ToUtf8(num)))
{
}

Symbolic::Symbolic(int _precision, const std::string& num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(num))
{
}

Symbolic::Symbolic(int _precision, const Real& num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(ToExpression(num)))
{
}

Symbolic::Symbolic(int _precision, const Complex& num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(ToExpression(num)))
{
}

Symbolic::Symbolic(int _precision, const Rational& num) : 
    precision(_precision),
    expr(std::make_unique<Expression>(ToExpression(num)))
{
}

Symbolic::Symbolic(const Symbolic& source) : 
    precision(source.precision),
    expr(std::make_unique<Expression>(*source.expr))
{
}

Symbolic& Symbolic::operator=(const Symbolic& source)
{
    precision = source.precision;
    *expr = *source.expr;
    return *this;
}

Symbolic& Symbolic::operator=(const int num)
{
    *expr = Expression(num);
    return *this;
}

Symbolic& Symbolic::operator=(const double num)
{
    *expr = Expression(num);
    return *this;
}

Symbolic& Symbolic::operator=(const std::u32string& num)
{
    *expr = Expression(ToUtf8(num));
    return *this;
}

Symbolic& Symbolic::operator=(const Real& num)
{
    *expr = ToExpression(num);
    return *this;
}

Symbolic& Symbolic::operator=(const Complex& num)
{
    *expr = ToExpression(num);
    return *this;
}

Symbolic& Symbolic::operator=(const Rational& num)
{
    *expr = ToExpression(num);
    return *this;
}

Symbolic Symbolic::operator+() const
{
    return *this;
}

Symbolic Symbolic::operator-() const
{
    Symbolic res(*this);
    *res.expr = -(*expr);
    return res;
}

Symbolic operator+(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr + *num2.expr;
    return res;
}

Symbolic operator+(const Symbolic& num1, const int num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr + Expression(num2);
    return res;
}

Symbolic operator+(const int num1, const Symbolic& num2)
{
    return num2 + num1;
}

Symbolic operator-(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr - *num2.expr;
    return res;
}

Symbolic operator-(const Symbolic& num1, const int num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr - Expression(num2);
    return res;
}

Symbolic operator-(const int num1, const Symbolic& num2)
{
    Symbolic res(num2.precision);
    *res.expr = Expression(num1) - *num2.expr;
    return res;
}

Symbolic operator*(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr * *num2.expr;
    return res;
}

Symbolic operator*(const Symbolic& num1, const int num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr * Expression(num2);
    return res;
}

Symbolic operator*(const int num1, const Symbolic& num2)
{
    return num2 * num1;
}

Symbolic operator/(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr / *num2.expr;
    return res;
}

Symbolic operator/(const Symbolic& num1, const int num2)
{
    Symbolic res(num1.precision);
    *res.expr = *num1.expr / Expression(num2);
    return res;
}

Symbolic operator/(const int num1, const Symbolic& num2)
{
    Symbolic res(num2.precision);
    *res.expr = Expression(num1) / *num2.expr;
    return res;
}

Symbolic operator^(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = pow(*num1.expr, *num2.expr);
    return res;
}

Symbolic operator^(const Symbolic& num1, const int num2)
{
    Symbolic res(num1.precision);
    *res.expr = pow(*num1.expr, Expression(num2));
    return res;
}

Symbolic operator^(const int num1, const Symbolic& num2)
{
    Symbolic res(num2.precision);
    *res.expr = pow(Expression(num1), *num2.expr);
    return res;
}

Symbolic operator+(const Symbolic& num1, const Real& num2)
{
    return num1 + Symbolic(num1.precision, num2);
}

Symbolic operator+(const Real& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) + num2;
}

Symbolic operator+(const Symbolic& num1, const Complex& num2)
{
    return num1 + Symbolic(num1.precision, num2);
}

Symbolic operator+(const Complex& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) + num2;
}

Symbolic operator+(const Symbolic& num1, const Rational& num2)
{
    return num1 + Symbolic(num1.precision, num2);
}

Symbolic operator+(const Rational& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) + num2;
}

Symbolic operator-(const Symbolic& num1, const Real& num2)
{
    return num1 - Symbolic(num1.precision, num2);
}

Symbolic operator-(const Real& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) - num2;
}

Symbolic operator-(const Symbolic& num1, const Complex& num2)
{
    return num1 - Symbolic(num1.precision, num2);
}

Symbolic operator-(const Complex& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) - num2;
}

Symbolic operator-(const Symbolic& num1, const Rational& num2)
{
    return num1 - Symbolic(num1.precision, num2);
}

Symbolic operator-(const Rational& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) - num2;
}

Symbolic operator*(const Symbolic& num1, const Real& num2)
{
    return num1 * Symbolic(num1.precision, num2);
}

Symbolic operator*(const Real& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) * num2;
}

Symbolic operator*(const Symbolic& num1, const Complex& num2)
{
    return num1 * Symbolic(num1.precision, num2);
}

Symbolic operator*(const Complex& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) * num2;
}

Symbolic operator*(const Symbolic& num1, const Rational& num2)
{
    return num1 * Symbolic(num1.precision, num2);
}

Symbolic operator*(const Rational& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) * num2;
}

Symbolic operator/(const Symbolic& num1, const Real& num2)
{
    return num1 / Symbolic(num1.precision, num2);
}

Symbolic operator/(const Real& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) / num2;
}

Symbolic operator/(const Symbolic& num1, const Complex& num2)
{
    return num1 / Symbolic(num1.precision, num2);
}

Symbolic operator/(const Complex& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) / num2;
}

Symbolic operator/(const Symbolic& num1, const Rational& num2)
{
    return num1 / Symbolic(num1.precision, num2);
}

Symbolic operator/(const Rational& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) / num2;
}

Symbolic operator^(const Symbolic& num1, const Real& num2)
{
    return num1 ^ Symbolic(num1.precision, num2);
}

Symbolic operator^(const Real& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) ^ num2;
}

Symbolic operator^(const Symbolic& num1, const Complex& num2)
{
    return num1 ^ Symbolic(num1.precision, num2);
}

Symbolic operator^(const Complex& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) ^ num2;
}

Symbolic operator^(const Symbolic& num1, const Rational& num2)
{
    return num1 ^ Symbolic(num1.precision, num2);
}

Symbolic operator^(const Rational& num1, const Symbolic& num2)
{
    return Symbolic(num2.precision, num1) ^ num2;
}

void Symbolic::operator+=(const Symbolic& num)
{
    *expr = *expr + *num.expr;
}

void Symbolic::operator+=(const int num)
{
    *expr = *expr + Expression(num);
}

void Symbolic::operator-=(const Symbolic& num)
{
    *expr = *expr - *num.expr;
}

void Symbolic::operator-=(const int num)
{
    *expr = *expr - Expression(num);
}

void Symbolic::operator*=(const Symbolic& num)
{
    *expr = *expr * *num.expr;
}

void Symbolic::operator*=(const int num)
{
    *expr = *expr * Expression(num);
}

void Symbolic::operator/=(const Symbolic& num)
{
    *expr = *expr / *num.expr;
}

void Symbolic::operator/=(const int num)
{
    *expr = *expr / Expression(num);
}

void Symbolic::operator^=(const Symbolic& num)
{
    *expr = SymEngine::pow(*expr, *num.expr);
}

void Symbolic::operator^=(const int num)
{
    *expr = SymEngine::pow(*expr, Expression(num));
}

bool operator==(const Symbolic& num1, const Symbolic& num2)
{
    return eq(*num1.expr, *num2.expr);
}

bool operator==(const Symbolic& num1, const int num2)
{
    return eq(*num1.expr, Expression(num2));
}

bool operator==(const int num1, const Symbolic& num2)
{
    return num2 == num1;
}

bool operator!=(const Symbolic& num1, const Symbolic& num2)
{
    return !eq(*num1.expr, *num2.expr);
}

bool operator!=(const Symbolic& num1, const int num2)
{
    return !eq(*num1.expr, Expression(num2));
}

bool operator!=(const int num1, const Symbolic& num2)
{
    return num2 != num1;
}

bool operator>(const Symbolic& num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator>(const Symbolic& num1, const int num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator>(const int num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator>=(const Symbolic& num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator>=(const Symbolic& num1, const int num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator>=(const int num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<(const Symbolic& num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<(const Symbolic& num1, const int num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<(const int num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<=(const Symbolic& num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<=(const Symbolic& num1, const int num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool operator<=(const int num1, const Symbolic& num2)
{
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

static RCP<const Basic> SafeEvalf(const RCP<const Basic>& expr, int precision)
{
#ifdef HAVE_SYMENGINE_MPFR
    return evalf(*expr, precision, SymEngine::EvalfDomain::Complex);
#else
    (void)precision;
    return evalf(*expr, 53, SymEngine::EvalfDomain::Complex);
#endif
}

Symbolic Symbolic::evalf(const Symbolic& num)
{
    Symbolic res(num.precision);
    *res.expr = SafeEvalf(num.expr->get_basic(), num.precision);
    return res;
}

Symbolic Symbolic::evalf(const Symbolic& num, const Symbolic& prec)
{
    int p = static_cast<int>(eval_double(*prec.expr));
    Symbolic res(p);
    *res.expr = SafeEvalf(num.expr->get_basic(), p);
    return res;
}

Symbolic Symbolic::expand(const Symbolic& num)
{
    Symbolic res(num.precision);
    *res.expr = SymEngine::expand(*num.expr);
    return res;
}

Symbolic Symbolic::simplify(const Symbolic& num)
{
    Symbolic res(num.precision);
    *res.expr = SymEngine::simplify(*num.expr);
    return res;
}

Symbolic Symbolic::factor(const Symbolic& num)
{
    //factor for expressions is not available in this version of SymEngine
    throw MathException(ParserExceptionCode::NotImplemented);
}

Symbolic Symbolic::diff(const Symbolic& num, const Symbolic& var)
{
    Symbolic res(num.precision);
    *res.expr = SymEngine::diff(num.expr->get_basic(), rcp_static_cast<const Symbol>(var.expr->get_basic()));
    return res;
}

Symbolic Symbolic::integrate(const Symbolic& num, const Symbolic& var)
{
    //integrate for expressions is not available in this version of SymEngine
    throw MathException(ParserExceptionCode::NotImplemented);
}

Symbolic Symbolic::subs(const Symbolic& num, const Symbolic& var, const Symbolic& value)
{
    Symbolic res(num.precision);
    map_basic_basic subs_map;
    subs_map[*var.expr] = *value.expr;
    *res.expr = SymEngine::subs(*num.expr, subs_map);
    return res;
}

static Real SymEngineNumberToReal(const RCP<const Basic>& eval_result, int precision)
{
    if (is_a<SymEngine::Integer>(*eval_result))
    {
        const SymEngine::Integer& i = down_cast<const SymEngine::Integer&>(*eval_result);
        std::string s = i.__str__();
        return Real(precision, s.c_str());
    }
    if (is_a<SymEngine::Rational>(*eval_result))
    {
        const SymEngine::Rational& r = down_cast<const SymEngine::Rational&>(*eval_result);
        std::string s = r.__str__();
        return Real(precision, s.c_str());
    }
#ifdef HAVE_SYMENGINE_MPFR
    if (is_a<RealMPFR>(*eval_result))
    {
        const RealMPFR& r = down_cast<const RealMPFR&>(*eval_result);
        Real res(precision);
        mpfr_set(res.number, r.as_mpfr().get_mpfr_t(), MPFR_RNDN);
        return res;
    }
#endif
    if (is_a<RealDouble>(*eval_result))
    {
        const RealDouble& r = down_cast<const RealDouble&>(*eval_result);
        return Real(precision, static_cast<float>(r.i));
    }
    if (is_a_sub<SymEngine::ComplexBase>(*eval_result))
    {
        auto re = down_cast<const SymEngine::ComplexBase&>(*eval_result).real_part();
        return SymEngineNumberToReal(re, precision);
    }
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

Real Symbolic::to_real(const Symbolic& num)
{
    auto eval_result = SafeEvalf(num.expr->get_basic(), num.precision);
    return SymEngineNumberToReal(eval_result, num.precision);
}

Complex Symbolic::to_complex(const Symbolic& num)
{
    auto eval_result = SafeEvalf(num.expr->get_basic(), num.precision);
    if (is_a_sub<ComplexBase>(*eval_result))
    {
        auto re = down_cast<const ComplexBase&>(*eval_result).real_part();
        auto im = down_cast<const ComplexBase&>(*eval_result).imaginary_part();
        return Complex(SymEngineNumberToReal(re, num.precision), SymEngineNumberToReal(im, num.precision));
    }
    Real re = SymEngineNumberToReal(eval_result, num.precision);
    return Complex(re, Real(num.precision, 0));
}

Rational Symbolic::to_rational(const Symbolic& num)
{
    RCP<const Basic> eval_result = num.expr->get_basic();
    if (is_a<SymEngine::Integer>(*eval_result))
    {
        const SymEngine::Integer& i = down_cast<const SymEngine::Integer&>(*eval_result);
        return Rational(ToUtf32(i.__str__()));
    }
    if (is_a<SymEngine::Rational>(*eval_result))
    {
        const SymEngine::Rational& r = down_cast<const SymEngine::Rational&>(*eval_result);
        return Rational(ToUtf32(r.__str__()));
    }
    eval_result = SymEngine::expand(eval_result);
    if (is_a<SymEngine::Integer>(*eval_result))
    {
        const SymEngine::Integer& i = down_cast<const SymEngine::Integer&>(*eval_result);
        return Rational(ToUtf32(i.__str__()));
    }
    if (is_a<SymEngine::Rational>(*eval_result))
    {
        const SymEngine::Rational& r = down_cast<const SymEngine::Rational&>(*eval_result);
        return Rational(ToUtf32(r.__str__()));
    }
    throw ParserException({}, ParserExceptionCode::IncorrectOperation);
}

bool Symbolic::IsZero() const
{
    return eq(*expr, Expression(0));
}

bool Symbolic::IsNumber() const
{
    return is_a_Number(*expr->get_basic());
}

int Symbolic::GetPrecision() const
{
    return precision;
}

void Symbolic::SetPrecision(int _precision)
{
    precision = _precision;
}

std::u32string Symbolic::ToString() const
{
    return ToUtf32(ToStdString());
}

static std::string ReplacePowerOperator(std::string s)
{
    size_t pos = 0;
    while ((pos = s.find("**", pos)) != std::string::npos)
    {
        size_t base_end = pos;
        size_t base_start = base_end;
        if (base_start > 0 && s[base_start - 1] == ')')
        {
            int depth = 1;
            base_start -= 2;
            while (base_start != std::string::npos && depth > 0)
            {
                if (s[base_start] == ')')
                    ++depth;
                else if (s[base_start] == '(')
                    --depth;
                if (depth > 0)
                    --base_start;
            }
        }
        else
        {
            while (base_start > 0 && (std::isalnum(static_cast<unsigned char>(s[base_start - 1])) || s[base_start - 1] == '.'))
                --base_start;
        }

        size_t exp_start = pos + 2;
        size_t exp_end = exp_start;
        if (exp_start < s.size() && s[exp_start] == '(')
        {
            int depth = 1;
            ++exp_end;
            while (exp_end < s.size() && depth > 0)
            {
                if (s[exp_end] == '(')
                    ++depth;
                else if (s[exp_end] == ')')
                    --depth;
                if (depth > 0)
                    ++exp_end;
            }
            ++exp_end;
        }
        else
        {
            while (exp_end < s.size() && (std::isalnum(static_cast<unsigned char>(s[exp_end])) || s[exp_end] == '.'))
                ++exp_end;
        }

        std::string base = s.substr(base_start, base_end - base_start);
        std::string exp = s.substr(exp_start, exp_end - exp_start);
        auto strip_parens = [](const std::string& str) -> std::string {
            if (str.size() >= 2 && str.front() == '(' && str.back() == ')')
                return str.substr(1, str.size() - 2);
            return str;
        };
        base = strip_parens(base);
        exp = strip_parens(exp);
        std::string replacement = "pow(" + base + "," + exp + ")";
        s.replace(base_start, exp_end - base_start, replacement);
        pos = base_start + replacement.size();
    }
    return s;
}

std::string Symbolic::ToStdString() const
{
    std::string s = expr->get_basic()->__str__();
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    s = ReplacePowerOperator(s);
    return s;
}

Symbolic Symbolic::pow(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = SymEngine::pow(*num1.expr, *num2.expr);
    return res;
}

Symbolic Symbolic::log(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = SymEngine::log(*num2.expr, *num1.expr);
    return res;
}

Symbolic Symbolic::root(const Symbolic& num1, const Symbolic& num2)
{
    Symbolic res(num1.precision);
    *res.expr = SymEngine::pow(*num1.expr, Expression(1) / *num2.expr);
    return res;
}

Symbolic Symbolic::sin(const Symbolic& num)
{
    Symbolic res(num.precision);
    *res.expr = SymEngine::sin(*num.expr);
    return res;
}

Symbolic Symbolic::cos(const Symbolic& num)
{
    Symbolic res(num.precision);
    *res.expr = SymEngine::cos(*num.expr);
    return res;
}

SymEngine::Expression& Symbolic::GetExpr() const
{
    return *expr;
}

std::string Symbolic::ToUtf8(const std::u32string& s)
{
    std::string res;
    for (char32_t c : s)
    {
        if (c < 0x80)
            res += static_cast<char>(c);
        else if (c < 0x800)
        {
            res += static_cast<char>(0xC0 | (c >> 6));
            res += static_cast<char>(0x80 | (c & 0x3F));
        }
        else if (c < 0x10000)
        {
            res += static_cast<char>(0xE0 | (c >> 12));
            res += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            res += static_cast<char>(0x80 | (c & 0x3F));
        }
        else
        {
            res += static_cast<char>(0xF0 | (c >> 18));
            res += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
            res += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            res += static_cast<char>(0x80 | (c & 0x3F));
        }
    }
    return res;
}

std::u32string Symbolic::ToUtf32(const std::string& s)
{
    std::u32string res;
    for (size_t i = 0; i < s.length();)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        char32_t cp = 0;
        if ((c & 0x80) == 0)
        {
            cp = c;
            i += 1;
        }
        else if ((c & 0xE0) == 0xC0)
        {
            cp = ((c & 0x1F) << 6) | (s[i+1] & 0x3F);
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0)
        {
            cp = ((c & 0x0F) << 12) | ((s[i+1] & 0x3F) << 6) | (s[i+2] & 0x3F);
            i += 3;
        }
        else
        {
            cp = ((c & 0x07) << 18) | ((s[i+1] & 0x3F) << 12) | ((s[i+2] & 0x3F) << 6) | (s[i+3] & 0x3F);
            i += 4;
        }
        res += cp;
    }
    return res;
}

template<>
SymEngine::Expression Symbolic::ToExpression<Real>(const Real& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

#ifdef HAVE_SYMENGINE_MPFR
    mpfr_class mc(num.number);
    return Expression(real_mpfr(mc));
#else
    std::u32string s32 = num.ToString();
    return Expression(ToUtf8(s32));
#endif
}

template<>
SymEngine::Expression Symbolic::ToExpression<Complex>(const Complex& num) const
{
    if (!num.GetRe().unit.IsEmpty() || !num.GetIm().unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

#ifdef HAVE_SYMENGINE_MPFR
    Real re = num.GetRe();
    Real im = num.GetIm();
    mpfr_class mc_re(re.number);
    mpfr_class mc_im(im.number);
    auto sym_re = real_mpfr(mc_re);
    auto sym_im = real_mpfr(mc_im);
    return Expression(add(*sym_re, mul(*sym_im, I)));
#else
    return Expression(std::complex<double>(static_cast<double>(num.GetRe()), static_cast<double>(num.GetIm())));
#endif
}

template<>
SymEngine::Expression Symbolic::ToExpression<Rational>(const Rational& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    std::u32string s32 = num.ToString();
    return Expression(ToUtf8(s32));
}

}
