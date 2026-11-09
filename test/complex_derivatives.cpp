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

TEST_F(CalcTestComplex, derivative_at_point1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*x, [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(sin(x), [x=0]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*x;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"4;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point_multi1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point_multi2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [y=3, x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point_multi3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"12;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, derivative_at_point_large_point)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,2);");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=10000000000]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"20000000000;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, evaluate1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"9;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, evaluate2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*x;");
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x), [x=2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"4;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, evaluate3)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3])+1;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10;")) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, evaluate_multi1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, evaluate_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [1=3]);"), yutovo_calculator::SyntaxException);
}

}
