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

namespace yutovo_calculator
{

//Symbolic

template<>
giac::gen Symbolic<Real>::ToExpression(const Real& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    std::string s = yutovo_calculator::ToBasicString(num.ToString());
    return giac::gen(s.c_str(), const_cast<giac::context*>(&context));
}

template<>
giac::gen Symbolic<Rational>::ToExpression(const Rational& num) const
{
    if (!num.unit.IsEmpty())
        throw ParserException({}, ParserExceptionCode::CannotCastToUnit);

    std::string s = yutovo_calculator::ToBasicString(num.ToString());
    return giac::gen(s.c_str(), const_cast<giac::context*>(&context));
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
    return giac::gen(expr.c_str(), const_cast<giac::context*>(&context));
}

namespace
{
    std::string GiacToString(const giac::gen& g, giac::context* ctx)
    {
        return g.print(ctx);
    }


    std::string RoundScientificHalfUp(std::string s, int decimals)
    {
        size_t e_pos = s.find_first_of("eE");
        if (e_pos == std::string::npos)
            return s;
        std::string mantissa = s.substr(0, e_pos);
        std::string exp_str = s.substr(e_pos);
        bool neg = !mantissa.empty() && mantissa[0] == '-';
        if (neg)
            mantissa = mantissa.substr(1);
        size_t dot = mantissa.find('.');
        if (dot == std::string::npos)
            return s;
        std::string digits = mantissa.substr(0, dot) + mantissa.substr(dot + 1);
        int total_decimals = static_cast<int>(mantissa.size() - dot - 1);
        int drop = total_decimals - decimals;
        if (drop <= 0)
            return s;
        std::string rounded = digits.substr(0, digits.size() - static_cast<size_t>(drop));
        char round_digit = digits[digits.size() - static_cast<size_t>(drop)];
        if (round_digit >= '5')
        {
            int i = static_cast<int>(rounded.size()) - 1;
            bool carry = true;
            while (i >= 0 && carry)
            {
                if (rounded[i] == '9')
                    rounded[i] = '0';
                else
                {
                    ++rounded[i];
                    carry = false;
                }
                --i;
            }
            if (carry)
                rounded.insert(rounded.begin(), '1');
        }
        std::string result = rounded.substr(0, rounded.size() - static_cast<size_t>(decimals)) + "." +
                             rounded.substr(rounded.size() - static_cast<size_t>(decimals));
        while (!result.empty() && result.back() == '0')
            result.pop_back();
        if (!result.empty() && result.back() == '.')
            result.pop_back();
        return (neg ? "-" : "") + result + exp_str;
    }

    std::string RealNumberStr(const std::string& num_str, int precision, int exp)
    {
        std::string raw = num_str;
        if (!raw.empty() && raw.back() == '.')
            raw.pop_back();
        if (raw == "inf" || raw == "+inf" || raw == "+infinity" || raw == "infinity")
            return "∞";
        if (raw == "-inf" || raw == "-infinity")
            return "-∞";
        if (raw == "nan" || raw == "undef")
            return "nan";

        int order = Symbolic<Real>::GetDecimalOrder(raw);
        bool is_integer_string = raw.find('.') == std::string::npos &&
                                 raw.find_first_of("eE") == std::string::npos;
        bool can_use_double = false;
        double d = 0.0;
        if (precision <= 15)
        {
            try
            {
                size_t idx = 0;
                d = std::stod(raw, &idx);
                if (idx == raw.size())
                {
                    // doubles cannot exactly represent very large integers
                    if (!is_integer_string || std::fabs(d) <= 9e15)
                        can_use_double = true;
                }
            }
            catch (...)
            {
            }
        }

        std::string s;
        if (can_use_double)
        {
            std::ostringstream oss;
            if (order > exp && exp >= 0)
            {
                oss << std::scientific << std::setprecision(precision) << d;
                s = Symbolic<Real>::FormatScientific(oss.str());
            }
            else
            {
                oss << std::fixed << std::setprecision(precision) << d;
                s = Symbolic<Real>::FormatFixed(oss.str(), false);
                if (d != 0.0 && Symbolic<Real>::IsEffectivelyZeroFixedStr(s))
                {
                    oss.str("");
                    oss.clear();
                    oss << std::scientific << std::setprecision(precision) << d;
                    s = Symbolic<Real>::FormatScientific(oss.str());
                }
            }
        }
        else
        {
            Symbolic<Real>::MpfrGuard guard;
            mpfr_set_str(guard.num, raw.c_str(), 10, MPFR_RNDN);
            if (order > exp && exp >= 0)
            {
                s = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", precision, guard.num));
            }
            else
            {
                s = Symbolic<Real>::FormatFixed(Symbolic<Real>::MpfrFormat("%.*Rf", precision, guard.num), false);
                if (mpfr_zero_p(guard.num) == 0 && Symbolic<Real>::IsEffectivelyZeroFixedStr(s))
                {
                    s = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", precision, guard.num));
                }
            }
        }
        return s;
    }

    std::string AddDotIfInteger(const std::string& s)
    {
        if (s.empty() || s == "-" || s.find('.') != std::string::npos ||
            s.find('e') != std::string::npos || s.find('E') != std::string::npos ||
            s == "∞" || s == "-∞" || s == "nan")
            return s;
        bool is_integer = std::all_of(s.begin(), s.end(), [](char c) {
            return std::isdigit(static_cast<unsigned char>(c)) || c == '-';
        }) && !(s.size() == 1 && s[0] == '-');
        if (is_integer)
        {
            long long v = 0;
            try { v = std::stoll(s); } catch (...) { v = 0; }
            if (std::llabs(v) >= 1000)
                return s;
        }
        return s + ".";
    }

    std::string StripStandaloneIntegerDot(std::string s)
    {
        if (s.empty() || s.back() != '.')
            return s;
        size_t i = 0;
        if (s[0] == '-')
            ++i;
        if (i >= s.size() - 1)
            return s;
        for (; i < s.size() - 1; ++i)
        {
            if (!std::isdigit(static_cast<unsigned char>(s[i])))
                return s;
        }
        s.pop_back();
        return s;
    }

    // ------------------------------------------------------------------
    // AST-based formatting helpers
    // ------------------------------------------------------------------

    struct FormatCtx
    {
        enum Mode
        {
            Real,
            Rational,
            Complex
        };
        Mode mode = Real;
        int precision = 0;
        int exp = 0;
        Language language = Language::English;
        bool json = false;
        bool in_product = false;
    };

    struct Expr
    {
        enum Kind
        {
            Number,
            Ident,
            Func,
            Sum,
            Product,
            Power,
            Neg,
            Div,
            Sqrt,
            Root
        };

        Kind kind = Number;
        std::string value;
        std::vector<Expr> args;

        Expr() = default;
        Expr(Kind k, std::string v = {}) :
            kind(k),
            value(std::move(v))
        {
        }
        Expr(Kind k, std::vector<Expr> a) :
            kind(k),
            args(std::move(a))
        {
        }
        Expr(Kind k, Expr a) :
            kind(k)
        {
            args.push_back(std::move(a));
        }
        Expr(Kind k, Expr a, Expr b) :
            kind(k)
        {
            args.push_back(std::move(a));
            args.push_back(std::move(b));
        }
        Expr(Kind k, std::string v, std::vector<Expr> a) :
            kind(k),
            value(std::move(v)),
            args(std::move(a))
        {
        }
    };

    std::string StripQuotes(const std::string& name)
    {
        std::string n = name;
        while (!n.empty() && (n.front() == '\'' || n.front() == '"'))
            n.erase(n.begin());
        while (!n.empty() && (n.back() == '\'' || n.back() == '"'))
            n.pop_back();
        return n;
    }

    class ExprParser
    {
    public:
        explicit ExprParser(const std::string& str) :
            s(str),
            pos(0)
        {
        }

        Expr parse()
        {
            Expr result = parseAddSub();
            skipSpaces();
            return result;
        }

    private:
        const std::string& s;
        size_t pos;

