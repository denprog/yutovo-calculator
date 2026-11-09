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
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, derivative_at_point1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*x, [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point2)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(pow(x,3), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*x;");
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"4;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point_multi1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point_multi2)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(x*y, [y=3, x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point_multi3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x,y)=pow(x,2)*y;");
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x,y), [x=2, y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"12;")) << res.ToStdString();
}

TEST_F(CalcTestRational, derivative_at_point_truncation)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=pow(x,3);");
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"derivative(f(x), [x=1]);");
    Rational error = abs(res - Rational(3));
    ASSERT_TRUE(error < Rational(0, 1e-15)) << res.ToStdString();
}

TEST_F(CalcTestRational, evaluate1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"9;")) << res.ToStdString();
}

TEST_F(CalcTestRational, evaluate2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*x;");
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"evaluate(f(x), [x=2]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"4;")) << res.ToStdString();
}

TEST_F(CalcTestRational, evaluate3)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [x=3])+1;");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10;")) << res.ToStdString();
}

TEST_F(CalcTestRational, evaluate_multi1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*y, [x=2, y=3]);");
    ASSERT_TRUE(res == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"6;")) << res.ToStdString();
}

TEST_F(CalcTestRational, evaluate_error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evaluate(x*x, [1=3]);"), yutovo_calculator::SyntaxException);
}

}
