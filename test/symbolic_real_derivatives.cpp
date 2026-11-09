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

TEST_F(CalcTestSymbolicReal, derivative1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x, 2), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_diff_synonym)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(pow(x, 2), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"derivative(f(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"derivative(f(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(sin(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_complex1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,3)+2*pow(x,2)-5*x+1, x);");
    ASSERT_TRUE(res.ToString(10) == U"-5.+4.*x+3.*pow(x,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"5\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":8,\"elements\":\"4\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":8,\"elements\":\"x\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":8,\"elements\":\"3\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2\"}]}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, derivative_power5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,5),x);");
    ASSERT_TRUE(res.ToStdString(10) == "5.*pow(x,4.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, derivative_cos)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(cos(x),x);");
    ASSERT_TRUE(res.ToStdString(10) == "-sin(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, derivative_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, x+1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, 5);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, sin(x));"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"6.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(sin(x), [x=0]);");
    ASSERT_TRUE(res.ToString(10) == U"1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"derivative(f(x), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"6.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [x=3])+1;");
    ASSERT_TRUE(res.ToString(10) == U"7.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, [x+1=1]);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_error2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x, [5=1]);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point6)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x*y;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point7)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x*y;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point8)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,y);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"log(x)*pow(x,3.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(1/x, [x=0]);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"∞"}]}]})r")
        << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_multi1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"3.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_multi2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [y=3, x=2]);");
    ASSERT_TRUE(res.ToString(10) == U"2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_multi3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"12.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_at_point_multi4)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y+sin(y);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=1, y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"6.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, derivative_mixed_second_order)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"func(x,y)=pow(x,2)*pow(y,2);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(derivative(func(x,y),y),x);");
    ASSERT_TRUE(res.ToStdString(10) == "4.*x*y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(pow(x,2), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"9.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x), [x=3]);");
    ASSERT_TRUE(res.ToString(10) == U"9.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(pow(x,2), [x=3])+1;");
    ASSERT_TRUE(res.ToString(10) == U"10.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate_multi1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x+y, [x=1, y=2]);");
    ASSERT_TRUE(res.ToString(10) == U"3.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate_multi2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,y);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x,y), [y=3]);");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,3.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, evaluate_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x, [x+1=1]);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, evaluate_error2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x, [5=1]);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, recursion_depth_symbolic1)
{
    std::u32string deep(U"derivative(");
    deep.append(20000, U'(');
    deep += U"x";
    deep.append(20000, U')');
    deep += U", [x=1]);";
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, deep), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestSymbolicReal, recursion_depth_symbolic2)
{
    std::u32string deep(U"derivative('");
    deep.append(20000, U'(');
    deep += U"x";
    deep.append(20000, U')');
    deep += U", [x=1]);";
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, deep), yutovo_calculator::SyntaxException);
}


}
