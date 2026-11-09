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

TEST_F(CalcTestArrayReal, arrays1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[1.E+0,2.E+0,3.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[123];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[123.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, arrays2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1+2,2.3,345E+1];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[3.E+0,2.3E+0,3.45E+3]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, arrays3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=x+y;");
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"[1,f(2,3),2.3,6/2];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[1.E+0,5.E+0,2.3E+0,3.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, arrays4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1234567890];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[1.235E+9]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, percent1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[5]%50;");
    ASSERT_TRUE(r.ToStdString(3, 15) == "[2.5E+0]") << r.ToStdString(3, 15);
}

TEST_F(CalcTestArrayReal, implicit_mul_div2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];");
    parser.Parse(LogicalId{0, 0, 2}, U"x=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=2;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2t{0};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"(x)/(y)t{0};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(1/2)t{0};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"2(t{0}+1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2abs(t{0}-5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"abs(t{0}-5)t{1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[9.E+0]") << r.ToStdString(3, 3);
}

//A nested array literal is a matrix, not a flat array - the array parser rejects it,
//the auto solver falls through to the symbolic parser which builds the matrix
TEST_F(CalcTestArrayReal, nested_array_rejected)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"[[1,2],[3,4]];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 2}, U"[[1,2],3];"), yutovo_calculator::MathException);
    auto r = parser.Parse(LogicalId{0, 0, 3}, U"[1,2,3];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0,2.E+0,3.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"(1)/(0);"), yutovo_calculator::MathException);
}


}
