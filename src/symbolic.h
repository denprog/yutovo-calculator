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
#include <symengine/expression.h>
#include <symengine/parser.h>
#include <symengine/symbol.h>
#include <symengine/eval_mpfr.h>
#include <symengine/eval_mpc.h>
#include <symengine/real_mpfr.h>
#include <symengine/basic.h>
#include <symengine/functions.h>
#include "math_helper.h"
#include "parser_exception.h"
#include "real.h"
#include "complex.h"
#include "rational.h"

namespace yutovo_calculator
{

class Symbolic
{
public:
    Symbolic();
    explicit Symbolic(int precision);
    explicit Symbolic(int precision, int num);
    explicit Symbolic(int precision, float num);
    explicit Symbolic(int precision, const std::u32string& num);
    explicit Symbolic(int precision, const std::string& num);
    explicit Symbolic(int precision, const Real& num);
    explicit Symbolic(int precision, const Complex& num);
    explicit Symbolic(int precision, const Rational& num);
    Symbolic(const Symbolic& source);
    
public:
    typedef Symbolic value_type;

    Symbolic& operator=(const Symbolic& source);
    Symbolic& operator=(const int num);
    Symbolic& operator=(const double num);
    Symbolic& operator=(const std::u32string& num);
    Symbolic& operator=(const Real& num);
    Symbolic& operator=(const Complex& num);
    Symbolic& operator=(const Rational& num);

    Symbolic operator+() const;
    Symbolic operator-() const;

    friend Symbolic operator+(const Symbolic& num1, const Symbolic& num2);
    friend Symbolic operator+(const Symbolic& num1, const int num2);
    friend Symbolic operator+(const int num1, const Symbolic& num2);
    friend Symbolic operator+(const Symbolic& num1, const Real& num2);
    friend Symbolic operator+(const Real& num1, const Symbolic& num2);
    friend Symbolic operator+(const Symbolic& num1, const Complex& num2);
    friend Symbolic operator+(const Complex& num1, const Symbolic& num2);
    friend Symbolic operator+(const Symbolic& num1, const Rational& num2);
    friend Symbolic operator+(const Rational& num1, const Symbolic& num2);

    friend Symbolic operator-(const Symbolic& num1, const Symbolic& num2);
    friend Symbolic operator-(const Symbolic& num1, const int num2);
    friend Symbolic operator-(const int num1, const Symbolic& num2);
    friend Symbolic operator-(const Symbolic& num1, const Real& num2);
    friend Symbolic operator-(const Real& num1, const Symbolic& num2);
    friend Symbolic operator-(const Symbolic& num1, const Complex& num2);
    friend Symbolic operator-(const Complex& num1, const Symbolic& num2);
    friend Symbolic operator-(const Symbolic& num1, const Rational& num2);
    friend Symbolic operator-(const Rational& num1, const Symbolic& num2);

    friend Symbolic operator*(const Symbolic& num1, const Symbolic& num2);
    friend Symbolic operator*(const Symbolic& num1, const int num2);
    friend Symbolic operator*(const int num1, const Symbolic& num2);
    friend Symbolic operator*(const Symbolic& num1, const Real& num2);
    friend Symbolic operator*(const Real& num1, const Symbolic& num2);
    friend Symbolic operator*(const Symbolic& num1, const Complex& num2);
    friend Symbolic operator*(const Complex& num1, const Symbolic& num2);
    friend Symbolic operator*(const Symbolic& num1, const Rational& num2);
    friend Symbolic operator*(const Rational& num1, const Symbolic& num2);

    friend Symbolic operator/(const Symbolic& num1, const Symbolic& num2);
    friend Symbolic operator/(const Symbolic& num1, const int num2);
    friend Symbolic operator/(const int num1, const Symbolic& num2);
    friend Symbolic operator/(const Symbolic& num1, const Real& num2);
    friend Symbolic operator/(const Real& num1, const Symbolic& num2);
    friend Symbolic operator/(const Symbolic& num1, const Complex& num2);
    friend Symbolic operator/(const Complex& num1, const Symbolic& num2);
    friend Symbolic operator/(const Symbolic& num1, const Rational& num2);
    friend Symbolic operator/(const Rational& num1, const Symbolic& num2);

