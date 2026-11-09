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

TEST_F(CalcTestSymbolicComplex, diff1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x+x, x);");
    ASSERT_TRUE(res.ToStdString(10) == "2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(sin(x), x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff_complex1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,3)+2*pow(x,2)-5*x+1, x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "-5.+4.*x+3.*pow(x,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},{"type":8,"elements":"5"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"4"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"x"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"3"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":15,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"x"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, diff_power5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,5),x);");
    ASSERT_TRUE(res.ToStdString(10) == "5.*pow(x,4.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, diff_cos)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(cos(x),x);");
    ASSERT_TRUE(res.ToStdString(10) == "-sin(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, diff_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, x+1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, 5);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, sin(x));"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicComplex, exp_derivative_e_base)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(e, 2*i*x), x);");
    std::string text = res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(text == "2.*i*exp(2.*i*x)") << text;
    ASSERT_TRUE(json ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":13,"symbol":"·"},{"type":8,"elements":"i"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"e"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"},{"type":13,"symbol":"·"},{"type":8,"elements":"i"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"}]}]}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [x=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "6") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "6") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x*y;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point4)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x*y;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(1/x, [x=0]);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"∞"}]}]})r")
        << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point_multi1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "3") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point_multi2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [y=3, x=2]);");
    ASSERT_TRUE(res.ToStdString(10) == "2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, derivative_at_point_multi3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "12") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, evaluate1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "9") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, evaluate2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x), [x=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "9") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, evaluate3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3])+1;");
    ASSERT_TRUE(res.ToStdString(10) == "10") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, evaluate_multi1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(10) == "6") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, evaluate_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x, [5=1]);"), yutovo_calculator::ParserException);
}

}
