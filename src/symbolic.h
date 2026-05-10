/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SYMBOLIC_H
#define SYMBOLIC_H

#include <string>
#include <memory>
#include <regex>
#include <symengine/expression.h>
#include <symengine/parser.h>
#include <symengine/symbol.h>
#include <symengine/eval.h>
#include <symengine/number.h>
#include <symengine/eval_mpfr.h>
#include <symengine/eval_mpc.h>
#include <symengine/real_mpfr.h>
#include <symengine/real_double.h>
#include <symengine/complex.h>
#include <symengine/basic.h>
#include <symengine/functions.h>
#include <symengine/subs.h>
#include <symengine/simplify.h>
#include <symengine/derivative.h>
#include "math_helper.h"
#include "parser_exception.h"

namespace yutovo_calculator
{

template<class Number>
class Symbolic
{
public:
    Symbolic() : 
        precision(3),
        expr(std::make_unique<SymEngine::Expression>())
    {
    }

    explicit Symbolic(int _precision) : 
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>())
    {
    }

    explicit Symbolic(int _precision, int num) :
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>(num))
    {
    }

    explicit Symbolic(int _precision, float num) : 
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>(static_cast<double>(num)))
    {
    }

    explicit Symbolic(int _precision, const Number& num) : 
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>(ToExpression(num)))
    {
    }

    explicit Symbolic(int _precision, const std::u32string& num) : 
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>(ToBasicString(num)))
    {
    }

    explicit Symbolic(int _precision, const std::string& num) : 
        precision(_precision),
        expr(std::make_unique<SymEngine::Expression>(num))
    {
    }

    Symbolic(const Symbolic& source) :
        precision(source.precision),
        expr(std::make_unique<SymEngine::Expression>(*source.expr))
    {
    }

