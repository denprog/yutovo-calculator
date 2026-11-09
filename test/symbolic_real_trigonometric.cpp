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

TEST_F(CalcTestSymbolicReal, sin1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToString(10) == U"sin(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, cos1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToString(10) == U"cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, sin_pi_12)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(pi/12));");
    ASSERT_TRUE(res.ToStdString(10) == "0.259") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json == R"xxx({"type":45,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"0.25"},{"type":13,"symbol":"·"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":13,"symbol":"·"},{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]},{"type":20,"symbol":")"}]}]}]})xxx") << json;
}

TEST_F(CalcTestSymbolicReal, hyperbolic_sinh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_cosh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_tanh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"tanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_coth0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(coth(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_sech)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_csch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asinh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acosh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_atanh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acoth1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asech)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acsch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_symbolic)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sinh(x)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "acosh(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_alias)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.881") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_inverse_synonyms)
{
    Symbolic<Real> a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asinh(0);");
    Symbolic<Real> b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    Symbolic<Real> c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsinh(0);");
    ASSERT_TRUE(a.ToStdString(10) == "0.") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0.") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0.") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"acosh(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arccosh(1);");
    ASSERT_TRUE(a.ToStdString(10) == "0.") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0.") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0.") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"atanh(0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arctanh(0);");
    ASSERT_TRUE(a.ToStdString(10) == "0.") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0.") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0.") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(acoth(x),x,1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccoth(x),x,1);");
    ASSERT_TRUE(a.ToStdString(10) == "∞") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "∞") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "∞") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"asech(1);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcsech(1);");
    ASSERT_TRUE(a.ToStdString(10) == "0.") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "0.") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "0.") << c.ToStdString(10);

    a = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(acsch(x),x,0);");
    b = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    c = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccsch(x),x,0);");
    Symbolic<Real> d = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcosech(x),x,0);");
    Symbolic<Real> e = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arccosech(x),x,0);");
    ASSERT_TRUE(a.ToStdString(10) == "∞") << a.ToStdString(10);
    ASSERT_TRUE(b.ToStdString(10) == "∞") << b.ToStdString(10);
    ASSERT_TRUE(c.ToStdString(10) == "∞") << c.ToStdString(10);
    ASSERT_TRUE(d.ToStdString(10) == "∞") << d.ToStdString(10);
    ASSERT_TRUE(e.ToStdString(10) == "∞") << e.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acosh_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asech_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(2);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "nan" || s == "-nan.") << s;
}

TEST_F(CalcTestSymbolicReal, hyperbolic_artanh_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(2);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "nan" || s == "-nan.") << s;
}

TEST_F(CalcTestSymbolicReal, hyperbolic_arcoth_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcoth(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, sinh_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sinh"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, cosh_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"cosh"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sin_expression_brackets)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x+1);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(1.+x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sin2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, sin3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, cos2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, cos3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

}