    friend Symbolic operator^(const Symbolic& num1, const Symbolic& num2);
    friend Symbolic operator^(const Symbolic& num1, const int num2);
    friend Symbolic operator^(const int num1, const Symbolic& num2);
    friend Symbolic operator^(const Symbolic& num1, const Real& num2);
    friend Symbolic operator^(const Real& num1, const Symbolic& num2);
    friend Symbolic operator^(const Symbolic& num1, const Complex& num2);
    friend Symbolic operator^(const Complex& num1, const Symbolic& num2);
    friend Symbolic operator^(const Symbolic& num1, const Rational& num2);
    friend Symbolic operator^(const Rational& num1, const Symbolic& num2);

    void operator+=(const Symbolic& num);
    void operator+=(const int num);

    void operator-=(const Symbolic& num);
    void operator-=(const int num);

    void operator*=(const Symbolic& num);
    void operator*=(const int num);

    void operator/=(const Symbolic& num);
    void operator/=(const int num);

    void operator^=(const Symbolic& num);
    void operator^=(const int num);

public:
    friend bool operator==(const Symbolic& num1, const Symbolic& num2);
    friend bool operator==(const Symbolic& num1, const int num2);
    friend bool operator==(const int num1, const Symbolic& num2);

    friend bool operator!=(const Symbolic& num1, const Symbolic& num2);
    friend bool operator!=(const Symbolic& num1, const int num2);
    friend bool operator!=(const int num1, const Symbolic& num2);

    friend bool operator>(const Symbolic& num1, const Symbolic& num2);
    friend bool operator>(const Symbolic& num1, const int num2);
    friend bool operator>(const int num1, const Symbolic& num2);

    friend bool operator>=(const Symbolic& num1, const Symbolic& num2);
    friend bool operator>=(const Symbolic& num1, const int num2);
    friend bool operator>=(const int num1, const Symbolic& num2);

    friend bool operator<(const Symbolic& num1, const Symbolic& num2);
    friend bool operator<(const Symbolic& num1, const int num2);
    friend bool operator<(const int num1, const Symbolic& num2);

    friend bool operator<=(const Symbolic& num1, const Symbolic& num2);
    friend bool operator<=(const Symbolic& num1, const int num2);
    friend bool operator<=(const int num1, const Symbolic& num2);

public:
    static Symbolic evalf(const Symbolic& num);
    static Symbolic evalf(const Symbolic& num, const Symbolic& prec);
    static Symbolic expand(const Symbolic& num);
    static Symbolic simplify(const Symbolic& num);
    static Symbolic factor(const Symbolic& num);
    static Symbolic diff(const Symbolic& num, const Symbolic& var);
    static Symbolic integrate(const Symbolic& num, const Symbolic& var);
    static Symbolic subs(const Symbolic& num, const Symbolic& var, const Symbolic& value);

    static Symbolic sin(const Symbolic& num);
    static Symbolic cos(const Symbolic& num);

    static Symbolic pow(const Symbolic& num1, const Symbolic& num2);
    static Symbolic log(const Symbolic& num1, const Symbolic& num2);
    static Symbolic root(const Symbolic& num1, const Symbolic& num2);

    static Real to_real(const Symbolic& num);
    static Complex to_complex(const Symbolic& num);
    static Rational to_rational(const Symbolic& num);

public:
    bool IsZero() const;
    bool IsNumber() const;

    int GetPrecision() const;
    void SetPrecision(int precision);

    std::u32string ToString() const;
    std::string ToStdString() const;

    SymEngine::Expression& GetExpr() const;

    static std::string ToUtf8(const std::u32string& s);
    static std::u32string ToUtf32(const std::string& s);

    template<typename T>
    SymEngine::Expression ToExpression(const T& num) const;

private:
    int precision;
    std::unique_ptr<SymEngine::Expression> expr;
};

}

#endif