        void skipSpaces()
        {
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])))
                ++pos;
        }

        char peek()
        {
            skipSpaces();
            if (pos >= s.size())
                return '\0';
            return s[pos];
        }

        bool consume(char c)
        {
            skipSpaces();
            if (pos < s.size() && s[pos] == c)
            {
                ++pos;
                return true;
            }
            return false;
        }

        Expr parseAddSub()
        {
            Expr left = parseMulDiv();
            while (true)
            {
                char c = peek();
                if (c != '+' && c != '-')
                    break;
                ++pos;
                Expr right = parseMulDiv();
                if (left.kind != Expr::Sum)
                {
                    Expr sum(Expr::Sum);
                    sum.args.push_back(std::move(left));
                    left = std::move(sum);
                }
                if (c == '-')
                    left.args.emplace_back(Expr::Neg, std::move(right));
                else
                    left.args.push_back(std::move(right));
            }
            return left;
        }

        Expr parseMulDiv()
        {
            Expr left = parsePower();
            while (true)
            {
                char c = peek();
                if (c != '*' && c != '/')
                    break;
                ++pos;
                Expr right = parsePower();
                if (c == '/')
                {
                    left = Expr(Expr::Div, std::move(left), std::move(right));
                }
                else
                {
                    if (left.kind != Expr::Product)
                    {
                        Expr prod(Expr::Product);
                        prod.args.push_back(std::move(left));
                        left = std::move(prod);
                    }
                    left.args.push_back(std::move(right));
                }
            }
            return left;
        }

        Expr parsePower()
        {
            Expr left = parseUnary();
            if (peek() == '^')
            {
                ++pos;
                Expr right = parseUnary();
                left = Expr(Expr::Power, std::move(left), std::move(right));
            }
            while (consume('!'))
            {
                left = Expr(Expr::Func, "factorial", std::vector<Expr>{std::move(left)});
            }
            return left;
        }

        Expr parseUnary()
        {
            char c = peek();
            if (c == '+')
            {
                ++pos;
                return parseUnary();
            }
            if (c == '-')
            {
                ++pos;
                Expr operand = parseUnary();
                if (operand.kind == Expr::Number)
                {
                    if (!operand.value.empty() && operand.value[0] == '-')
                        operand.value = operand.value.substr(1);
                    else
                        operand.value = "-" + operand.value;
                    return operand;
                }
                return Expr(Expr::Neg, std::move(operand));
            }
            return parsePrimary();
        }

        Expr parsePrimary()
        {
            if (peek() == '(')
            {
                ++pos;
                Expr inside = parseAddSub();
                consume(')');
                return inside;
            }
            char c = peek();
            if (c == '\0')
                return Expr(Expr::Number, "0");
            if (std::isdigit(static_cast<unsigned char>(c)) || c == '.')
                return parseNumber();
            return parseIdentOrFunc();
        }

        Expr parseNumber()
        {
            skipSpaces();
            size_t start = pos;
            while (pos < s.size() && (std::isdigit(static_cast<unsigned char>(s[pos])) || s[pos] == '.'))
                ++pos;
            if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E'))
            {
                ++pos;
                if (pos < s.size() && (s[pos] == '+' || s[pos] == '-'))
                    ++pos;
                while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos])))
                    ++pos;
            }
            return Expr(Expr::Number, s.substr(start, pos - start));
        }

        Expr parseIdentOrFunc()
        {
            skipSpaces();
            size_t start = pos;
            while (pos < s.size())
            {
                char c = s[pos];
                if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' ||
                    c == '/' || c == '^' || c == ',' || c == '!' ||
                    std::isspace(static_cast<unsigned char>(c)))
                    break;
                ++pos;
            }
            std::string name = StripQuotes(s.substr(start, pos - start));
            if (consume('('))
            {
                std::vector<Expr> args;
                if (peek() != ')')
                {
                    args.push_back(parseAddSub());
                    while (consume(','))
                        args.push_back(parseAddSub());
                }
                consume(')');

                if (name == "pow" && args.size() == 2)
                    return Expr(Expr::Power, std::move(args[0]), std::move(args[1]));
                if (name == "sqrt" && args.size() == 1)
                    return Expr(Expr::Sqrt, std::move(args[0]));
                if (args.size() == 2 && name.size() > 3 &&
                    name.compare(name.size() - 3, 3, "pow") == 0)
                {
                    std::string base_name = name.substr(0, name.size() - 3);
                    return Expr(Expr::Power,
                        Expr(Expr::Func, base_name, std::vector<Expr>{std::move(args[0])}),
                        std::move(args[1]));
                }
                return Expr(Expr::Func, name, std::move(args));
            }
            return Expr(Expr::Ident, name);
        }
    };

    bool IsNumberString(const std::string& v)
    {
        static const std::regex re(R"(-?\d+(\.\d+)?([eE][+-]?\d+)?)");
        return std::regex_match(v, re);
    }

    bool IsIntegerString(const std::string& v)
    {
        static const std::regex re(R"(-?\d+)");
        return std::regex_match(v, re);
    }

    std::string StripDot(const std::string& v)
    {
        if (!v.empty() && v.back() == '.')
            return v.substr(0, v.size() - 1);
        return v;
    }

    bool IsOne(const std::string& v)
    {
        std::string t = StripDot(v);
        return t == "1" || t == "+1";
    }

    bool IsNumericFactor(const Expr& e)
    {
        return e.kind == Expr::Number &&
            (IsNumberString(e.value) || e.value.find('/') != std::string::npos);
    }

    std::string ReciprocalDecimal(const std::string& den, int precision, int exp)
    {
        Symbolic<Real>::MpfrGuard guard;
        mpfr_t den_mpfr, q;
        mpfr_init2(den_mpfr, 512);
        mpfr_init2(q, 512);
        mpfr_set_str(den_mpfr, den.c_str(), 10, MPFR_RNDN);
        mpfr_ui_div(q, 1, den_mpfr, MPFR_RNDN);

        double qd = mpfr_get_d(q, MPFR_RNDN);
        int order = 0;
        if (qd != 0.0 && std::isfinite(qd))
            order = static_cast<int>(std::floor(std::log10(std::fabs(qd)))) + 1;

        std::string result;
        if (order > exp && exp >= 0)
            result = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", precision, q));
        else
            result = Symbolic<Real>::FormatFixed(Symbolic<Real>::MpfrFormat("%.*Rf", precision, q), false);

        if (mpfr_zero_p(q) == 0 && Symbolic<Real>::IsEffectivelyZeroFixedStr(result))
            result = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", precision, q));

        mpfr_clear(den_mpfr);
        mpfr_clear(q);
        return result;
    }

    bool IsPolynomial(const Expr& e)
    {
        switch (e.kind)
        {
        case Expr::Number:
            return true;
        case Expr::Ident:
            return true;
        case Expr::Power:
            return e.args.size() == 2 &&
                IsPolynomial(e.args[0]) &&
                e.args[1].kind == Expr::Number &&
                IsIntegerString(StripDot(e.args[1].value));
        case Expr::Product:
        case Expr::Sum:
            for (const auto& a : e.args)
                if (!IsPolynomial(a))
                    return false;
            return true;
        case Expr::Neg:
            return IsPolynomial(e.args[0]);
        case Expr::Root:
            return false;
        default:
            return false;
        }
    }

    bool IsKnownConstant(const std::string& name)
    {
        static const std::set<std::string> known = {"i", "j", "e", "pi", "oo", "inf", "nan", "undef"};
        return known.find(name) != known.end();
    }

    bool IsVariableIdent(const std::string& name)
    {
        if (name.size() == 1 && std::isalpha(static_cast<unsigned char>(name[0])))
            return !IsKnownConstant(name);
        return false;
    }

    constexpr int NON_POLY_DEGREE = 100;

    int TermDegree(const Expr& e)
    {
        switch (e.kind)
        {
        case Expr::Number:
            return 0;
        case Expr::Ident:
            return IsVariableIdent(e.value) ? 1 : 0;
        case Expr::Power:
        {
            int base_deg = TermDegree(e.args[0]);
            if (base_deg == 0)
                return 0;
            if (e.args[1].kind == Expr::Number && IsIntegerString(StripDot(e.args[1].value)))
            {
                int exp_val = std::stoi(StripDot(e.args[1].value));
                return base_deg * exp_val;
            }
            return NON_POLY_DEGREE;
        }
        case Expr::Product:
        case Expr::Sum:
        {
            int d = 0;
            for (const auto& a : e.args)
                d += TermDegree(a);
            return d;
        }
        case Expr::Neg:
            return TermDegree(e.args[0]);
        case Expr::Div:
        {
            int num_deg = TermDegree(e.args[0]);
            int den_deg = TermDegree(e.args[1]);
            if (den_deg == 0)
                return num_deg;
            if (num_deg == 0)
                return NON_POLY_DEGREE;
            return num_deg; // keep polynomial degree for simple denominators
        }
        case Expr::Func:
        case Expr::Sqrt:
        case Expr::Root:
            return NON_POLY_DEGREE;
        default:
            return NON_POLY_DEGREE;
        }
    }

    bool IsConstantExpr(const Expr& e)
    {
        switch (e.kind)
        {
        case Expr::Number:
            return true;
        case Expr::Ident:
            return IsKnownConstant(e.value);
        case Expr::Func:
        case Expr::Sqrt:
        {
            for (const auto& a : e.args)
                if (!IsConstantExpr(a))
                    return false;
            return true;
        }
        case Expr::Power:
        case Expr::Div:
        case Expr::Neg:
            if (e.args.empty())
                return true;
            return IsConstantExpr(e.args[0]) && (e.args.size() < 2 || IsConstantExpr(e.args[1]));
        case Expr::Sum:
        case Expr::Product:
        {
            for (const auto& a : e.args)
                if (!IsConstantExpr(a))
                    return false;
            return true;
        }
        case Expr::Root:
            return false;
        default:
            return false;
        }
    }

    std::string MapSpecialIdent(const std::string& name)
    {
        std::string n = StripQuotes(name);
        if (n == "inf" || n == "infinity" || n == "oo")
            return "∞";
        if (n == "undef")
            return "nan";
        if (n == "pi")
            return "π";
        return n;
    }

    std::string MapFuncName(const std::string& name)
    {
        std::string n = StripQuotes(name);
        if (n == "Gamma" || n == "GAMMA")
            return "gamma";
        if (n == "yut_cot")
            return "cot";
        if (n == "yut_sec")
            return "sec";
        if (n == "yut_csc")
            return "csc";
        if (n == "yut_coth")
            return "coth";
        if (n == "yut_sech")
            return "sech";
        if (n == "yut_csch")
            return "csch";
        if (n == "yut_asinh")
            return "asinh";
        if (n == "yut_acosh")
            return "acosh";
        if (n == "yut_atanh")
            return "atanh";
        if (n == "yut_acoth")
            return "acoth";
        if (n == "yut_asech")
            return "asech";
        if (n == "yut_acsch")
            return "acsch";
        return n;
    }

    std::string EmitString(const Expr& e, const FormatCtx& ctx);
    std::string EvaluateGiacExpression(const std::string& expr, int precision);

    std::string StripTrailingDotZeros(std::string s)
    {
        if (s.find('.') == std::string::npos)
            return s;
        while (!s.empty() && s.back() == '0')
            s.pop_back();
        if (!s.empty() && s.back() == '.')
            s.pop_back();
        return s.empty() ? "0" : s;
    }

    std::string EmitNumber(const std::string& value, const FormatCtx& ctx)
    {
        if (ctx.mode == FormatCtx::Rational)
            return StripTrailingDotZeros(value);
        return AddDotIfInteger(RealNumberStr(value, ctx.precision, ctx.exp));
    }

    bool NeedsParensForPowerBase(const Expr& base, const Expr& exp)
    {
        (void)exp;
        if (base.kind == Expr::Number || base.kind == Expr::Ident ||
            base.kind == Expr::Func || base.kind == Expr::Sqrt)
            return false;
        if (base.kind == Expr::Product || base.kind == Expr::Sum ||
            base.kind == Expr::Power)
            return false;
        return true;
    }

    bool NeedsParensForPowerExp(const Expr& e)
    {
        if (e.kind == Expr::Number || e.kind == Expr::Ident ||
            e.kind == Expr::Func || e.kind == Expr::Sqrt)
            return false;
        if (e.kind == Expr::Neg && e.args[0].kind == Expr::Number)
            return false;
        return true;
    }

    std::string EmitFuncArgs(const Expr& func, const FormatCtx& ctx)
    {
        std::string result;
        for (size_t i = 0; i < func.args.size(); ++i)
        {
            if (i > 0)
                result += ",";
            result += EmitString(func.args[i], ctx);
        }
        return result;
    }

    std::string EmitString(const Expr& e, const FormatCtx& ctx)
    {
        switch (e.kind)
        {
        case Expr::Number:
            return EmitNumber(e.value, ctx);
        case Expr::Ident:
            return MapSpecialIdent(e.value);
        case Expr::Func:
        {
            std::string name = MapFuncName(e.value);
            if (name == "ln" && ctx.language != Language::Russian)
                name = "log";
            if ((name == "factorial") && e.args.size() == 1)
            {
                // x! is rendered as gamma(1+x)
                std::string arg = EmitString(e.args[0], ctx);
                return "gamma(1.+" + arg + ")";
            }
            // in text output, log/ln of a single numeric argument omits parentheses
            if ((name == "log" || name == "ln") && e.args.size() == 1 &&
                e.args[0].kind == Expr::Number && ctx.mode != FormatCtx::Rational)
            {
                return name + EmitString(e.args[0], ctx);
            }
            std::string s = name + "(";
            for (size_t i = 0; i < e.args.size(); ++i)
            {
                if (i > 0)
                    s += ",";
                s += EmitString(e.args[i], ctx);
            }
            s += ")";
            return s;
        }
        case Expr::Sqrt:
            return "sqrt(" + EmitString(e.args[0], ctx) + ")";
        case Expr::Root:
        {
            std::string base_str = EmitString(e.args[0], ctx);
            std::string exp_val = EvaluateGiacExpression("1/" + e.args[1].value, ctx.precision);
            return "pow(" + base_str + "," + EmitNumber(exp_val, ctx) + ")";
        }
        case Expr::Power:
        {
            std::string exp_str = EmitString(e.args[1], ctx);
            if (e.args[0].kind == Expr::Func)
            {
                std::string name = MapFuncName(e.args[0].value);
                return name + "pow(" + EmitFuncArgs(e.args[0], ctx) + "," + exp_str + ")";
            }
            // Preserve explicit right-associative chains as pow(base,exp1**exp2).
            if (e.args[1].kind == Expr::Power)
            {
                const Expr& inner = e.args[1];
                bool inner_simple =
                    (inner.args[0].kind == Expr::Ident || inner.args[0].kind == Expr::Number) &&
                    (inner.args[1].kind == Expr::Ident || inner.args[1].kind == Expr::Number);
                if (inner_simple)
                {
                    std::string base_str = EmitString(e.args[0], ctx);
                    if (NeedsParensForPowerBase(e.args[0], e.args[1]))
                        base_str = "(" + base_str + ")";
                    return "pow(" + base_str + "," +
                        EmitString(inner.args[0], ctx) + "**" + EmitString(inner.args[1], ctx) + ")";
                }
            }
            std::string base_str = EmitString(e.args[0], ctx);
            if (NeedsParensForPowerBase(e.args[0], e.args[1]))
                base_str = "(" + base_str + ")";
            if (NeedsParensForPowerExp(e.args[1]))
                exp_str = "(" + exp_str + ")";
            return "pow(" + base_str + "," + exp_str + ")";
        }
        case Expr::Div:
        {
            std::string num = EmitString(e.args[0], ctx);
            std::string den = EmitString(e.args[1], ctx);
            if (e.args[0].kind == Expr::Sum)
                num = "(" + num + ")";
            if (e.args[1].kind == Expr::Sum)
                den = "(" + den + ")";
            return num + "/" + den;
        }
        case Expr::Neg:
        {
            std::string operand = EmitString(e.args[0], ctx);
            if (e.args[0].kind == Expr::Sum || e.args[0].kind == Expr::Div ||
                e.args[0].kind == Expr::Power)
                operand = "(" + operand + ")";
            return "-" + operand;
        }
        case Expr::Sum:
        {
            std::string s = EmitString(e.args[0], ctx);
            for (size_t i = 1; i < e.args.size(); ++i)
            {
                if (e.args[i].kind == Expr::Neg)
                    s += "-" + EmitString(e.args[i].args[0], ctx);
                else
                    s += "+" + EmitString(e.args[i], ctx);
            }
            return s;
        }
        case Expr::Product:
        {
            auto emit_factor = [&](const Expr& f) -> std::string {
                std::string fs = EmitString(f, ctx);
                if (f.kind == Expr::Sum || f.kind == Expr::Div || f.kind == Expr::Neg ||
                    (ctx.mode == FormatCtx::Rational && f.kind == Expr::Number && f.value.find('/') != std::string::npos))
                    fs = "(" + fs + ")";
                return fs;
            };
            std::string s = emit_factor(e.args[0]);
            for (size_t i = 1; i < e.args.size(); ++i)
                s += "*" + emit_factor(e.args[i]);
            return s;
        }
        }
        return {};
    }

    void ReorderFactors(Expr& prod)
    {
        std::vector<Expr> numeric;
        std::vector<Expr> other;
        size_t n = prod.args.size();
        numeric.reserve(n);
        other.reserve(n);
        for (size_t i = 0; i < n; ++i)
        {
            if (IsNumericFactor(prod.args[i]))
                numeric.push_back(std::move(prod.args[i]));
            else
                other.push_back(std::move(prod.args[i]));
        }
        prod.args.clear();
        for (auto& x : numeric)
            prod.args.push_back(std::move(x));
        for (auto& x : other)
            prod.args.push_back(std::move(x));
    }

    std::string TermSortKey(const Expr& e, const FormatCtx& ctx)
    {
        if (e.kind == Expr::Product)
        {
            // ignore numeric/constant coefficients when ordering terms
            std::string key;
            for (const auto& f : e.args)
            {
                if (IsNumericFactor(f))
                    continue;
                if (IsConstantExpr(f))
                    continue;
                key += EmitString(f, ctx);
            }
            return key;
        }
        if (e.kind == Expr::Neg)
            return TermSortKey(e.args[0], ctx);
        if (e.kind == Expr::Number)
            return "";
        return EmitString(e, ctx);
    }

    int FunctionSortRank(const std::string& text)
    {
        // Canonical function ordering used by SymEngine for same-degree terms.
        static const std::vector<std::string> order = {
            "sin", "cos", "tan", "cot", "sec", "csc",
            "sinh", "cosh", "tanh", "coth", "sech", "csch",
            "asin", "acos", "atan", "acot", "asec", "acsc",
            "asinh", "acosh", "atanh", "acoth", "asech", "acsch"
        };
        std::string name = text;
        size_t paren = name.find('(');
        if (paren != std::string::npos)
            name = name.substr(0, paren);
        // powers of functions are emitted as sinpow(...), cospow(...)
        if (name.size() > 3 && name.substr(name.size() - 3) == "pow")
            name = name.substr(0, name.size() - 3);
        auto it = std::find(order.begin(), order.end(), name);
        if (it != order.end())
            return static_cast<int>(it - order.begin());
        return INT_MAX;
    }

    void SortTerms(Expr& sum, const FormatCtx& ctx)
    {
        struct Key
        {
            int degree = 0;
            int func_rank = INT_MAX;
            std::string text;
        };
        std::vector<Key> keys;
        keys.reserve(sum.args.size());
        for (const auto& a : sum.args)
        {
            std::string key = TermSortKey(a, ctx);
            keys.push_back({TermDegree(a), FunctionSortRank(key), std::move(key)});
        }

        std::vector<size_t> idx(sum.args.size());
        std::iota(idx.begin(), idx.end(), 0);
        std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
            if (keys[a].degree != keys[b].degree)
                return keys[a].degree < keys[b].degree;
            if (!ctx.json && keys[a].func_rank != keys[b].func_rank)
                return keys[a].func_rank < keys[b].func_rank;
            if (keys[a].degree == NON_POLY_DEGREE || keys[b].degree == NON_POLY_DEGREE)
                return ctx.json && keys[a].text < keys[b].text;
            return keys[a].text < keys[b].text;
        });

        std::vector<Expr> sorted(sum.args.size());
        for (size_t i = 0; i < idx.size(); ++i)
            sorted[i] = std::move(sum.args[idx[i]]);
        sum.args = std::move(sorted);
    }

    std::string AddCoeffs(const std::string& a, const std::string& b, const FormatCtx& ctx)
    {
        giac::context ct;
        giac::gen ag(a.c_str(), &ct);
        giac::gen bg(b.c_str(), &ct);
        giac::gen sum = ag + bg;
        if (ctx.mode == FormatCtx::Rational)
            sum = giac::normal(sum, &ct);
        else
            sum = giac::evalf(sum, 1, &ct);
        return sum.print(&ct);
    }

    std::string MultiplyCoeffs(const std::string& a, const std::string& b, const FormatCtx& ctx)
    {
        giac::context ct;
        giac::gen ag(a.c_str(), &ct);
        giac::gen bg(b.c_str(), &ct);
        giac::gen prod = ag * bg;
        if (ctx.mode == FormatCtx::Rational)
            prod = giac::normal(prod, &ct);
        else
            prod = giac::evalf(prod, 1, &ct);
        return prod.print(&ct);
    }

    void CombineLikeTerms(Expr& sum, const FormatCtx& ctx)
    {
        struct TermParts
        {
            std::string coeff;
            Expr body;
        };
        std::vector<TermParts> parts;
        parts.reserve(sum.args.size());

        for (auto& a : sum.args)
        {
            bool neg = false;
            Expr* cur = &a;
            if (cur->kind == Expr::Neg)
            {
                neg = true;
                cur = &cur->args[0];
            }

            std::string coeff;
            Expr body;
            if (cur->kind == Expr::Number)
            {
                coeff = (neg ? "-" : "") + cur->value;
                body = Expr(Expr::Number, "1");
            }
            else if (cur->kind == Expr::Product)
            {
                std::string num_coeff;
                std::vector<Expr> body_factors;
                for (auto& f : cur->args)
                {
                    if (IsNumericFactor(f))
                    {
                        if (num_coeff.empty())
                            num_coeff = f.value;
                        else
                            num_coeff = MultiplyCoeffs(num_coeff, f.value, ctx);
                    }
                    else
                    {
                        body_factors.push_back(std::move(f));
                    }
                }
                if (num_coeff.empty())
                    num_coeff = "1";
                if (neg && (num_coeff.empty() || num_coeff[0] != '-'))
                    num_coeff = "-" + num_coeff;
                coeff = num_coeff;
                if (body_factors.empty())
                    body = Expr(Expr::Number, "1");
                else if (body_factors.size() == 1)
                    body = std::move(body_factors[0]);
                else
                    body = Expr(Expr::Product, std::move(body_factors));
            }
            else
            {
                coeff = neg ? "-1" : "1";
                body = std::move(*cur);
            }
            parts.push_back({std::move(coeff), std::move(body)});
        }

        std::vector<std::string> keys;
        std::vector<std::string> coeffs;
        std::vector<Expr> bodies;
        for (auto& p : parts)
        {
            std::string key = EmitString(p.body, ctx);
            auto it = std::find(keys.begin(), keys.end(), key);
            if (it == keys.end())
            {
                keys.push_back(key);
                coeffs.push_back(std::move(p.coeff));
                bodies.push_back(std::move(p.body));
            }
            else
            {
                size_t idx = static_cast<size_t>(it - keys.begin());
                coeffs[idx] = AddCoeffs(coeffs[idx], p.coeff, ctx);
            }
        }

        std::vector<Expr> new_terms;
        for (size_t i = 0; i < bodies.size(); ++i)
        {
            if (coeffs[i] == "0")
                continue;
            if (bodies[i].kind == Expr::Number && bodies[i].value == "1")
            {
                new_terms.emplace_back(Expr::Number, coeffs[i]);
            }
            else if (coeffs[i] == "1")
            {
                new_terms.push_back(std::move(bodies[i]));
            }
            else if (coeffs[i] == "-1")
            {
                new_terms.emplace_back(Expr::Neg, std::move(bodies[i]));
            }
            else
            {
                Expr p(Expr::Product);
                p.args.emplace_back(Expr::Number, coeffs[i]);
                p.args.push_back(std::move(bodies[i]));
                new_terms.push_back(std::move(p));
            }
        }
        sum.args = std::move(new_terms);
    }

    bool IsConstantPower(const Expr& e)
    {
        return e.kind == Expr::Power &&
            e.args[0].kind == Expr::Number &&
            e.args[1].kind == Expr::Number &&
            IsIntegerString(StripDot(e.args[1].value));
    }

    std::string EvaluateConstantPower(const Expr& base, const Expr& exp, const FormatCtx& ctx)
    {
        Symbolic<Real>::MpfrGuard guard;
        mpfr_t r;
        mpfr_init2(r, 512);
        mpfr_set_str(guard.num, base.value.c_str(), 10, MPFR_RNDN);
        long e = std::stol(StripDot(exp.value));
        mpfr_pow_si(r, guard.num, e, MPFR_RNDN);

        double rd = mpfr_get_d(r, MPFR_RNDN);
        int order = 0;
        if (rd != 0.0 && std::isfinite(rd))
            order = static_cast<int>(std::floor(std::log10(std::fabs(rd)))) + 1;

        std::string s;
        if (order > ctx.exp && ctx.exp >= 0)
            s = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", ctx.precision, r));
        else
            s = Symbolic<Real>::FormatFixed(Symbolic<Real>::MpfrFormat("%.*Rf", ctx.precision, r), false);

        if (mpfr_zero_p(r) == 0 && Symbolic<Real>::IsEffectivelyZeroFixedStr(s))
            s = Symbolic<Real>::FormatScientific(Symbolic<Real>::MpfrFormat("%.*Re", ctx.precision, r));

        mpfr_clear(r);
        return s;
    }

    long long IntegerNthRootExact(long long base, int n)
    {
        if (n <= 1)
            return base;
        if (base == 0)
            return 0;
        bool negative = base < 0 && (n % 2 == 1);
        unsigned long long abs_base = negative
            ? -static_cast<unsigned long long>(base)
            : static_cast<unsigned long long>(base);
        if (abs_base <= 1)
            return base;
        long double approx = std::pow(static_cast<long double>(abs_base), 1.0L / n);
        long long r = static_cast<long long>(std::llround(approx));
        if (r < 1)
            r = 1;
        auto ipow = [&](long long x) -> __int128
        {
            __int128 res = 1;
            for (int i = 0; i < n; ++i)
                res *= x;
            return res;
        };
        for (long long cand = r - 1; cand <= r + 1; ++cand)
        {
            if (cand <= 0)
                continue;
            __int128 p = ipow(cand);
            if (p == static_cast<__int128>(abs_base))
                return negative ? -cand : cand;
        }
        return 0;
    }

    bool IsArithmeticConstantExpr(const Expr& e)
    {
        switch (e.kind)
        {
        case Expr::Number:
            return true;
        case Expr::Ident:
            return IsKnownConstant(e.value);
        case Expr::Func:
        case Expr::Sqrt:
            return false;
        case Expr::Power:
        case Expr::Div:
        case Expr::Neg:
            if (e.args.empty())
                return true;
            if (!IsArithmeticConstantExpr(e.args[0]))
                return false;
            return e.args.size() < 2 || IsArithmeticConstantExpr(e.args[1]);
        case Expr::Sum:
        case Expr::Product:
        {
            for (const auto& a : e.args)
                if (!IsArithmeticConstantExpr(a))
                    return false;
            return true;
        }
        case Expr::Root:
            return false;
        default:
            return false;
        }
    }

    std::string EvaluateGiacExpression(const std::string& expr, int precision)
    {
        giac::context ct;
        giac::decimal_digits(std::max(1, precision + 1), &ct);
        giac::gen g(expr.c_str(), &ct);
        g = giac::evalf(g, std::max(1, precision), &ct);
        if (g.type == giac::_CPLX)
            return "nan";
        if (g == giac::undef)
            return "nan";
        std::string s = g.print(&ct);
        if (s.find("undef") != std::string::npos ||
            s.find("error") != std::string::npos ||
            s.find("Error") != std::string::npos)
            return "nan";
        return s;
    }

    bool IsInertFunction(const std::string& name)
    {
        static const std::set<std::string> inert = {
            "cot", "sec", "csc", "coth", "sech", "csch",
            "asinh", "acosh", "atanh", "acoth", "asech", "acsch"
        };
        return inert.find(name) != inert.end();
    }

    std::string InertFunctionExpr(const std::string& name, const std::string& arg)
    {
        if (name == "cot")
            return "1/tan(" + arg + ")";
        if (name == "sec")
            return "1/cos(" + arg + ")";
        if (name == "csc")
            return "1/sin(" + arg + ")";
        if (name == "coth")
            return "1/tanh(" + arg + ")";
        if (name == "sech")
            return "1/cosh(" + arg + ")";
        if (name == "csch")
            return "1/sinh(" + arg + ")";
        if (name == "asinh")
            return "asinh(" + arg + ")";
        if (name == "acosh")
            return "acosh(" + arg + ")";
        if (name == "atanh")
            return "atanh(" + arg + ")";
        if (name == "acoth")
            return "atanh(1/(" + arg + "))";
        if (name == "asech")
            return "acosh(1/(" + arg + "))";
        if (name == "acsch")
            return "asinh(1/(" + arg + "))";
        return name + "(" + arg + ")";
    }

    bool IsGiacExpressionZero(const std::string& expr, int precision)
    {
        (void)precision;
        giac::context ct;
        giac::decimal_digits(60, &ct);
        giac::gen g(expr.c_str(), &ct);
        giac::gen n = giac::normal(g, &ct);
        if (giac::is_zero(n, &ct))
            return true;
        giac::gen f = giac::evalf(g, 60, &ct);
        if (f.type == giac::_CPLX)
            return false;
        giac::gen ab = giac::abs(f, &ct);
        ab = giac::evalf(ab, 3, &ct);
        std::string as = ab.print(&ct);
        try
        {
            double d = std::stod(as);
            return std::fabs(d) < 1e-6;
        }
        catch (...)
        {
        }
        return false;
    }

    std::string ExactInertArg(std::string arg)
    {
        for (size_t i = 0; i < arg.size(); )
        {
            if (arg[i] == '.' && (i + 1 >= arg.size() || !std::isdigit(static_cast<unsigned char>(arg[i + 1]))))
                arg.erase(i, 1);
            else
                ++i;
        }
        return arg;
    }

    bool IsInverseHyperbolicInertSingularity(const std::string& name, const std::string& arg, int precision)
    {
        if (name == "atanh" || name == "acoth")
        {
            if (IsGiacExpressionZero("(" + arg + ")-1", precision) ||
                IsGiacExpressionZero("(" + arg + ")+1", precision))
                return true;
        }
        if (name == "asech" || name == "acsch")
        {
            if (IsGiacExpressionZero(arg, precision))
                return true;
        }
        return false;
    }

    std::optional<Expr> InertFunctionSingularity(const std::string& name, const std::string& arg, int precision)
    {
        std::string exact = ExactInertArg(arg);
        if (name == "cot" && IsGiacExpressionZero("sin(" + exact + ")", precision))
            return Expr(Expr::Ident, "inf");
        if (name == "sec" && IsGiacExpressionZero("cos(" + exact + ")", precision))
            return Expr(Expr::Ident, "inf");
        if (name == "csc" && IsGiacExpressionZero("sin(" + exact + ")", precision))
            return Expr(Expr::Ident, "inf");
        if (name == "coth" && IsGiacExpressionZero("tanh(" + exact + ")", precision))
            return Expr(Expr::Ident, "inf");
        if (name == "csch" && IsGiacExpressionZero("sinh(" + exact + ")", precision))
            return Expr(Expr::Ident, "inf");
        return std::nullopt;
    }

    Expr ExprFromGiacResult(std::string s)
    {
        s = Symbolic<Real>::ReplaceAll(s, " ", "");
        s = Symbolic<Real>::ReplaceSqrtSymbol(s);
        if (!s.empty() && s.back() == '.')
            s.pop_back();
        if (s == "inf" || s == "+inf" || s == "+infinity" || s == "infinity")
            return Expr(Expr::Ident, "inf");
        if (s == "-inf" || s == "-infinity")
            return Expr(Expr::Neg, Expr(Expr::Ident, "inf"));
        if (s == "nan" || s == "undef")
            return Expr(Expr::Ident, "nan");
        return Expr(Expr::Number, s);
    }

    bool ContainsFunctionCall(const Expr& e)
    {
        switch (e.kind)
        {
        case Expr::Func:
        case Expr::Sqrt:
        case Expr::Root:
            return true;
        default:
            break;
        }
        for (const auto& a : e.args)
            if (ContainsFunctionCall(a))
                return true;
        return false;
    }

    bool ContainsImaginaryUnit(const Expr& e)
    {
        if (e.kind == Expr::Ident && (e.value == "i" || e.value == "j"))
            return true;
        for (const auto& a : e.args)
            if (ContainsImaginaryUnit(a))
                return true;
        return false;
    }

    Expr EvaluateArithmeticExpr(Expr e, const FormatCtx& ctx)
    {
        if (ContainsImaginaryUnit(e))
            return e;
        if (ctx.json && ContainsFunctionCall(e))
            return e;
        if (ctx.mode == FormatCtx::Rational)
        {
            std::string expr = EmitString(e, ctx);
            giac::context ct;
            giac::decimal_digits(std::max(1, ctx.precision + 1), &ct);
            giac::gen g(expr.c_str(), &ct);
            g = giac::normal(g, &ct);
            return ExprFromGiacResult(GiacToString(g, &ct));
        }
        std::string expr = EmitString(e, ctx);
        std::string val = EvaluateGiacExpression(expr, ctx.precision);
        return ExprFromGiacResult(val);
    }

    bool IsNegativeNumberString(const std::string& s)
    {
        return !s.empty() && s[0] == '-';
    }

    Expr NormalizeNegativeProductTerm(Expr e)
    {
        if (e.kind == Expr::Product)
        {
            bool negative = false;
            for (auto& f : e.args)
            {
                if (f.kind == Expr::Number && IsNegativeNumberString(f.value))
                {
                    f.value = f.value.substr(1);
                    negative = !negative;
                }
            }
            if (negative)
                return Expr(Expr::Neg, std::move(e));
        }
        else if (e.kind == Expr::Number && IsNegativeNumberString(e.value))
        {
            std::string v = e.value.substr(1);
            return Expr(Expr::Neg, Expr(Expr::Number, std::move(v)));
        }
        return e;
    }

    bool HasNegativeConstantTerm(const Expr& e)
    {
        if (e.kind != Expr::Sum)
            return false;
        for (const auto& a : e.args)
        {
            if (a.kind == Expr::Number && IsNegativeNumberString(a.value))
                return true;
            if (a.kind == Expr::Neg && a.args[0].kind == Expr::Number)
                return true;
        }
        return false;
    }

    Expr NegateSum(Expr e)
    {
        if (e.kind != Expr::Sum)
            return Expr(Expr::Neg, std::move(e));
        for (auto& a : e.args)
        {
            if (a.kind == Expr::Number)
            {
                if (IsNegativeNumberString(a.value))
                    a.value = a.value.substr(1);
                else
                    a.value = "-" + a.value;
            }
            else if (a.kind == Expr::Neg && a.args[0].kind == Expr::Number)
            {
                a = std::move(a.args[0]);
            }
            else
            {
                a = Expr(Expr::Neg, std::move(a));
            }
        }
        return e;
    }

    long long GcdLL(long long a, long long b)
    {
        a = std::llabs(a);
        b = std::llabs(b);
        while (b != 0)
        {
            long long t = a % b;
            a = b;
            b = t;
        }
        return a;
    }

    long long SquarefreePartLL(long long n)
    {
        n = std::llabs(n);
        long long result = 1;
        for (long long p = 2; p * p <= n; ++p)
        {
            if (n % p == 0)
            {
                int count = 0;
                while (n % p == 0)
                {
                    n /= p;
                    ++count;
                }
                if (count % 2 == 1)
                    result *= p;
            }
        }
        if (n > 1)
            result *= n;
        return result;
    }

    std::optional<Expr> TryFactorSqrtSum(const Expr& sum)
    {
        if (sum.kind != Expr::Sum || sum.args.size() < 2)
            return std::nullopt;
        std::vector<long long> radicands;
        std::vector<int> signs;
        for (const auto& a : sum.args)
        {
            int sign = 1;
            const Expr* term = &a;
            if (term->kind == Expr::Neg)
            {
                sign = -1;
                term = &term->args[0];
            }
            if (term->kind != Expr::Sqrt || term->args[0].kind != Expr::Number)
                return std::nullopt;
            std::string v = StripDot(term->args[0].value);
            if (!IsIntegerString(v))
                return std::nullopt;
            try
            {
                long long n = std::stoll(v);
                if (n <= 0)
                    return std::nullopt;
                radicands.push_back(n);
                signs.push_back(sign);
            }
            catch (...)
            {
                return std::nullopt;
            }
        }
        long long g = radicands[0];
        for (size_t i = 1; i < radicands.size(); ++i)
            g = GcdLL(g, radicands[i]);
        g = SquarefreePartLL(g);
        if (g <= 1)
            return std::nullopt;
        Expr inner(Expr::Sum);
        for (size_t i = 0; i < radicands.size(); ++i)
        {
            long long r = radicands[i] / g;
            Expr term;
            if (r == 1)
                term = Expr(Expr::Number, "1");
            else
                term = Expr(Expr::Sqrt, Expr(Expr::Number, std::to_string(r)));
            if (signs[i] < 0)
                term = Expr(Expr::Neg, std::move(term));
            inner.args.push_back(std::move(term));
        }
        Expr factor(Expr::Sqrt, Expr(Expr::Number, std::to_string(g)));
        return Expr(Expr::Product, std::vector<Expr>{
            std::move(factor), std::move(inner)});
    }

    Expr Transform(Expr e, const FormatCtx& ctx);

    Expr TransformChildren(Expr e, const FormatCtx& ctx)
    {
        for (auto& a : e.args)
            a = Transform(std::move(a), ctx);
        return e;
    }

    Expr Transform(Expr e, const FormatCtx& ctx)
    {
        switch (e.kind)
        {
        case Expr::Sum:
        {
            std::vector<Expr> terms;
            for (auto& a : e.args)
            {
                Expr t = Transform(std::move(a), ctx);
                if (t.kind == Expr::Sum)
                {
                    for (auto& st : t.args)
                        terms.push_back(std::move(st));
                }
                else
                {
                    terms.push_back(std::move(t));
                }
            }
            e.args = std::move(terms);
            if (IsArithmeticConstantExpr(e))
                return EvaluateArithmeticExpr(std::move(e), ctx);
            CombineLikeTerms(e, ctx);
            for (auto& term : e.args)
                term = NormalizeNegativeProductTerm(std::move(term));
            SortTerms(e, ctx);
            if (auto factored = TryFactorSqrtSum(e))
            {
                Expr r = Transform(std::move(*factored), ctx);
                return r;
            }
            return e;
        }
        case Expr::Product:
        {
            std::vector<Expr> factors;
            for (size_t ai = 0; ai < e.args.size(); ++ai)
            {
                Expr f = Transform(std::move(e.args[ai]), ctx);
                if (f.kind == Expr::Product)
                {
                    for (auto& sf : f.args)
                        factors.push_back(std::move(sf));
                }
                else
                {
                    factors.push_back(std::move(f));
                }
            }
            // Normalize a leading Neg(Number) into a negative number coefficient.
            for (auto& f : factors)
            {
                if (f.kind == Expr::Neg && f.args[0].kind == Expr::Number)
                {
                    std::string v = f.args[0].value;
                    if (!v.empty() && v[0] == '-')
                        v = v.substr(1);
                    else
                        v = "-" + v;
                    f = Expr(Expr::Number, v);
                }
            }
            // Merge sqrt/root factors into powers and combine powers with the same base.
            for (auto& f : factors)
            {
                if (f.kind == Expr::Sqrt && f.args[0].kind != Expr::Number)
                {
                    f = Expr(Expr::Power,
                        std::move(f.args[0]),
                        Expr(Expr::Div,
                            Expr(Expr::Number, "1"),
                            Expr(Expr::Number, "2")));
                }
                else if (f.kind == Expr::Root && f.args[0].kind != Expr::Number)
                {
                    f = Expr(Expr::Power,
                        std::move(f.args[0]),
                        Expr(Expr::Div,
                            Expr(Expr::Number, "1"),
                            Expr(Expr::Number, f.args[1].value)));
                }
            }
            {
                // Preserve the original factor order while still combining powers
                // that share the same base (SymEngine orders by complexity, not
                // lexicographically).
                std::vector<std::pair<std::string, std::vector<Expr>>> groups;
                std::vector<Expr> others;
                for (auto& f : factors)
                {
                    if (f.kind == Expr::Number)
                    {
                        others.push_back(std::move(f));
                        continue;
                    }
                    std::string key = (f.kind == Expr::Power)
                        ? EmitString(f.args[0], ctx)
                        : EmitString(f, ctx);
                    auto it = groups.begin();
                    for (; it != groups.end(); ++it)
                    {
                        if (it->first == key)
                            break;
                    }
                    if (it == groups.end())
                    {
                        groups.emplace_back(std::move(key), std::vector<Expr>{});
                        it = groups.end() - 1;
                    }
                    it->second.push_back(std::move(f));
                }
                factors.clear();
                for (auto& g : groups)
                {
                    if (g.second.size() == 1)
                    {
                        factors.push_back(std::move(g.second[0]));
                    }
                    else
                    {
                        Expr base = std::move(g.second[0]);
                        bool base_is_power = base.kind == Expr::Power;
                        Expr real_base = base_is_power ? std::move(base.args[0]) : std::move(base);
                        Expr sum(Expr::Sum);
                        if (base_is_power)
                            sum.args.push_back(std::move(base.args[1]));
                        else
                            sum.args.emplace_back(Expr::Number, "1");
                        for (size_t i = 1; i < g.second.size(); ++i)
                        {
                            Expr& f = g.second[i];
                            if (f.kind == Expr::Power)
                                sum.args.push_back(std::move(f.args[1]));
                            else
                                sum.args.emplace_back(Expr::Number, "1");
                        }
                        factors.emplace_back(Expr::Power,
                            std::move(real_base),
                            Transform(std::move(sum), ctx));
                    }
                }
                factors.insert(factors.end(), std::make_move_iterator(others.begin()), std::make_move_iterator(others.end()));
            }
            bool negative = false;
            if (ctx.json)
            {
                // Flatten negated products so the minus sign can be hoisted to the outer product.
                {
                    std::vector<Expr> flat;
                    for (auto& f : factors)
                    {
                        if (f.kind == Expr::Neg && f.args[0].kind == Expr::Product)
                        {
                            negative = !negative;
                            for (auto& sf : f.args[0].args)
                                flat.push_back(std::move(sf));
                        }
                        else
                        {
                            flat.push_back(std::move(f));
                        }
                    }
                    factors = std::move(flat);
                }
                for (auto& f : factors)
                {
                    if (f.kind == Expr::Number && IsNegativeNumberString(f.value))
                    {
                        negative = !negative;
                        f.value = f.value.substr(1);
                    }
                    else if (f.kind == Expr::Sum && HasNegativeConstantTerm(f))
                    {
                        negative = !negative;
                        f = NegateSum(std::move(f));
                    }
                }
            }
            e.args = std::move(factors);
            ReorderFactors(e);
            bool is_arith = IsArithmeticConstantExpr(e);
            if (is_arith)
            {
                Expr r = EvaluateArithmeticExpr(std::move(e), ctx);
                if (r.kind == Expr::Number && IsNegativeNumberString(r.value))
                {
                    negative = !negative;
                    r.value = r.value.substr(1);
                }
                if (negative)
                    return Expr(Expr::Neg, std::move(r));
                return r;
            }
            if (negative)
                return Expr(Expr::Neg, std::move(e));
            return e;
        }
        case Expr::Div:
        {
            Expr num = Transform(std::move(e.args[0]), ctx);
            Expr den = Transform(std::move(e.args[1]), ctx);
            if (num.kind == Expr::Neg && num.args[0].kind == Expr::Number)
            {
                Expr inner(Expr::Div, std::move(num.args[0]), std::move(den));
                return Transform(Expr(Expr::Neg, Transform(std::move(inner), ctx)), ctx);
            }
            if (num.kind == Expr::Number && IsNegativeNumberString(num.value))
            {
                Expr n(Expr::Number, num.value.substr(1));
                Expr inner(Expr::Div, std::move(n), std::move(den));
                return Transform(Expr(Expr::Neg, Transform(std::move(inner), ctx)), ctx);
            }
            {
                Expr tmp(Expr::Div, Expr(num), Expr(den));
                if (IsArithmeticConstantExpr(tmp))
                    return EvaluateArithmeticExpr(std::move(tmp), ctx);
            }
            if (IsOne(num.value))
                return Expr(Expr::Power, std::move(den), Expr(Expr::Number, "-1"));
            if (IsConstantPower(den))
            {
                std::string den_val = EvaluateConstantPower(den.args[0], den.args[1], ctx);
                Expr tmp(Expr::Div, std::move(num), Expr(Expr::Number, den_val));
                return Transform(std::move(tmp), ctx);
            }
            if (IsNumericFactor(den))
            {
                std::string rec;
                if (ctx.mode == FormatCtx::Rational)
                    rec = "1/" + den.value;
                else
                    rec = ReciprocalDecimal(den.value, ctx.precision, ctx.exp);
                if (num.kind == Expr::Product)
                {
                    std::vector<Expr> numeric;
                    std::vector<Expr> other;
                    for (auto& f : num.args)
                    {
                        if (IsNumericFactor(f))
                            numeric.push_back(std::move(f));
                        else
                            other.push_back(std::move(f));
                    }
                    if (!numeric.empty())
                    {
                        Expr num_prod = numeric.size() == 1
                            ? std::move(numeric[0])
                            : Expr(Expr::Product, std::move(numeric));
                        Expr frac(Expr::Div, std::move(num_prod), std::move(den));
                        Expr p(Expr::Product);
                        p.args.push_back(Transform(std::move(frac), ctx));
                        for (auto& o : other)
                            p.args.push_back(std::move(o));
                        return Transform(std::move(p), ctx);
                    }
                    Expr p(Expr::Product);
                    for (auto& o : other)
                        p.args.push_back(std::move(o));
                    p.args.emplace_back(Expr::Number, rec);
                    return Transform(std::move(p), ctx);
                }
                Expr p(Expr::Product);
                p.args.push_back(std::move(num));
                p.args.emplace_back(Expr::Number, rec);
                return Transform(std::move(p), ctx);
            }
            return Expr(Expr::Div, std::move(num), std::move(den));
        }
        case Expr::Power:
        {
            // Integer perfect roots: 8^(1/3) -> 2.
            if (e.args[0].kind == Expr::Number &&
                e.args[1].kind == Expr::Div &&
                e.args[1].args.size() == 2 &&
                e.args[1].args[0].kind == Expr::Number &&
                StripDot(e.args[1].args[0].value) == "1" &&
                e.args[1].args[1].kind == Expr::Number &&
                IsIntegerString(StripDot(e.args[1].args[1].value)))
            {
                int n = std::stoi(StripDot(e.args[1].args[1].value));
                if (n > 1)
                {
                    try
                    {
                        std::string base_str = StripDot(e.args[0].value);
                        size_t pos = 0;
                        long long base_val = std::stoll(base_str, &pos);
                        if (pos == base_str.size())
                        {
                            long long root = IntegerNthRootExact(base_val, n);
                            if (root != 0)
                                return Expr(Expr::Number, std::to_string(root));
                        }
                    }
                    catch (...)
                    {
                    }
                }
            }
            // x^(1/n) is rendered as an n-th root in JSON. Detect it before
            // the exponent is evaluated to a decimal approximation.
            if (ctx.json &&
                e.args[0].kind != Expr::Number &&
                e.args[1].kind == Expr::Div &&
                e.args[1].args.size() == 2 &&
                e.args[1].args[0].kind == Expr::Number &&
                StripDot(e.args[1].args[0].value) == "1" &&
                e.args[1].args[1].kind == Expr::Number &&
                IsIntegerString(StripDot(e.args[1].args[1].value)))
            {
                int n = std::stoi(StripDot(e.args[1].args[1].value));
                if (n > 1)
                {
                    Expr base = Transform(std::move(e.args[0]), ctx);
                    return Expr(Expr::Root, std::vector<Expr>{
                        std::move(base),
                        Expr(Expr::Number, std::to_string(n))});
                }
            }
            e.args[0] = Transform(std::move(e.args[0]), ctx);
            e.args[1] = Transform(std::move(e.args[1]), ctx);
            // Giac flattens (x^y)^z into x^(y*z). Reconstruct the nested power
            // so that left-associative chains are printed as pow(pow(x,y),z).
            if (e.args[1].kind == Expr::Product && e.args[1].args.size() == 2)
            {
                Expr inner(Expr::Power,
                    std::move(e.args[0]),
                    std::move(e.args[1].args[0]));
                Expr outer(Expr::Power,
                    std::move(inner),
                    std::move(e.args[1].args[1]));
                return Transform(std::move(outer), ctx);
            }
            // abs(A)^N -> pow(A,N); abs(A)^B outer -> pow(pow(A,N?),B)...
            // The parser turns abspow(A,B) into pow(abs(A),B). If there is an
            // additional outer power N, distribute it to A: pow(pow(A,N),B).
            if (e.args[0].kind == Expr::Power &&
                e.args[0].args[0].kind == Expr::Func &&
                e.args[0].args[0].value == "abs" &&
                e.args[0].args[0].args.size() == 1 &&
                e.args[1].kind == Expr::Number &&
                IsIntegerString(StripDot(e.args[1].value)))
            {
                Expr a = std::move(e.args[0].args[0].args[0]);
                Expr b = std::move(e.args[0].args[1]);
                Expr n = std::move(e.args[1]);
                Expr new_inner(Expr::Power, std::move(a), std::move(n));
                return Transform(Expr(Expr::Power,
                    std::move(new_inner),
                    std::move(b)), ctx);
            }
            // abs(X)^N -> pow(X,N)
            if (e.args[0].kind == Expr::Func &&
                e.args[0].value == "abs" &&
                e.args[0].args.size() == 1 &&
                e.args[1].kind == Expr::Number &&
                IsIntegerString(StripDot(e.args[1].value)))
            {
                Expr base = std::move(e.args[0].args[0]);
                Expr exp = std::move(e.args[1]);
                return Transform(Expr(Expr::Power,
                    std::move(base),
                    std::move(exp)), ctx);
            }
            if (IsConstantPower(e))
            {
                std::string val = EvaluateConstantPower(e.args[0], e.args[1], ctx);
                return Expr(Expr::Number, val);
            }
            if (IsArithmeticConstantExpr(e))
                return EvaluateArithmeticExpr(std::move(e), ctx);
            return e;
        }
        case Expr::Neg:
        {
            e.args[0] = Transform(std::move(e.args[0]), ctx);
            if (e.args[0].kind == Expr::Neg)
                return std::move(e.args[0].args[0]);
            if (IsArithmeticConstantExpr(e))
                return EvaluateArithmeticExpr(std::move(e), ctx);
            if (e.args[0].kind == Expr::Number)
            {
                std::string v = e.args[0].value;
                if (!v.empty() && v[0] == '-')
                    v = v.substr(1);
                else
                    v = "-" + v;
                return Expr(Expr::Number, v);
            }
            return e;
        }
        case Expr::Func:
        {
            std::string mapped_name = MapFuncName(e.value);
            std::string raw_arg_str;
            bool inert_constant = (e.args.size() == 1 &&
                IsInertFunction(mapped_name) && IsConstantExpr(e.args[0]));
            if (inert_constant)
                raw_arg_str = EmitString(e.args[0], ctx);
            for (auto& a : e.args)
                a = Transform(std::move(a), ctx);
            e.value = std::move(mapped_name);
            if ((e.value == "cos" || e.value == "cosh") &&
                e.args.size() == 1 &&
                e.args[0].kind == Expr::Sum &&
                HasNegativeConstantTerm(e.args[0]))
            {
                e.args[0] = NegateSum(std::move(e.args[0]));
                return Transform(std::move(e), ctx);
            }
            if (e.value == "factorial" && e.args.size() == 1)
            {
                Expr sum(Expr::Sum);
                sum.args.emplace_back(Expr::Number, "1");
                sum.args.push_back(std::move(e.args[0]));
                return Expr(Expr::Func, "gamma", std::vector<Expr>{std::move(sum)});
            }
            if (inert_constant)
            {
                if (IsInverseHyperbolicInertSingularity(e.value, raw_arg_str, ctx.precision))
                    return e;
                auto singular = InertFunctionSingularity(e.value, raw_arg_str, ctx.precision);
                if (singular)
                    return *singular;
                std::string expr = InertFunctionExpr(e.value, EmitString(e.args[0], ctx));
                if (ctx.mode == FormatCtx::Rational)
                {
                    giac::context ct;
                    giac::decimal_digits(std::max(1, ctx.precision + 1), &ct);
                    giac::gen g(expr.c_str(), &ct);
                    g = giac::normal(g, &ct);
                    return ExprFromGiacResult(GiacToString(g, &ct));
                }
                std::string val = EvaluateGiacExpression(expr, ctx.precision);
                return ExprFromGiacResult(val);
            }
            return e;
        }
        case Expr::Sqrt:
        {
            e.args[0] = Transform(std::move(e.args[0]), ctx);
            return e;
        }
        default:
            return e;
        }
    }

    Expr BuildFormattedAst(std::string s, const FormatCtx& ctx)
    {
        s = Symbolic<Real>::ReplaceSqrtSymbol(s);
        s = Symbolic<Real>::ReplacePowerOperator(s);
        s = Symbolic<Real>::ReplaceCaretOperator(s);
        s = Symbolic<Real>::ReplaceAll(s, " ", "");
        s = Symbolic<Real>::ReplaceAll(s, "+infinity", "oo");
        s = Symbolic<Real>::ReplaceAll(s, "-infinity", "-oo");
        s = Symbolic<Real>::ReplaceAll(s, "undef", "nan");

        Expr ast = ExprParser(s).parse();
        return Transform(std::move(ast), ctx);
    }

    std::string FormatGiacString(std::string s, const FormatCtx& ctx)
    {
        Expr ast = BuildFormattedAst(std::move(s), ctx);
        std::string result = EmitString(ast, ctx);
        if (ctx.language != Language::Russian)
            result = Symbolic<Real>::ReplaceAll(result, "ln(", "log(");
        return result;
    }

    // JSON emission

    std::string JsonComma()
    {
        return R"({"type":18,"symbol":","})";
    }

    // Returns the JSON element for the absolute value of a number.
    // The caller is responsible for emitting a leading minus when the value is negative.
    std::vector<std::string> JsonNumberRaw(const std::string& value, const FormatCtx& ctx)
    {
        if (ctx.mode == FormatCtx::Rational)
        {
            std::string pos = !value.empty() && value[0] == '-' ? value.substr(1) : value;
            pos = StripTrailingDotZeros(pos);
            size_t slash = pos.find('/');
            if (slash != std::string::npos)
            {
                std::string num = pos.substr(0, slash);
                std::string den = pos.substr(slash + 1);
                return {Symbolic<Real>::JsonDivision(
                    Symbolic<Real>::JsonCodeRow({Symbolic<Real>::JsonCodeString(num)}),
                    Symbolic<Real>::JsonCodeRow({Symbolic<Real>::JsonCodeString(den)}))};
            }
            return {Symbolic<Real>::JsonCodeString(pos)};
        }

        std::string formatted = RealNumberStr(value, ctx.precision, ctx.exp);
        if (formatted.find_first_of("eE") != std::string::npos)
        {
            return Symbolic<Real>::JsonScientificElements(formatted);
        }
        std::string pos = !formatted.empty() && formatted[0] == '-' ? formatted.substr(1) : formatted;
        // strip trailing dot from integers in JSON
        if (!pos.empty() && pos.back() == '.')
        {
            bool all_digits = true;
            for (char c : pos)
            {
                if (c == '-')
                    continue;
                if (!std::isdigit(static_cast<unsigned char>(c)))
                {
                    all_digits = false;
                    break;
                }
            }
            if (all_digits)
                pos.pop_back();
        }
        return {Symbolic<Real>::JsonCodeString(pos)};
    }

    bool IsNegativeNumberValue(const std::string& value)
    {
        return !value.empty() && value[0] == '-';
    }

    std::vector<std::string> EmitJson(const Expr& e, const FormatCtx& ctx);
    bool IsCodeStringJson(const std::string& s);

    std::vector<std::string> EmitJsonChildren(const std::vector<Expr>& args, const FormatCtx& ctx)
    {
        std::vector<std::string> result;
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i > 0)
                result.push_back(JsonComma());
            auto child = EmitJson(args[i], ctx);
            result.insert(result.end(), child.begin(), child.end());
        }
        return result;
    }

    std::vector<std::string> EmitJson(const Expr& e, const FormatCtx& ctx)
    {
        switch (e.kind)
        {
        case Expr::Number:
        {
            auto raw = JsonNumberRaw(e.value, ctx);
            if (IsNegativeNumberValue(e.value))
            {
                // scientific notation already includes the minus sign in its elements
                if (!raw.empty() && raw[0].find(R"({"type":12,)") == 0)
                    return raw;
                std::vector<std::string> res;
                res.reserve(raw.size() + 1);
                res.push_back(Symbolic<Real>::JsonMinus());
                res.insert(res.end(), raw.begin(), raw.end());
                return res;
            }
            return raw;
        }
        case Expr::Ident:
            return {Symbolic<Real>::JsonCodeString(MapSpecialIdent(e.value))};
        case Expr::Func:
        {
            std::string name = MapFuncName(e.value);
            if (ctx.language != Language::Russian && name == "ln")
                name = "log";
            if (name == "exp" && e.args.size() == 1)
            {
                return {Symbolic<Real>::JsonPower(
                    Symbolic<Real>::JsonCodeRow({Symbolic<Real>::JsonCodeString("E")}),
                    Symbolic<Real>::JsonCodeRow(EmitJson(e.args[0], ctx)))};
            }
            std::vector<std::string> elems;
            elems.push_back(Symbolic<Real>::JsonCodeString(name));
            elems.push_back(Symbolic<Real>::JsonOpenRoundBracket());
            auto args = EmitJsonChildren(e.args, ctx);
            elems.insert(elems.end(), args.begin(), args.end());
            elems.push_back(Symbolic<Real>::JsonCloseRoundBracket());
            return elems;
        }
        case Expr::Sqrt:
        {
            std::string base_row = Symbolic<Real>::JsonCodeRow(EmitJson(e.args[0], ctx));
            return {Symbolic<Real>::JsonSquareRoot(base_row)};
        }
        case Expr::Root:
        {
            std::string n_row = Symbolic<Real>::JsonCodeRow({Symbolic<Real>::JsonCodeString(e.args[1].value)});
            std::string base_row = Symbolic<Real>::JsonCodeRow(EmitJson(e.args[0], ctx));
            return {Symbolic<Real>::JsonNthRoot(n_row, base_row)};
        }
        case Expr::Power:
        {
            auto base_elems = EmitJson(e.args[0], ctx);
            bool base_needs_parens = e.args[0].kind == Expr::Sum || e.args[0].kind == Expr::Product ||
                                     e.args[0].kind == Expr::Div || e.args[0].kind == Expr::Neg ||
                                     e.args[0].kind == Expr::Power || e.args[0].kind == Expr::Root;
            std::string base_inner;
            if (base_elems.size() == 1 && !IsCodeStringJson(base_elems[0]))
                base_inner = base_elems[0];
            else
                base_inner = Symbolic<Real>::JsonCodeRow(base_elems);
            std::string base_row;
            if (base_needs_parens)
            {
                base_row = Symbolic<Real>::JsonCodeRow({
                    Symbolic<Real>::JsonOpenRoundBracket(),
                    base_inner,
                    Symbolic<Real>::JsonCloseRoundBracket()});
                if (ctx.in_product)
                    base_row = Symbolic<Real>::JsonCodeRow({base_row});
            }
            else
            {
                base_row = base_inner;
            }
            auto exp_elems = EmitJson(e.args[1], ctx);
            std::string exp_row;
            if (exp_elems.size() == 1 && !IsCodeStringJson(exp_elems[0]))
                exp_row = exp_elems[0];
            else
                exp_row = Symbolic<Real>::JsonCodeRow(exp_elems);
            return {Symbolic<Real>::JsonPower(base_row, exp_row)};
        }
        case Expr::Div:
        {
            auto wrap_operand = [&](const Expr& op) -> std::string {
                auto inner = EmitJson(op, ctx);
                std::string elem;
                if (inner.size() == 1)
                    elem = inner[0];
                else
                    elem = Symbolic<Real>::JsonCodeRow(inner);
                //avoid nested CodeRow if the operand already produced one
                if (elem.size() >= 10 && elem.substr(0, 10) == R"({"type":7,)")
                    return elem;
                return Symbolic<Real>::JsonCodeRow({elem});
            };
            return {Symbolic<Real>::JsonDivision(
                wrap_operand(e.args[0]),
                wrap_operand(e.args[1]))};
        }
        case Expr::Neg:
        {
            std::vector<std::string> res;
            res.push_back(Symbolic<Real>::JsonMinus());
            auto inner = EmitJson(e.args[0], ctx);
            if (inner.size() > 1 && e.args[0].kind != Expr::Product)
                inner = {Symbolic<Real>::JsonCodeRow(inner)};
            res.insert(res.end(), inner.begin(), inner.end());
            return res;
        }
        case Expr::Sum:
        {
            std::vector<std::string> res = EmitJson(e.args[0], ctx);
            for (size_t i = 1; i < e.args.size(); ++i)
            {
                if (e.args[i].kind == Expr::Neg)
                {
                    res.push_back(Symbolic<Real>::JsonMinus());
                    auto inner = EmitJson(e.args[i].args[0], ctx);
                    res.insert(res.end(), inner.begin(), inner.end());
                }
                else
                {
                    res.push_back(Symbolic<Real>::JsonPlus());
                    auto term = EmitJson(e.args[i], ctx);
                    res.insert(res.end(), term.begin(), term.end());
                }
            }
            return res;
        }
        case Expr::Product:
        {
            FormatCtx product_ctx = ctx;
            product_ctx.in_product = true;
            auto emit_factor = [&](const Expr& f) -> std::vector<std::string> {
                auto elems = EmitJson(f, product_ctx);
                if (f.kind == Expr::Sum)
                {
                    std::string row = Symbolic<Real>::JsonCodeRow(elems);
                    if (!ContainsImaginaryUnit(f))
                    {
                        return {Symbolic<Real>::JsonCodeRow({
                            Symbolic<Real>::JsonOpenRoundBracket(),
                            row,
                            Symbolic<Real>::JsonCloseRoundBracket()})};
                    }
                    return {row};
                }
                return elems;
            };
            std::vector<std::string> res = emit_factor(e.args[0]);
            for (size_t i = 1; i < e.args.size(); ++i)
            {
                res.push_back(Symbolic<Real>::JsonMultiply());
                auto f = emit_factor(e.args[i]);
                res.insert(res.end(), f.begin(), f.end());
            }
            return res;
        }
        }
        return {};
    }

    bool IsCodeStringJson(const std::string& s)
    {
        return s.size() >= 10 && s.substr(0, 10) == R"({"type":8,)";
    }

    std::string ExprToJson(const Expr& ast, int result_type, const FormatCtx& ctx)
    {
        auto elems = EmitJson(ast, ctx);
        if (elems.size() == 1 && !IsCodeStringJson(elems[0]))
            return Symbolic<Real>::JsonResultWrapper(result_type, elems[0]);
        return Symbolic<Real>::JsonResultWrapper(result_type,
            Symbolic<Real>::JsonCodeRow(elems));
    }
}

