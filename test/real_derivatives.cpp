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

TEST_F(CalcTestReal, derivative_at_point1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,3), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point3)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(sin(x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point4)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point5)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=2*x;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"2;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point6)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [x=3])+1;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"7;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,2), [1=3]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, derivative_at_point7)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x*y;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,5), [x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"5;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point8)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=x+y;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(4,y), [y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"1;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point_multi1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point_multi2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [y=3, x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point_multi3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"12;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, derivative_at_point_step_precision)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=sin(x);");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=1]);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.5403023059E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, derivative_at_point_large_point)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,2);");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=10000000000]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"20000000000;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(pow(x,2), [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"9;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=2*x;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x), [x=5]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"10;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate3)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(pow(x,2), [x=3])+1;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate_multi1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x+y, [x=1, y=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate_multi2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"12;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, evaluate_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(pow(x,2), [1=3]);"), yutovo_calculator::SyntaxException);
}

}
