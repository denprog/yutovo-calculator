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

TEST_F(CalcTestSymbolicRational, limit_cot0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cot(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_sec_pi2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sec(x),x,(pi)/(2));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_csc0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csc(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_sin_x)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":46,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"nan\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, limit_exp_m1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((exp(x)-1)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_ln_x)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":46,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, limit_exp_1x)
{
    bool thrown = false;
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(1/x),x,0);");
    }
    catch (...)
    {
        thrown = true;
    }
    ASSERT_TRUE(thrown);
}

TEST_F(CalcTestSymbolicRational, limit_sin_1x)
{
    bool thrown = false;
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(1/x),x,0);");
    }
    catch (...)
    {
        thrown = true;
    }
    ASSERT_TRUE(thrown);
}

TEST_F(CalcTestSymbolicRational, limit_cos_1x)
{
    bool thrown = false;
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cos(1/x),x,0);");
    }
    catch (...)
    {
        thrown = true;
    }
    ASSERT_TRUE(thrown);
}

TEST_F(CalcTestSymbolicRational, limit_ln_1x)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_sqrt_noo)
{
    bool thrown = false;
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(-∞);");
    }
    catch (...)
    {
        thrown = true;
    }
    ASSERT_TRUE(thrown);
}

TEST_F(CalcTestSymbolicRational, limit1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x*x-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x+2), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1/2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == u8"∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit5)
{
    //the built-in function form limit(expression, variable, point, direction)
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,2)/x, x, 0, 0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit6)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x+2), x, 1/2);");
    ASSERT_TRUE(res.ToStdString(10) == "2/5") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic1)
{
    //the first remarkable limit, exact
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic2)
{
    //the equivalence 1-cos(x) ~ x^2/2, exact fraction
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-cos(x))/(x*x), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1/2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic3)
{
    //the equivalence e^x-1 ~ x
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(e,x)-1)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic4)
{
    //the equivalence ln(1+x) ~ x
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(ln(1+x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic5)
{
    //the first remarkable limit, scaled
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(sin(3*x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "3") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic6)
{
    //the derivative of x^3 at 2 as a limit
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-8)/(x-2), [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "12") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic7)
{
    //the second remarkable limit keeps the symbolic constant: (1+1/x)^x -> e
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1+1/x, x), [x=∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "e") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic8)
{
    //the derivative of x^5 at 2 as a limit: 5*2^4 = 80
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,5)-32)/(x-2), [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "80") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic9)
{
    //the difference quotient of x^2 at 2
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,2)-4)/(x-2), [x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "4") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_classic10)
{
    //the difference quotient of 1-x^2: (1-x^2)/(1-x) = 1+x -> 2
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1-pow(x,2))/(1-x), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_neg_infinity1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,3), [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == u8"-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_neg_infinity2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((x+1)/x, [x=-∞]);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_variable_point1)
{
    //the limit point is a previously defined variable
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"limit((pow(x,2)-pow(a,2))/(x-a), [x=a]);");
    ASSERT_TRUE(res.ToStdString(10) == "4") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate1)
{
    //0/0: (x^3-1)/(x-1) -> 3
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((pow(x,3)-1)/(x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "3") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate2)
{
    //infinity/infinity: (1/h)/(1/h^2) = h -> 0
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit((1/(x-1))/(1/pow(x-1,2)), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate3)
{
    //0*(-infinity): x*ln(x) -> 0 at 0 from above
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(x*ln(x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate4)
{
    //infinity-infinity: 1/(x-1)-2/(x^2-1) = 1/(x+1) -> 1/2
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/(x-1)-2/(x*x-1), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "1/2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate5)
{
    //1^infinity: x^(1/(x-1)) -> e
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x, 1/(x-1)), [x=1]);");
    ASSERT_TRUE(res.ToStdString(10) == "e") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate6)
{
    //0^0: x^x -> 1
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(x,x), [x=0], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_indeterminate7)
{
    //infinity^0: (1/h)^h -> 1
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(pow(1/(x-1), x-1), [x=1], 1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_negative1)
{
    //a negative fraction result
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(-1/(x+2), [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "-1/2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_nan1)
{
    //the two-sided limit of 1/x at 0 does not exist: unsigned infinity prints as nan
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(1/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, limit_parameter1)
{
    //a symbolic parameter that does not involve the limit variable stays in the result
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"limit(a*sin(x)/x, [x=0]);");
    ASSERT_TRUE(res.ToStdString(10) == "a") << res.ToStdString(10);
}

}
