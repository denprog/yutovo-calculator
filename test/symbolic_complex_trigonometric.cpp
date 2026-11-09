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

TEST_F(CalcTestSymbolicComplex, sin1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, cos1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, sin_pi_12)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(pi/12));");
    ASSERT_TRUE(res.ToStdString(10) == "0.259") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json == R"xxx({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"0.25"},{"type":13,"symbol":"·"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":13,"symbol":"·"},{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]},{"type":20,"symbol":")"}]}]}]})xxx") << json;
}

TEST_F(CalcTestSymbolicComplex, trig_power1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(sin(x),2)+pow(cos(x),2));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(cos(x),2.)+pow(sin(x),2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_sinh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_cosh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_tanh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"tanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_coth0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(coth(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_sech)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_csch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_asinh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_acosh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_atanh)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_acoth1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_asech)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_acsch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_symbolic)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sinh(x)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "acosh(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_alias)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.881") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_inverse_synonyms)
{
    Symbolic<Complex> a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asinh(0);");
    Symbolic<Complex> b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    Symbolic<Complex> c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsinh(0);");
    ASSERT_TRUE(a.ToStdString(10) == "0") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"acosh(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccosh(1);");
    ASSERT_TRUE(a.ToStdString(10) == "0") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"atanh(0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arctanh(0);");
    ASSERT_TRUE(a.ToStdString(10) == "0") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(acoth(x),x,1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccoth(x),x,1);");
    ASSERT_TRUE(a.ToStdString(10) == "∞") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "∞") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "∞") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asech(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsech(1);");
    ASSERT_TRUE(a.ToStdString(10) == "0") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(acsch(x),x,0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccsch(x),x,0);");
    Symbolic<Complex> d = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcosech(x),x,0);");
    Symbolic<Complex> e = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccosech(x),x,0);");
    ASSERT_TRUE(a.ToStdString(10) == "∞") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "∞") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "∞") << c.ToStdString(10);
    ASSERT_TRUE(d.ToStdString(10) == "∞") << d.ToStdString(10);
    ASSERT_TRUE(e.ToStdString(10) == "∞") << e.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, hyperbolic_acosh_nan)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, sinh_tojson)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"sinh"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, cos_expression_brackets)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x-1);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(1.-x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, sin2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"0"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicComplex, sin3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicComplex, cos2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicComplex, cos3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

}
