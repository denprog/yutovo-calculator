/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include <clocale>
#include <condition_variable>
#include <cstdlib>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestSymbolicReal, limit_cot0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cot(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sec_pi2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sec(x),x,(pi)/(2));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_csc0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csc(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sin_x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"nan\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, limit_exp_m1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((exp(x)-1)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_ln_x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, limit_exp_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sin_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_cos_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cos(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_ln_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sqrt_noo)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/pow(x,2), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == u8"∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == u8"∞") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(1/x, [x=0], -1);");
    ASSERT_TRUE(res.ToStdString(10) == u8"-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(1/x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit5)
{
    //a parameter stays symbolic in the limit result
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(a*x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "a") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit6)
{
    //the built-in function form limit(expression, variable, point, direction)
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2)/x, x, 0, 0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(1/x, x, 0, 1);");
    ASSERT_TRUE(res.ToStdString(10) == u8"∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit8)
{
    //a bounded non-convergent function has no limit at infinity
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit9)
{
    //iterated limits over several variables
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "6.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit10)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x, [x=0], 2);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, limit11)
{
    //a decimal zero point must be converted to an exact value, otherwise giac substitutes it
    //instead of computing the limit (0/0) and the result would be nan
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0.0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit12)
{
    //a decimal point away from the singularity: sin(0.5)/0.5 = 0.958851077208406
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0.5]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.959") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit13)
{
    //the two-sided limit of 1/x at 0 does not exist: unsigned infinity prints as nan, not as ∞
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit14)
{
    //the positional form limit(f, x, a) and the equation form limit(f, x=a) as keywords
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, x, 0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit(1/x, x=∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic1)
{
    //the first remarkable limit, scaled
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(5*x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "5.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic2)
{
    //the equivalence 1-cos(x) ~ x^2/2 (exact constants are printed in the decimal Real style)
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic3)
{
    //the equivalence e^x-1 ~ x
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic4)
{
    //the equivalence ln(1+x) ~ x
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(ln(1+x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic5)
{
    //the first remarkable limit for the tangent
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(tg(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic6)
{
    //the derivative of x^3 at 2 as a limit
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "12.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic7)
{
    //the ratio of the leading coefficients of polynomials at infinity
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((2*x*x+3*x)/(x*x+5), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic8)
{
    //x^(1/x) -> 1 at infinity
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic9)
{
    //the squeeze theorem: |x^2*sin(1/x)| <= x^2 -> 0
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*x*sin(1/x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic10)
{
    //the second remarkable limit evaluates the constant: (1+1/x)^x -> e = 2.718281828459045
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+1/x, x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.718") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic11)
{
    //the conjugate multiplication: sqrt(x^2+x)-x -> 1/2 at infinity
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sqrt(x*x+x)-x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_classic12)
{
    //the derivative of x^2 at the symbolic point a: (x^2-a^2)/(x-a) -> 2*a
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)-pow(a,2))/(x-a), [x=a]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*a") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_neg_infinity1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,3), [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == u8"-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_neg_infinity2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_neg_infinity3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(exp(x), [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_variable_point1)
{
    //the limit point is a previously defined variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)-pow(a,2))/(x-a), [x=a]);");
    ASSERT_TRUE(res.ToStdString(10) == "4.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_outer1)
{
    //the expression uses a variable and a user function that do not involve the limit variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(t)=2*t;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"limit(b*f(1)*sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "6.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_outer2)
{
    //a symbolic coefficient that does not involve the limit variable stays in the result
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(a*sin(2*x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*a") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate1)
{
    //0/0: (x^4-1)/(x-1) -> 4
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "4.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate5)
{
    //1^infinity: x^(1/(x-1)) -> e = 2.718281828459045
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.718") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate6)
{
    //0^0: x^x -> 1
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_dne1)
{
    //an oscillating function has no limit at the accumulation point of the oscillations
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(1/x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_negative1)
{
    //a negative finite result
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "-1.") << res.ToStdString(10);
}

}
