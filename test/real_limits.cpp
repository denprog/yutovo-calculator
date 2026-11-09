/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, limit1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit3)
{
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/pow(x,2), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit4)
{
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit5)
{
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], -1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit6)
{
    //a numeric result cannot be undefined - the symbolic parser returns the non-existent limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 0);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit7)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit8)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(e,x), [x=-∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit9)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit10)
{
    //the numerical fallback evaluates user functions that giac does not know
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=sin(x)/x;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit11)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x/(x+1);");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit12)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit13)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2));"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, limit14)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x, [x=0], 2);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestReal, limit15)
{
    //a bounded non-convergent function has no limit at infinity - a numeric result cannot be undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x), [x=∞]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit16)
{
    //a decimal limit point is converted to an exact value; sin(0.5)/0.5 = 0.958851077208406
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0.5]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.959E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit17)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)-0.25)/(x-0.5), [x=0.5]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit18)
{
    //the positional form limit(f, x, a) and the equation form limit(f, x=a)
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, x, 0);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(1/x, x, 0, -1);"), yutovo_calculator::MathException);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"limit(1/x, x=∞);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit19)
{
    //an angle measure on the limit point must not be dropped: sin(90deg) is 1, not sin(90 radians)
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x), [x=90deg]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit20)
{
    //one-sided limits of a user function run through the numerical fallback - an infinite value overflows it
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=1/x;");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=0], 1);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"limit(f(x), [x=0], -1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit21)
{
    //a user function with different one-sided limits has no two-sided limit
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=1/x;");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit22)
{
    //a user function running away to infinity on both sides - an infinite value overflows the numeric result
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=1/pow(x,2);");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit23)
{
    //an iterated limit: the inner limit over y gives 1 for every x, then the outer limit over x keeps 1
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x*y)/(x*y), [x=0, y=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit24)
{
    //lim (1+1/x)^x = e = 2.718281828459045
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+1/x, x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.718E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit25)
{
    //identifiers that merely start with the limit keywords are not affected
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limited=5;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limited;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit26)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*limit(sin(x)/x, [x=0])+1;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit27)
{
    //the direction is an arbitrary expression evaluating to 1 - an infinite value overflows the numeric result
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 2-1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit_classic1)
{
    //the first remarkable limit, scaled: sin(5x)/x -> 5
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(5*x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic2)
{
    //the equivalence 1-cos(x) ~ x^2/2
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic3)
{
    //the equivalence e^x-1 ~ x
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic4)
{
    //the equivalence ln(1+x) ~ x
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(ln(1+x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic5)
{
    //the first remarkable limit for the tangent (computed through the numerical fallback: giac does not know tg)
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(tg(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic6)
{
    //the derivative as a limit: (x^3-8)/(x-2) is the difference quotient of x^3 at 2
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic7)
{
    //the ratio of the leading coefficients of polynomials at infinity
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((2*x*x+3*x)/(x*x+5), [x=∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic8)
{
    //x^(1/x) -> 1 at infinity
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/x), [x=∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic9)
{
    //the squeeze theorem: |x^2*sin(1/x)| <= x^2 -> 0
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*x*sin(1/x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic10)
{
    //one-sided limits of |x|/x (the jump of the sign function)
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(abs(x)/x, [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(abs(x)/x, [x=0], -1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"-1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic11)
{
    //the second remarkable limit at zero: (1+x)^(1/x) -> e = 2.718281828459045
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+x, 1/x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.718E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_classic12)
{
    //the conjugate multiplication: sqrt(x^2+x)-x -> 1/2 at infinity
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sqrt(x*x+x)-x, [x=∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_neg_infinity1)
{
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,3), [x=-∞]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, limit_neg_infinity2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=-∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_neg_infinity3)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/pow(x,2), [x=-∞]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_variable_point1)
{
    //the limit point is a previously defined variable; the difference quotient of x^2 at a
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)-pow(a,2))/(x-a), [x=a]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"4;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_outer1)
{
    //the expression uses a variable and a user function that do not involve the limit variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(t)=2*t;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"limit(b*f(1)*sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_outer2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(t)=t*t+1;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(2)*sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate1)
{
    //0/0: (x^4-1)/(x-1) = x^3+x^2+x+1 -> 4 at 1
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2 at 1
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate5)
{
    //1^infinity: x^(1/(x-1)) = (1+h)^(1/h) -> e = 2.718281828459045
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.718E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate6)
{
    //0^0: x^x -> 1 at 0 from above
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, limit_dne1)
{
    //an oscillating function has no limit at the accumulation point of the oscillations -
    //a numeric result cannot be undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(1/x), [x=0]);"), yutovo_calculator::MathException);
}

}
