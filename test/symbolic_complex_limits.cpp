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

TEST_F(CalcTestSymbolicComplex, limit_cot0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cot(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_sec_pi2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sec(x),x,(pi)/(2));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_csc0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csc(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_sin_x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":47,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"nan\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, limit_exp_m1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((exp(x)-1)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_ln_x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":47,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, limit_exp_1x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_sin_1x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_cos_1x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cos(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_ln_1x)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_sqrt_noo)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == u8"∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2)/x, x, 0, 0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, x, 0);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic1)
{
    //the first remarkable limit, scaled
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(3*x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "3") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic2)
{
    //the derivative of x^3 at 2 as a limit
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "12") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic3)
{
    //the equivalence 1-cos(x) ~ x^2/2
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic4)
{
    //the equivalence e^x-1 ~ x
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic5)
{
    //factorization over the complex numbers: (x^4-1)/(x-i) = (x-1)(x+1)(x+i) -> -4i at i
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-i), [x=i]);");
    ASSERT_TRUE(res.ToStdString(10) == "-4.*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic7)
{
    //1^infinity: x^(1/(x-1)) -> e = 2.718281828459045
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.718") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_classic8)
{
    //the second remarkable limit evaluates the constant: (1+1/x)^x -> e = 2.718281828459045
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+1/x, x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.718") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_neg_infinity1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,3), [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == u8"-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_neg_infinity2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_variable_point1)
{
    //the limit point is a previously defined variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=i;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)+1)/(x-i), [x=a]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate1)
{
    //0/0: (x^4-1)/(x-1) -> 4
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,4)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "4") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate5)
{
    //1^infinity: x^(1/(x-1)) -> e = 2.718281828459045
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "2.718") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate6)
{
    //0^0: x^x -> 1
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_negative1)
{
    //a negative finite result
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_both_parts1)
{
    //a result with both real and imaginary parts: x^2+i -> 4+i at 2
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2)+i, [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "4.+i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_nan1)
{
    //the two-sided limit of 1/x at 0 does not exist: unsigned infinity prints as nan
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, limit_parameter1)
{
    //a symbolic parameter that does not involve the limit variable stays in the result
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(a*x/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "a") << res.ToStdString(10);
}

}
