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

TEST_F(CalcTestSymbolicRational, sin1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sin(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, cos1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(0) == "cos(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, sin_pi_12)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(pi/12));");
    ASSERT_TRUE(res.ToStdString(0) == "(1/4)*sqrt(2)*(-1+sqrt(3))") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json == R"xxx({"type":46,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]},{"type":13,"symbol":"·"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":13,"symbol":"·"},{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]},{"type":20,"symbol":")"}]}]}]})xxx") << json;
}

TEST_F(CalcTestSymbolicRational, trig_power1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(sin(x),2)+pow(cos(x),2));");
    ASSERT_TRUE(res.ToStdString(0) == "pow(cos(x),2)+pow(sin(x),2)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_sinh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(0);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_cosh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(0);");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_tanh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"tanh(0);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_coth0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(coth(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_sech)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sech(0);");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_csch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_asinh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_acosh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_atanh)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_acoth1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "acoth(1)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_asech)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_acsch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "acsch(0)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_symbolic)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sinh(x)") << res.ToStdString(0);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(x);");
    ASSERT_TRUE(res.ToStdString(0) == "acosh(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_alias)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosech(1);");
    ASSERT_TRUE(res.ToStdString(0) == "log(1+sqrt(2))") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_inverse_synonyms)
{
    Symbolic<Rational> a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asinh(0);");
    Symbolic<Rational> b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    Symbolic<Rational> c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsinh(0);");
    ASSERT_TRUE(a.ToStdString(0) == "0") << a.ToStdString(0);
    ASSERT_TRUE(b.ToStdString(0) == "0") << b.ToStdString(0);
    ASSERT_TRUE(c.ToStdString(0) == "0") << c.ToStdString(0);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"acosh(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccosh(1);");
    ASSERT_TRUE(a.ToStdString(0) == "0") << a.ToStdString(0);
    ASSERT_TRUE(b.ToStdString(0) == "0") << b.ToStdString(0);
    ASSERT_TRUE(c.ToStdString(0) == "0") << c.ToStdString(0);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"atanh(0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arctanh(0);");
    ASSERT_TRUE(a.ToStdString(0) == "0") << a.ToStdString(0);
    ASSERT_TRUE(b.ToStdString(0) == "0") << b.ToStdString(0);
    ASSERT_TRUE(c.ToStdString(0) == "0") << c.ToStdString(0);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"acoth(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcoth(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccoth(1);");
    ASSERT_TRUE(a.ToStdString(10) == "acoth(1)") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "acoth(1)") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "acoth(1)") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asech(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsech(1);");
    ASSERT_TRUE(a.ToStdString(0) == "0") << a.ToStdString(0);
    ASSERT_TRUE(b.ToStdString(0) == "0") << b.ToStdString(0);
    ASSERT_TRUE(c.ToStdString(0) == "0") << c.ToStdString(0);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"acsch(0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsch(0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccsch(0);");
    Symbolic<Rational> d = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosech(0);");
    Symbolic<Rational> e = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccosech(0);");
    ASSERT_TRUE(a.ToStdString(10) == "acsch(0)") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "acsch(0)") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "acsch(0)") << c.ToStdString(10);
    ASSERT_TRUE(d.ToStdString(10) == "acsch(0)") << d.ToStdString(10);
    ASSERT_TRUE(e.ToStdString(10) == "acsch(0)") << e.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, hyperbolic_acosh_nan)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, sin2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"0"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, sin3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sin(x)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, cos2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(0);");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, cos3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(0) == "cos(x)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

}
