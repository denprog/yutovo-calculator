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
#include <symengine/infinity.h>
#include <symengine/nan.h>
#include "math_helper.h"
#include "parser_exception.h"

namespace yutovo_calculator
{

template<typename Number> class Symbolic;
class Rational;
class Complex;

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
    
    friend Symbolic<Number> cot(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::cot(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> sec(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::sec(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> csc(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::csc(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> exp(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::exp(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> ln(const Symbolic<Number>& num)
    {
        Symbolic<Number> res(num.precision);
        *res.expr = SymEngine::log(*num.expr);
        return res;
    }
    
    friend Symbolic<Number> sqrt(const Symbolic<Number>& num)
    {
        return root(num, Symbolic<Number>(num.precision, 2));
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
    std::string ToJson(int exp = -1) const;

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
                mantissa.pop_back();
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

    static std::vector<std::string> JsonScientificElements(const std::string& sci_str)
    {
        size_t e_pos = sci_str.find_first_of("eE");
        if (e_pos == std::string::npos)
            return {JsonCodeString(sci_str)};

        std::string mantissa = sci_str.substr(0, e_pos);
        std::string exp_str = sci_str.substr(e_pos + 1);

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

        std::string power_exp = JsonCodeString(exp_str);
        if (neg)
            power_exp = JsonCodeRow({JsonMinus(), power_exp});

        std::string power = JsonPower(JsonCodeRow({JsonCodeString("10")}), JsonCodeRow({power_exp}));
        std::vector<std::string> result;
        if (!mantissa.empty() && mantissa[0] == '-')
        {
            result.push_back(JsonMinus());
            result.push_back(JsonCodeString(mantissa.substr(1)));
        }
        else
        {
            result.push_back(JsonCodeString(mantissa));
        }
        result.push_back(JsonMultiply());
        result.push_back(power);
        return result;
    }

public:
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

    static bool IsEffectivelyZeroFixedStr(const std::string& s)
    {
        for (char c : s)
        {
            if (c != '0' && c != '.' && c != '-')
                return false;
        }
        return true;
    }

    static std::string AddTrailingDotIfSingleDigit(std::string s)
    {
        if (s.find('.') == std::string::npos && s.length() == 1)
            s += '.';
        return s;
    }

    static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
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
                if (mpfr_zero_p(q) == 0 && IsEffectivelyZeroFixedStr(s))
                {
                    mpfr_snprintf(buf, 512, "%.*Re", precision, q);
                    s = buf;
                    s = FormatScientific(s);
                }
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
    enum class JsonNumberFormat
    {
        REAL,
        RATIONAL,
        COMPLEX
    };

    static std::vector<std::string> NumberToJsonElements(const SymEngine::Number& num, int precision, int exp, JsonNumberFormat format)
    {
        std::string s = FormatNumberForJson(num, precision, exp, format);
        if (s.find('E') != std::string::npos)
            return JsonScientificElements(s);
        if (!s.empty() && s[0] == '-')
            return {JsonMinus(), JsonCodeString(s.substr(1))};
        return {JsonCodeString(s)};
    }

    static std::string JsonEscape(const std::string& s)
    {
        std::string r;
        for (char c : s)
        {
            if (c == '\\' || c == '"')
                r += '\\';
            r += c;
        }
        return r;
    }

    static std::string JsonCodeString(const std::string& text)
    {
        return std::string(R"({"type":8,"elements":")") + JsonEscape(text) + R"("})";
    }

    static std::string JsonCodeRow(const std::vector<std::string>& items)
    {
        std::string s = R"({"type":7,"elements":[)";
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (i > 0)
                s += ",";
            s += items[i];
        }
        s += "]}";
        return s;
    }

    static std::string JsonPlus()
    {
        return R"({"type":11,"symbol":"+"})";
    }

    static std::string JsonMinus()
    {
        return R"({"type":12,"symbol":"-"})";
    }

    static std::string JsonMultiply()
    {
        return R"({"type":13,"symbol":"·"})";
    }

    static std::string JsonShape()
    {
        return R"({"type":10,"elements":[]})";
    }

    static std::string JsonOpenRoundBracket()
    {
        return R"({"type":19,"symbol":"("})";
    }

    static std::string JsonCloseRoundBracket()
    {
        return R"xx({"type":20,"symbol":")"})xx";
    }

    static std::string JsonPower(const std::string& base, const std::string& exp)
    {
        return R"({"type":15,"elements":[)" + base + "," + JsonShape() + "," + exp + "]}";
    }

    static std::string JsonDivision(const std::string& num, const std::string& den)
    {
        return R"({"type":14,"elements":[)" + num + "," + JsonShape() + "," + den + "]}";
    }

    static std::string JsonResultWrapper(int type, const std::string& content)
    {
        return R"({"type":)" + std::to_string(type) + R"(,"elements":[)" + content + "]}";
    }

    static std::string FormatNumberForJson(const SymEngine::Number& num, int precision, int exp, JsonNumberFormat format)
    {
        if (SymEngine::is_a<const SymEngine::Integer>(num))
        {
            std::string s = static_cast<const SymEngine::Integer&>(num).__str__();
            if (format == JsonNumberFormat::REAL || format == JsonNumberFormat::COMPLEX)
            {
                int order = static_cast<int>(s.length());
                if (order > exp && exp >= 0)
                {
                    mpfr_t mpfr_num;
                    mpfr_init2(mpfr_num, 512);
                    mpfr_set_str(mpfr_num, s.c_str(), 10, MPFR_RNDN);
                    char buf[512];
                    mpfr_snprintf(buf, 512, "%.*Re", precision, mpfr_num);
                    std::string res = FormatScientific(buf);
                    mpfr_clear(mpfr_num);
                    return res;
                }
            }
            return s;
        }
        if (SymEngine::is_a<const SymEngine::Rational>(num))
        {
            if (format == JsonNumberFormat::REAL || format == JsonNumberFormat::COMPLEX)
            {
                auto evaluated = SymEngine::evalf(num, MathHelper::ToBitPrecision(precision));
                if (SymEngine::is_a<const SymEngine::RealDouble>(*evaluated))
                    return FormatNumberForJson(static_cast<const SymEngine::RealDouble&>(*evaluated), precision, exp, format);
                if (SymEngine::is_a<const SymEngine::RealMPFR>(*evaluated))
                    return FormatNumberForJson(static_cast<const SymEngine::RealMPFR&>(*evaluated), precision, exp, format);
                return evaluated->__str__();
            }
            return static_cast<const SymEngine::Rational&>(num).__str__();
        }
        if (SymEngine::is_a<const SymEngine::RealDouble>(num))
        {
            double value = static_cast<const SymEngine::RealDouble&>(num).as_double();
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
                s = FormatFixed(fixed_str, false);
            }
            return s;
        }
        if (SymEngine::is_a<const SymEngine::ComplexDouble>(num))
        {
            const auto& cd = static_cast<const SymEngine::ComplexDouble&>(num);
            auto re = cd.real_part();
            auto im = cd.imaginary_part();
            std::string re_str = FormatNumberForJson(*re, precision, exp, format);
            std::string im_str = FormatNumberForJson(*im, precision, exp, format);
            return re_str + "+" + im_str + "*i";
        }
        if (SymEngine::is_a<const SymEngine::RealMPFR>(num))
        {
            mpfr_srcptr value = static_cast<const SymEngine::RealMPFR&>(num).as_mpfr().get_mpfr_t();
            std::string str = num.__str__();
            str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
            int order = GetDecimalOrder(str);
            if (!str.empty() && str[0] == '-')
                order--;
            char buf[512];
            mpfr_snprintf(buf, 512, "%.*Rf", precision, value);
            bool is_zero_fixed = IsEffectivelyZeroFixedStr(FormatFixed(std::string(buf), false));
            if ((order > exp && exp >= 0) || (mpfr_zero_p(value) == 0 && is_zero_fixed))
            {
                mpfr_snprintf(buf, 512, "%.*Re", precision, value);
                return FormatScientific(buf);
            }
            else
            {
                return FormatFixed(buf, false);
            }
        }
        return num.__str__();
    }

    static std::string BasicToJson(const SymEngine::Basic& expr, int precision, int exp, JsonNumberFormat format)
    {
        if (SymEngine::is_a<const SymEngine::Pow>(expr))
        {
            const SymEngine::Pow& pow = static_cast<const SymEngine::Pow&>(expr);
            std::string base = BasicToJson(*pow.get_base(), precision, exp, format);
            std::string exp_str = BasicToJson(*pow.get_exp(), precision, exp, format);
            auto exp_basic = pow.get_exp();
            if (SymEngine::is_a<const SymEngine::Integer>(*exp_basic))
            {
                const auto& exp_int = static_cast<const SymEngine::Integer&>(*exp_basic);
                if (exp_int.is_negative())
                {
                    auto neg = exp_int.neg();
                    auto neg_elems = NumberToJsonElements(*neg, precision, exp, format);
                    std::string abs_json = neg_elems.size() == 1 ? neg_elems[0] : JsonCodeRow(neg_elems);
                    exp_str = JsonCodeRow({JsonMinus(), abs_json});
                }
            }
            else if (SymEngine::is_a<const SymEngine::Rational>(*exp_basic))
            {
                const auto& exp_rat = static_cast<const SymEngine::Rational&>(*exp_basic);
                if (exp_rat.is_negative())
                {
                    auto neg_rat = exp_rat.neg();
                    auto neg_elems = NumberToJsonElements(*neg_rat, precision, exp, format);
                    std::string abs_json = neg_elems.size() == 1 ? neg_elems[0] : JsonCodeRow(neg_elems);
                    exp_str = JsonCodeRow({JsonMinus(), abs_json});
                }
            }
            return JsonPower(base, exp_str);
        }
        auto items = BasicToJsonElements(expr, precision, exp, format);
        if (items.size() == 1 && items[0].find(R"({"type":14)") == 0)
            return items[0];
        return JsonCodeRow(items);
    }

    static std::vector<std::string> BasicToJsonElements(const SymEngine::Basic& expr, int precision, int exp, JsonNumberFormat format)
    {
        if (SymEngine::is_a<const SymEngine::Symbol>(expr))
            return {JsonCodeString(expr.__str__())};

        if (SymEngine::is_a<const SymEngine::Integer>(expr) ||
            SymEngine::is_a<const SymEngine::RealDouble>(expr) || SymEngine::is_a<const SymEngine::RealMPFR>(expr))
        {
            return NumberToJsonElements(static_cast<const SymEngine::Number&>(expr), precision, exp, format);
        }

        if (SymEngine::is_a<const SymEngine::ComplexDouble>(expr))
        {
            const auto& cd = static_cast<const SymEngine::ComplexDouble&>(expr);
            auto re = cd.real_part();
            auto im = cd.imaginary_part();
            std::vector<std::string> items;
            std::string re_str = FormatNumberForJson(*re, precision, exp, format);
            if (re_str.find('E') != std::string::npos)
            {
                auto sci = JsonScientificElements(re_str);
                items.insert(items.end(), sci.begin(), sci.end());
            }
            else if (!re_str.empty() && re_str[0] == '-')
            {
                items.push_back(JsonMinus());
                items.push_back(JsonCodeString(re_str.substr(1)));
            }
            else
                items.push_back(JsonCodeString(re_str));
            items.push_back(JsonPlus());
            std::string im_str = FormatNumberForJson(*im, precision, exp, format);
            if (im_str.find('E') != std::string::npos)
            {
                auto sci = JsonScientificElements(im_str);
                items.insert(items.end(), sci.begin(), sci.end());
            }
            else if (!im_str.empty() && im_str[0] == '-')
            {
                items.push_back(JsonMinus());
                items.push_back(JsonCodeString(im_str.substr(1)));
            }
            else
                items.push_back(JsonCodeString(im_str));
            items.push_back(JsonMultiply());
            items.push_back(JsonCodeString("i"));
            return items;
        }

        if (SymEngine::is_a<const SymEngine::Rational>(expr))
        {
            const SymEngine::Rational& rat = static_cast<const SymEngine::Rational&>(expr);
            if (format == JsonNumberFormat::RATIONAL)
            {
                auto num = rat.get_num();
                auto den = rat.get_den();
                if (num->is_negative())
                    num = num->neg();
                std::string num_json = JsonCodeString(num->__str__());
                std::string den_json = JsonCodeString(den->__str__());
                std::string div = JsonDivision(JsonCodeRow({num_json}), JsonCodeRow({den_json}));
                if (rat.is_negative())
                    return {JsonMinus(), div};
                return {div};
            }
            return NumberToJsonElements(rat, precision, exp, format);
        }

        if (SymEngine::is_a<const SymEngine::Add>(expr))
        {
            const SymEngine::Add& add = static_cast<const SymEngine::Add&>(expr);
            auto coef = add.get_coef();
            auto dict = add.get_dict();

            std::vector<std::string> items;

            if (!coef->is_zero())
            {
                auto coef_elements = BasicToJsonElements(*coef, precision, exp, format);
                items.insert(items.end(), coef_elements.begin(), coef_elements.end());
            }

            std::vector<std::pair<std::string, std::pair<SymEngine::RCP<const SymEngine::Basic>, SymEngine::RCP<const SymEngine::Basic>>>> sorted;
            for (const auto& p : dict)
                sorted.emplace_back(p.first->__str__(), std::make_pair(p.first, p.second));
            std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

            for (const auto& entry : sorted)
            {
                const auto& p = entry.second;
                auto term_elements = BasicToJsonElements(*p.first, precision, exp, format);
                auto coeff_elements = BasicToJsonElements(*p.second, precision, exp, format);
                bool pos = true;

                if (SymEngine::is_a<const SymEngine::Integer>(*p.second))
                {
                    const SymEngine::Integer& coeff_int = static_cast<const SymEngine::Integer&>(*p.second);
                    if (coeff_int.is_negative())
                    {
                        pos = false;
                        auto neg = coeff_int.neg();
                        coeff_elements = BasicToJsonElements(*neg, precision, exp, format);
                    }
                }
                else if (SymEngine::is_a<const SymEngine::Rational>(*p.second))
                {
                    const SymEngine::Rational& coeff_rat = static_cast<const SymEngine::Rational&>(*p.second);
                    if (coeff_rat.is_negative())
                    {
                        pos = false;
                        auto neg_rat = coeff_rat.neg();
                        coeff_elements = BasicToJsonElements(*neg_rat, precision, exp, format);
                    }
                }

                bool coeff_is_one = false;
                if (coeff_elements.size() == 1)
                {
                    if (coeff_elements[0] == JsonCodeString("1.") || coeff_elements[0] == JsonCodeString("1"))
                        coeff_is_one = true;
                }

                if (!coeff_is_one)
                {
                    term_elements.insert(term_elements.begin(), JsonMultiply());
                    for (auto it = coeff_elements.rbegin(); it != coeff_elements.rend(); ++it)
                        term_elements.insert(term_elements.begin(), *it);
                }

                if (!items.empty())
                    items.push_back(pos ? JsonPlus() : JsonMinus());
                else if (!pos)
                    items.push_back(JsonMinus());

                for (auto& el : term_elements)
                    items.push_back(el);
            }
            return items;
        }

        if (SymEngine::is_a<const SymEngine::Pow>(expr))
            return {BasicToJson(expr, precision, exp, format)};

        if (SymEngine::is_a<const SymEngine::Sin>(expr))
        {
            const auto& sin = static_cast<const SymEngine::Sin&>(expr);
            auto arg_elements = BasicToJsonElements(*sin.get_arg(), precision, exp, format);
            std::vector<std::string> items;
            items.push_back(JsonCodeString("sin"));
            items.push_back(JsonOpenRoundBracket());
            for (auto& el : arg_elements)
                items.push_back(el);
            items.push_back(JsonCloseRoundBracket());
            return items;
        }

        if (SymEngine::is_a<const SymEngine::Cos>(expr))
        {
            const auto& cos = static_cast<const SymEngine::Cos&>(expr);
            auto arg_elements = BasicToJsonElements(*cos.get_arg(), precision, exp, format);
            std::vector<std::string> items;
            items.push_back(JsonCodeString("cos"));
            items.push_back(JsonOpenRoundBracket());
            for (auto& el : arg_elements)
                items.push_back(el);
            items.push_back(JsonCloseRoundBracket());
            return items;
        }

        if (SymEngine::is_a<const SymEngine::Mul>(expr))
        {
            const SymEngine::Mul& mul = static_cast<const SymEngine::Mul&>(expr);
            auto coef = mul.get_coef();
            auto dict = mul.get_dict();

            std::vector<std::string> positive;
            std::vector<std::string> negative;
            bool leading_minus = false;

            if (!coef->is_zero() && !coef->is_one() && !coef->is_minus_one())
            {
                auto coef_elems = BasicToJsonElements(*coef, precision, exp, format);
                if (coef_elems.size() == 1)
                    positive.push_back(coef_elems[0]);
                else
                    positive.push_back(JsonCodeRow(coef_elems));
            }
            else if (coef->is_minus_one())
                leading_minus = true;

            for (const auto& p : dict)
            {
                std::string base_json;
                if (SymEngine::is_a<const SymEngine::Symbol>(*p.first))
                    base_json = JsonCodeString(p.first->__str__());
                else if (SymEngine::is_a<const SymEngine::Integer>(*p.first) ||
                    SymEngine::is_a<const SymEngine::Rational>(*p.first) ||
                    SymEngine::is_a<const SymEngine::RealDouble>(*p.first) ||
                    SymEngine::is_a<const SymEngine::RealMPFR>(*p.first) ||
                    SymEngine::is_a<const SymEngine::ComplexDouble>(*p.first))
                {
                    auto base_elems = NumberToJsonElements(static_cast<const SymEngine::Number&>(*p.first), precision, exp, format);
                    if (base_elems.size() == 1)
                        base_json = base_elems[0];
                    else
                        base_json = JsonCodeRow(base_elems);
                }
                else
                    base_json = BasicToJson(*p.first, precision, exp, format);

                bool is_neg = false;
                if (SymEngine::is_a<const SymEngine::Integer>(*p.second))
                {
                    if (static_cast<const SymEngine::Integer&>(*p.second).is_negative())
                        is_neg = true;
                }

                if (is_neg)
                {
                    auto neg_exp = static_cast<const SymEngine::Integer&>(*p.second).neg();
                    auto exp_elems = NumberToJsonElements(*neg_exp, precision, exp, format);
                    std::string exp_json = exp_elems.size() == 1 ? exp_elems[0] : JsonCodeRow(exp_elems);
                    if (exp_json == JsonCodeString("1.") || exp_json == JsonCodeString("1"))
                        negative.push_back(base_json);
                    else
                        negative.push_back(JsonPower(JsonCodeRow({base_json}), JsonCodeRow({exp_json})));
                }
                else
                {
                    auto exp_elems = NumberToJsonElements(*static_cast<const SymEngine::Number*>(p.second.get()), precision, exp, format);
                    std::string exp_json = exp_elems.size() == 1 ? exp_elems[0] : JsonCodeRow(exp_elems);
                    if (exp_json == JsonCodeString("1.") || exp_json == JsonCodeString("1"))
                        positive.push_back(base_json);
                    else
                        positive.push_back(JsonPower(JsonCodeRow({base_json}), JsonCodeRow({exp_json})));
                }
            }

            std::vector<std::string> items;

            if (!negative.empty())
            {
                std::vector<std::string> num_items;
                bool first = true;
                for (auto& p : positive)
                {
                    if (!first)
                        num_items.push_back(JsonMultiply());
                    num_items.push_back(p);
                    first = false;
                }

                std::string num = num_items.empty() ? JsonCodeRow({JsonCodeString("1.")}) : JsonCodeRow(num_items);
                std::vector<std::string> den_items;
                first = true;
                for (auto& n : negative)
                {
                    if (!first)
                        den_items.push_back(JsonMultiply());
                    den_items.push_back(n);
                    first = false;
                }
                std::string den = JsonCodeRow(den_items);

                std::string div = JsonDivision(num, den);
                if (leading_minus)
                    items.push_back(JsonMinus());
                items.push_back(div);
            }
            else
            {
                bool first = true;
                for (auto& p : positive)
                {
                    if (!first)
                        items.push_back(JsonMultiply());
                    items.push_back(p);
                    first = false;
                }
                if (leading_minus)
                    items.insert(items.begin(), JsonMinus());
            }
            return items;
        }

        if (SymEngine::is_a<SymEngine::Infty>(expr))
        {
            const auto& inf = static_cast<const SymEngine::Infty&>(expr);
            if (inf.is_negative())
                return {JsonMinus(), JsonCodeString("∞")};
            return {JsonCodeString("∞")};
        }

        if (SymEngine::is_a<SymEngine::NaN>(expr))
            return {JsonCodeString("nan")};

        return {JsonCodeString(expr.__str__())};
    }

private:
    friend class Symbolic<Rational>;
    friend class Symbolic<Complex>;

    static bool HasNan(const SymEngine::Basic& expr)
    {
        if (SymEngine::is_a<SymEngine::NaN>(expr))
            return true;
        for (const auto& arg : expr.get_args())
        {
            if (HasNan(*arg))
                return true;
        }
        return false;
    }

    static bool HasInfinityOrNan(const SymEngine::Basic& expr)
    {
        if (SymEngine::is_a<SymEngine::Infty>(expr) || SymEngine::is_a<SymEngine::NaN>(expr))
            return true;
        for (const auto& arg : expr.get_args())
        {
            if (HasInfinityOrNan(*arg))
                return true;
        }
        return false;
    }

    static std::string RemoveInsignificantPoint(const std::string& s)
    {
        if (s.size() < 2 || s.back() != '.')
            return s;
        for (size_t i = 0; i < s.size() - 1; ++i)
        {
            if (i == 0 && s[i] == '-')
                continue;
            if (!std::isdigit(static_cast<unsigned char>(s[i])))
                return s;
        }
        return s.substr(0, s.size() - 1);
    }

    int precision = 0;
    std::unique_ptr<SymEngine::Expression> expr;
};

template<typename T>
struct is_symbolic : std::false_type {};

template<typename T>
struct is_symbolic<Symbolic<T>> : std::true_type {};

template<typename T>
inline constexpr bool is_symbolic_v = is_symbolic<T>::value;

}

#endif
