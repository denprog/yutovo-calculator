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

TEST_F(CalcTestSymbolicRational, numbers1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1/2;");
    ASSERT_TRUE(res.ToStdString(0) == "1/2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890;");
    ASSERT_TRUE(res.ToStdString(0) == "12345678901234567890") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890/9876543210987654321;");
    ASSERT_TRUE(res.ToStdString(0) == "137174210/109739369") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3;");
    ASSERT_TRUE(res.ToStdString(0) == "23/10") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;");
    ASSERT_TRUE(res.ToStdString(0) == "-23/10") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.12;");
    ASSERT_TRUE(res.ToStdString(0) == "3/25") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".12;");
    ASSERT_TRUE(res.ToStdString(0) == "3/25") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.0;");
    ASSERT_TRUE(res.ToStdString(0) == "12") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"54.990;");
    ASSERT_TRUE(res.ToStdString(0) == "5499/100") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3*x;");
    ASSERT_TRUE(res.ToStdString(0) == "(23/10)*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers6)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicRational, infinity_syntax_error)
{
    for (size_t n = 2; n <= 5; ++n)
    {
        std::u32string expr(n, U'∞');
        expr += U';';
        ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, expr), SyntaxException) << "n=" << n;
    }
}

TEST_F(CalcTestSymbolicRational, infinity_in_identifier)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞x;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ab∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞ab;"), SyntaxException);
}

TEST_F(CalcTestSymbolicRational, infinity_in_number)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.3∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞1;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞12.3;"), SyntaxException);
}

TEST_F(CalcTestSymbolicRational, inf1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inf;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf6)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, ln_zero)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(0);");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf7)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf8)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf9)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf10)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((pow(x,2)-1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, inf11)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-∞;");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan+1;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x*nan/3;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(0)/(0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(∞-∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan6)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(0*∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan7)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((∞)/(∞),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan8)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(∞,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, nan9)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(0,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=x;");
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2a;");
    ASSERT_TRUE(r.ToStdString(10) == "2*x") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"(1/2)a;");
    ASSERT_TRUE(r.ToStdString(10) == "(1/2)*x") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"2(a+1);");
    ASSERT_TRUE(r.ToStdString(10) == "2*(1+x)") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"2sin(x);");
    ASSERT_TRUE(r.ToStdString(10) == "2*sin(x)") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"sin(x)a;");
    ASSERT_TRUE(r.ToStdString(10) == "sin(x)*x") << r.ToStdString(10);
}

}