namespace detail
{
    bool IsInfiniteLimit(const giac::gen& lim, giac::context* ctx)
    {
        if (lim == giac::plus_inf || lim == giac::minus_inf)
            return true;
        if (lim.type == giac::_IDNT)
        {
            std::string s = lim.print(ctx);
            if (s == "undef" || s == "unsigned_inf" || s == "infinity")
                return true;
        }
        return false;
    }

    bool HasAmbiguousPoleArgument(const giac::gen& expr, const giac::identificateur& var, const giac::gen& value, giac::context* ctx)
    {
        if (expr.type == giac::_SYMB)
        {
            const giac::symbolic& s = *expr._SYMBptr;
            bool ambiguous = (s.sommet == giac::at_exp || s.sommet == giac::at_sin || s.sommet == giac::at_cos ||
                              s.sommet == giac::at_sinh || s.sommet == giac::at_cosh);
            if (ambiguous)
            {
                giac::gen lim = giac::limit(s.feuille, var, value, 1, ctx);
                if (IsInfiniteLimit(lim, ctx))
                    return true;
            }
            return HasAmbiguousPoleArgument(s.feuille, var, value, ctx);
        }
        if (expr.type == giac::_FRAC)
        {
            return HasAmbiguousPoleArgument(expr._FRACptr->num, var, value, ctx) ||
                   HasAmbiguousPoleArgument(expr._FRACptr->den, var, value, ctx);
        }
        if (expr.type == giac::_CPLX)
        {
            return HasAmbiguousPoleArgument(expr._CPLXptr[0], var, value, ctx) ||
                   HasAmbiguousPoleArgument(expr._CPLXptr[1], var, value, ctx);
        }
        if (expr.type == giac::_VECT)
        {
            const giac::vecteur& v = *expr._VECTptr;
            for (const auto& e : v)
                if (HasAmbiguousPoleArgument(e, var, value, ctx))
                    return true;
            return false;
        }
        return false;
    }
}

