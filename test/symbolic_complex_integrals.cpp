/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestSymbolicComplex, indefinite_integral1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(pow(x,2), x);");
    ASSERT_TRUE(res.ToStdString(10) == "0.333*pow(x,3.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, indefinite_integral2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x*y, y);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5*x*pow(y,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, indefinite_integral_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, 1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, x+1);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicComplex, definite_integral1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(2,34,3y,x);");
    ASSERT_TRUE(res.ToStdString(10) == "96.*y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,(1)/(x+i),x);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "0.5*((-i)*π+2.*log(1.+i))") << res.ToStdString(10);

    parser.SetLocale(Language::Russian);
    auto res_russian = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,(1)/(x+j),x);", 10);
    ASSERT_TRUE(res_russian.ToStdString(10) == "0.5*((-i)*π+2.*log(1.+i))") << res_russian.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_j_russian)
{
    parser.SetLocale(Language::Russian);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,j*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::Russian) == "2.*j") << res.ToStdString(10, Language::Russian);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_english)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::English) == "2.*i") << res.ToStdString(10, Language::English);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_spanish)
{
    parser.SetLocale(Language::Spanish);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::Spanish) == "2.*i") << res.ToStdString(10, Language::Spanish);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_brazilian_portuguese)
{
    parser.SetLocale(Language::BrazilianPortuguese);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::BrazilianPortuguese) == "2.*i") << res.ToStdString(10, Language::BrazilianPortuguese);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_german)
{
    parser.SetLocale(Language::German);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::German) == "2.*i") << res.ToStdString(10, Language::German);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_french)
{
    parser.SetLocale(Language::French);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::French) == "2.*i") << res.ToStdString(10, Language::French);
}

TEST_F(CalcTestSymbolicComplex, definite_integral_i_italian)
{
    parser.SetLocale(Language::Italian);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,\u03C0,pow(e,i*z),z);", 10);
    ASSERT_TRUE(res.ToStdString(10, Language::Italian) == "2.*i") << res.ToStdString(10, Language::Italian);
}

}
