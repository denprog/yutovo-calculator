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

TEST_F(CalcTestArrayReal, functions1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[exp(1)];", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "[2.71828E+0]") << r.ToStdString(3, 5);
}

TEST_F(CalcTestArrayReal, functions2)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[24.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions3)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow([2,3,4],[3,2,1]);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[8.E+0,9.E+0,4.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions4)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[abs(-2.3),abs(2.3)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.3E+0,2.3E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions5)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[integer(-2.3),integer(-2.7),integer(2.3),integer(2.6)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-2.E+0,-2.E+0,2.E+0,2.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions6)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"fract(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-0.3E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"fract(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[0.3E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions7)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[ceil(-2.6),ceil(-2.3),ceil(2.3),ceil(2.5)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-2.E+0,-2.E+0,3.E+0,3.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions8)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[floor(-2.6),floor(2.5),floor(-2.3),floor(2.3)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-3.E+0,2.E+0,-3.E+0,2.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions9)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-2.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-3.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"[round(2.3),round(2.5)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.E+0,3.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, functions10)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-2.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-2.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"[trunc(2.5)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"[trunc(2.1)];", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, min_max1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"min([3,1,4,1,5,9,2,6]);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[1.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max([3,1,4,1,5,9,2,6]);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[9.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"min([-2.5,-3.5,-1.5]);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[-3.5E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max([-2.5,-3.5,-1.5]);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[-1.5E+0]") << res.ToStdString(3, 3);
}

}