namespace
{
    bool IsExplicitNegativeInfinity(const giac::gen& g)
    {
        return g == giac::minus_inf;
    }
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
    if (!e.is_integer() && e.type != giac::_REAL && e.type != giac::_DOUBLE_ &&
        !detail::HasUnknownSymbol(e))
    {
        e = giac::evalf(e, 1, &context);
    }
    std::string s = e.print(&context);
    // Yutovo convention: ln(0) is +infinity, not -infinity
    if ((s == "-inf" || s == "-infinity" || s == "-oo") &&
        !explicit_negative_infinity)
    {
        s = "oo";
    }
    // real sqrt of -infinity is undefined
    if (expr->type == giac::_SYMB && expr->_SYMBptr->sommet == giac::at_sqrt)
    {
        giac::gen arg = giac::eval(expr->_SYMBptr->feuille, 1, &context);
        std::string arg_str = arg.print(&context);
        if (arg_str.find("-inf") != std::string::npos ||
            arg_str.find("-infinity") != std::string::npos ||
            arg_str.find("-oo") != std::string::npos)
        {
            s = "nan";
        }
    }

    if (e.is_integer())
    {
        std::string int_str = e.print(&context);
        if (int_str.size() < 6)
            return AddDotIfInteger(RealNumberStr(int_str, precision, exp));
        return int_str;
    }

