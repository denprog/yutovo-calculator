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

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestComplex, limit1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)+1)/(x-i), [x=i]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2i;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit3)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)-4)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit4)
{
    //a numeric result cannot be infinite - the symbolic parser returns the infinite limit
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit5)
{
    //the numerical fallback evaluates user functions that giac does not know
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=sin(x)/x;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(f(x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit6)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x, [x=0], 2);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestComplex, limit7)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, x, 0);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit8)
{
    //a complex direction with a non-zero imaginary part is not a valid limit direction
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x, [x=0], 1+2i);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit_classic1)
{
    //the first remarkable limit, scaled
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(3*x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic2)
{
    //the difference quotient of x^3 at 2
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic3)
{
    //the equivalence 1-cos(x) ~ x^2/2
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic4)
{
    //the equivalence e^x-1 ~ x
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic5)
{
    //the equivalence ln(1+x) ~ x
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(ln(1+x)/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic6)
{
    //the derivative of sin at the complex point i: (sin(x)-sin(i))/(x-i) -> cos(i) = cosh(1) = 1.5430806348152437
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((sin(x)-sin(i))/(x-i), [x=i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.543E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic7)
{
    //the squeeze theorem: |x^2*sin(1/x)| <= x^2 -> 0
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*x*sin(1/x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic8)
{
    //the square root is continuous at the boundary of its domain: sqrt(x) -> 0 at 0 from above
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sqrt(x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic9)
{
    //factorization over the complex numbers: (x^4-1)/(x-i) = (x-1)(x+1)(x+i) -> (i-1)(i+1)*2i = -4i at i
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-i), [x=i]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-4i;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_classic10)
{
    //the theorem (1-cos(x))/x -> 0
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/x, [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_variable_point1)
{
    //the limit point is a previously defined variable; the expression itself stays free of user
    //identifiers, otherwise giac would treat the denominator as a non-zero symbolic constant
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=i;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)+1)/(x-i), [x=a]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2i;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_outer1)
{
    //the expression uses a variable and a user function that do not involve the limit variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"c=2;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(t)=t+1;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"limit(c*f(1)*sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate1)
{
    //0/0: (x^4-1)/(x-1) -> 4 at 1
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2 at 1
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.5;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate5)
{
    //1^infinity: x^(1/(x-1)) -> e = 2.718281828459045
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.718E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate6)
{
    //0^0: x^x -> 1 at 0 from above
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, limit_neg_infinity1)
{
    //1/x runs to -infinity from below - a numeric result cannot be infinite
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], -1);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit_neg_infinity2)
{
    //-1/x^2 runs to -infinity from both sides - a numeric result cannot be infinite
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-1/pow(x,2), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit_nan1)
{
    //the two-sided limit of 1/x at 0 does not exist - a numeric result cannot be undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit_nan2)
{
    //an oscillating function has no limit at the accumulation point of the oscillations -
    //a numeric result cannot be undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(1/x), [x=0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, limit_both_parts1)
{
    //a result with both real and imaginary parts: x^2+i -> 4+i at 2
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2)+i, [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4+i;")) << res.ToStdString(3, 3);
}

}