public:
    typedef Symbolic<Number> value_type;

    Symbolic<Number>& operator=(const Symbolic<Number>& source)
    {
        precision = source.precision;
        *expr = *source.expr;
        return *this;
    }
    
    Symbolic<Number>& operator=(const int num)
    {
        *expr = SymEngine::Expression(num);
        return *this;
    }

    Symbolic<Number>& operator=(const double num)
    {
        *expr = SymEngine::Expression(num);
        return *this;
    }

    Symbolic<Number>& operator=(const std::u32string& num)
    {
        *expr = SymEngine::Expression(ToBasicString(num));
        return *this;
    }

    Symbolic<Number>& operator=(const Number& num)
    {
        *expr = ToExpression(num);
        return *this;
    }

    Symbolic<Number> operator+() const
    {
        return *this;
    }

    Symbolic<Number> operator-() const
    {
        Symbolic<Number> res(*this);
        *res.expr = -(*expr);
        return res;
    }

    friend Symbolic<Number> operator+(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = *num1.expr + *num2.expr;
        return res;
    }

    friend Symbolic<Number> operator-(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = *num1.expr - *num2.expr;
        return res;
    }

    friend Symbolic<Number> operator*(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = *num1.expr * *num2.expr;
        return res;
    }

    friend Symbolic<Number> operator/(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = *num1.expr / *num2.expr;
        return res;
    }

    friend Symbolic<Number> operator^(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = pow(*num1.expr, *num2.expr);
        return res;
    }

    void operator+=(const Symbolic<Number>& num)
    {
        *expr = *expr + *num.expr;
    }

    void operator-=(const Symbolic<Number>& num)
    {
        *expr = *expr - *num.expr;
    }

    void operator*=(const Symbolic<Number>& num)
    {
        *expr = *expr * *num.expr;
    }

    void operator/=(const Symbolic<Number>& num)
    {
        *expr = *expr / *num.expr;
    }

    void operator^=(const Symbolic<Number>& num)
    {
        *expr = SymEngine::pow(*expr, *num.expr);
    }

    friend bool operator==(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        return eq(*num1.expr, *num2.expr);
    }

    friend bool operator==(const Symbolic<Number>& num1, const int num2)
    {
        return eq(*num1.expr, SymEngine::Expression(num2));
    }

    friend bool operator==(const int num1, const Symbolic<Number>& num2)
    {
        return num2 == num1;
    }

    friend bool operator!=(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        return !eq(*num1.expr, *num2.expr);
    }

    friend bool operator!=(const Symbolic<Number>& num1, const int num2)
    {
        return !eq(*num1.expr, SymEngine::Expression(num2));
    }

    friend bool operator!=(const int num1, const Symbolic<Number>& num2)
    {
        return num2 != num1;
    }

    friend bool operator>(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator>(const Symbolic<Number>& num1, const int num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator>(const int num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator>=(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator>=(const Symbolic<Number>& num1, const int num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator>=(const int num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<(const Symbolic<Number>& num1, const int num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<(const int num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<=(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<=(const Symbolic<Number>& num1, const int num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

    friend bool operator<=(const int num1, const Symbolic<Number>& num2)
    {
        throw ParserException({}, ParserExceptionCode::IncorrectOperation);
    }

public:
    friend Symbolic<Number> evalf(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::evalf(*num.expr->get_basic(), MathHelper::ToBitPrecision(num.precision));
        return res;
    }

    friend Symbolic<Number> evalf(const Symbolic<Number>& num, const Symbolic<Number>& prec)
    {
        int p = static_cast<int>(eval_double(*prec.expr));
        Symbolic<Number> res(p);
        *res.expr = SymEngine::evalf(*num.expr->get_basic(), p);
        return res;
    }
    
    friend Symbolic<Number> expand(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::expand(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> simplify(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::simplify(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> diff(const Symbolic<Number>& num, const Symbolic<Number>& var)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::diff(num.expr->get_basic(), SymEngine::rcp_static_cast<const SymEngine::Symbol>(var.expr->get_basic()));
        return res;
    }
    
    friend Symbolic<Number> subs(const Symbolic<Number>& num, const Symbolic<Number>& var, const Symbolic<Number>& value)
    {
        Symbolic<Number> res(num.precision);
        SymEngine::map_basic_basic subs_map;
        subs_map[*var.expr] = *value.expr;
        *res.expr = SymEngine::subs(*num.expr, subs_map);
        return res;
    }
    
    friend Symbolic<Number> sin(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::sin(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> cos(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::cos(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> pow(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = SymEngine::pow(*num1.expr, *num2.expr);
        return res;
    }
    
    friend Symbolic<Number> log(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = SymEngine::log(*num2.expr, *num1.expr);
        return res;
    }
    
    friend Symbolic<Number> root(const Symbolic<Number>& num1, const Symbolic<Number>& num2)
    {
        Symbolic<Number> res(num1.precision);
        *res.expr = SymEngine::pow(*num1.expr, SymEngine::Expression(1) / *num2.expr);
        return res;
    }

public:
    std::u32string ToString(int exp) const;
    std::string ToStdString(int exp) const;

    int GetPrecision() const
    {
        return precision;
    }

    bool IsZero() const
    {
        return eq(*expr, SymEngine::Expression(0));
    }

    bool IsNumber() const
    {
        return is_a_Number(*expr->get_basic());
    }

private:
    SymEngine::Expression ToExpression(const Number& num) const;

    std::string ReplacePowerOperator(std::string s) const
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

    static std::string FormatScientific(std::string s)
    {
        size_t e_pos = s.find_first_of("eE");
        if (e_pos == std::string::npos)
            return s;

        std::string mantissa = s.substr(0, e_pos);
        std::string exp_str = s.substr(e_pos + 1);

        //remove trailing zeros from mantissa
        if (mantissa.find('.') != std::string::npos)
        {
            while (!mantissa.empty() && mantissa.back() == '0')
                mantissa.pop_back();
            if (!mantissa.empty() && mantissa.back() == '.')
                mantissa += '0';
        }

        //process exponent sign and leading zeros
        bool neg = false;
        if (!exp_str.empty() && exp_str[0] == '-')
        {
            neg = true;
            exp_str = exp_str.substr(1);
        }
        else if (!exp_str.empty() && exp_str[0] == '+')
        {
            exp_str = exp_str.substr(1);
        }
        while (exp_str.size() > 1 && exp_str[0] == '0')
            exp_str.erase(0, 1);

        return mantissa + "E" + (neg ? "-" : "+") + exp_str;
    }

    static std::string FormatFixed(std::string s, bool keep_trailing_dot)
    {
        if (s.find('.') != std::string::npos)
        {
            while (!s.empty() && s.back() == '0')
                s.pop_back();
            if (!s.empty() && s.back() == '.')
            {
                if (!keep_trailing_dot)
                    s.pop_back();
            }
        }
        return s;
    }

    static std::string AddTrailingDotIfSingleDigit(std::string s)
    {
        if (s.find('.') == std::string::npos && s.length() == 1)
            s += '.';
        return s;
    }

    static int GetDecimalOrder(const std::string& num_str)
    {
        size_t e_pos = num_str.find_first_of("eE");
        if (e_pos != std::string::npos)
            return std::stoi(num_str.substr(e_pos + 1)) + 1;
        size_t dotPos = num_str.find('.');
        if (dotPos != std::string::npos)
            return static_cast<int>(dotPos);
        return static_cast<int>(num_str.length());
    }

    static std::string ReplaceRationalNumbers(const std::string& expr_str, int exp, int precision)
    {
        std::regex re(R"((\d+)/(\d+))");
        std::string result;
        size_t pos = 0;
        std::sregex_iterator it(expr_str.begin(), expr_str.end(), re);
        std::sregex_iterator end;
        for (; it != end; ++it)
        {
            std::smatch match = *it;
            result += expr_str.substr(pos, match.position() - pos);
            std::string num_str = match[1];
            std::string den_str = match[2];

            if (exp <= precision && num_str == "1")
            {
                result += match[0];
            }
            else
            {
                mpfr_t num, den, q;
                mpfr_init2(num, 512);
                mpfr_init2(den, 512);
                mpfr_init2(q, 512);
                mpfr_set_str(num, num_str.c_str(), 10, MPFR_RNDN);
                mpfr_set_str(den, den_str.c_str(), 10, MPFR_RNDN);
                mpfr_div(q, num, den, MPFR_RNDN);
                char buf[512];
                mpfr_snprintf(buf, 512, "%.*Rf", precision, q);
                std::string s = buf;
                s = FormatFixed(s, false);
                result += s;
                mpfr_clear(num);
                mpfr_clear(den);
                mpfr_clear(q);
            }

            pos = match.position() + match.length();
        }
        result += expr_str.substr(pos);
        return result;
    }

    static std::string RemoveNumberParentheses(const std::string& expr_str)
    {
        std::regex re(R"(\((\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)\))");
        return std::regex_replace(expr_str, re, "$1");
    }

    static std::string FormatNumberInExpression(const std::string& expr_str, int exp, int precision)
    {
        std::regex re(R"((\d+(\.\d+)?([eE][+-]?\d+)?))");
        std::string result;
        size_t pos = 0;
        std::sregex_iterator it(expr_str.begin(), expr_str.end(), re);
        std::sregex_iterator end;
        for (; it != end; ++it)
        {
            std::smatch match = *it;
            result += expr_str.substr(pos, match.position() - pos);
            std::string num_str = match[0];

            int order = GetDecimalOrder(num_str);

            mpfr_t num;
            mpfr_init2(num, 512);
            mpfr_set_str(num, num_str.c_str(), 10, MPFR_RNDN);
            char buf[512];
            if (order > exp || num_str.find_first_of("eE") != std::string::npos)
            {
                mpfr_snprintf(buf, 512, "%.*Re", precision, num);
                result += FormatScientific(buf);
            }
            else
            {
                mpfr_snprintf(buf, 512, "%.*Rf", precision, num);
                result += FormatFixed(buf, false);
            }
            mpfr_clear(num);

            pos = match.position() + match.length();
        }
        result += expr_str.substr(pos);
        return result;
    }

private:
    int precision = 0;
    std::unique_ptr<SymEngine::Expression> expr;
};

}

#endif