    FormatCtx ctx{FormatCtx::Real, precision, exp, language, false};
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

    FormatCtx ctx{FormatCtx::Rational, precision, exp, language, false};
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
        e.type != giac::_CPLX && !detail::HasUnknownSymbol(e))
    {
        e = giac::evalf(e, 1, &context);
    }
    if (e.is_integer())
        return e.print(&context);
    std::string s = e.print(&context);

    s = Symbolic<Real>::ReplaceSqrtSymbol(s);
    char imag_unit = (language == Language::Russian) ? 'j' : 'i';
    s = Symbolic<Real>::ReplaceImaginaryUnit(s, imag_unit);

    FormatCtx ctx{FormatCtx::Complex, precision, exp, language, false};
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
    FormatCtx ctx{FormatCtx::Real, precision, exp, language, true};
    Expr ast = BuildFormattedAst(std::move(s), ctx);
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
    FormatCtx ctx{FormatCtx::Rational, precision, exp, language, true};
    Expr ast = BuildFormattedAst(std::move(s), ctx);
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

    FormatCtx ctx{FormatCtx::Complex, precision, exp, language, true};
    Expr ast = BuildFormattedAst(std::move(s), ctx);
    return ExprToJson(ast, 47, ctx);
}

template class Symbolic<Rational>;
template class Symbolic<Complex>;

}
