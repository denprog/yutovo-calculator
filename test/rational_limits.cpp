/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, limit1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x*x-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit2)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x+2), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit3)
{
    //an infinite limit is not representable in the rational type
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, limit4)
{
    //the rational type has no infinity identifier
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=∞]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestRational, limit5)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit6)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x, [x=0], 2);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestRational, limit7)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x+2), [x=1/2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/5;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit8)
{
    //an unsigned infinity (different one-sided limits) is not representable in the rational type
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, limit9)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x*x-1)/(x-1), x, 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic1)
{
    //the difference quotient of x^3 at 2
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic2)
{
    //the difference quotient of x^4 at 2
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-16)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"32;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic3)
{
    //the first remarkable limit, computed exactly by giac
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic4)
{
    //the equivalence 1-cos(x) ~ x^2/2, exact 1/2
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic5)
{
    //the equivalence e^x-1 ~ x
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic6)
{
    //the equivalence ln(1+x) ~ x
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(ln(1+x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic7)
{
    //the first remarkable limit, scaled
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(3*x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic8)
{
    //the difference quotient of 1-x^2: (1-x^2)/(1-x) = 1+x -> 2
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-pow(x,2))/(1-x), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic9)
{
    //the derivative of x^5 at 2 as a limit: 5*2^4 = 80
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,5)-32)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"80;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_classic10)
{
    //factorization of a rational function: (x^2+3x+2)/(x+1) = x+2 -> 1 at -1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x*x+3*x+2)/(x+1), [x=-1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_variable_point1)
{
    //the limit point is a previously defined variable; the expression itself stays free of user
    //identifiers — the rational parser has no numerical fallback for expressions giac cannot evaluate
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)-4)/(x-2), [x=a]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"4;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate1)
{
    //0/0: (x^3-1)/(x-1) = x^2+x+1 -> 3 at 1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2 at 1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate5)
{
    //1^infinity with a rational limit: (1+h^2)^(1/h) = exp(h) -> 1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+pow(x-1,2), 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate6)
{
    //0^0: x^x -> 1 at 0 from above
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_neg_infinity1)
{
    //a negative infinite limit is not representable in the rational type
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], -1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, limit_neg_infinity2)
{
    //-1/x^2 runs to -infinity from both sides
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-1/pow(x,2), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, limit_nan1)
{
    //an oscillating function has no limit, and nan is not representable in the rational type
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(1/x), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, limit_negative1)
{
    //a negative integer result: (1-x^3)/(x-1) = -(x^2+x+1) -> -3 at 1
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-pow(x,3))/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-3;")) << res.ToStdString();
}

TEST_F(CalcTestRational, limit_negative2)
{
    //a negative fraction result: -1/(x+2) -> -1/2 at 0
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-1/(x+2), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1/2;")) << res.ToStdString();
}

}
