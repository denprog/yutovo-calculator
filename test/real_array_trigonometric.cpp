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

TEST_F(CalcTestArrayReal, trigonometric1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[0.E+0]") << res.ToStdString(3, 3);
    
    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(0);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "[0.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"[sin(0),arcsin(0)];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.E+0,0.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, trigonometric2)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[1.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(1);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Degree && res.ToStdString(3, 3) == "[90.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, trigonometric3)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"sin(deg(90));", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[1.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"deg(arcsin(1));", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Degree && res.ToStdString(3, 3) == "[90.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, trigonometric4)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[1.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin((pi)/(2)rad);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[1.E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"[sin(90°),sin((pi)/(2)rad)];", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[1.E+0,1.E+0]") << res.ToStdString(3, 3);
}

//Arc minutes and seconds: 1' = 1/60 degree, 1'' = 1/3600 degree; 1°30' = 1.5 degrees
TEST_F(CalcTestArrayReal, trigonometric5)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"[1',1''];", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Degree && res.ToStdString(3, 3) == "[0.0167E+0,2.778E-4]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"[1°30',1°30'30''];", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Degree && res.ToStdString(3, 3) == "[1.5E+0,1.508E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"[45'30'',22'];", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Degree && res.ToStdString(3, 3) == "[0.758E+0,0.367E+0]") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin([1°30']);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "[0.0262E+0]") << res.ToStdString(3, 3);
}

}
