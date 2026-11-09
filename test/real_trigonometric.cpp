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

TEST_F(CalcTestReal, trigonometric1)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
    
    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric2)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(1);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric3)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(deg(90));", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"deg(arcsin(1));", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric4)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin((pi)/(2)rad);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, trigonometric5)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"90°;", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "90.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"90deg;", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "90.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1rad;", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1grad;", AngleMeasure::Radian, AngleMeasure::Grad);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Grad && res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, trigonometric6)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"90°;", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"2rad;", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "114.592E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"2grad;", AngleMeasure::Radian, AngleMeasure::Grad);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Grad && res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, trigonometric7)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"90°;", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 10) == "1.5707963268E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"2rad;", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "114.5915590262E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"2grad;", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 10) == "0.03141592654E+0") << res.ToStdString(3, 10);
}

TEST_F(CalcTestReal, trigonometric8)
{
    //180deg - rad(pi) should be 0 after converting to a common angle measure.
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"180deg - rad(pi);", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) <<
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;

    //180deg / rad(pi) should be 1 after converting to a common angle measure.
    res = parser.Parse(LogicalId{0, 0, 1}, U"180deg / rad(pi);", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) <<
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

//Angle measures and arithmetic operations
TEST_F(CalcTestReal, trigonometric9)
{
    Real deg180 = parser.Parse(LogicalId{0, 0, 1}, U"180deg;", AngleMeasure::Radian, AngleMeasure::Degree);
    Real rad_pi = parser.Parse(LogicalId{0, 0, 1}, U"rad(pi);", AngleMeasure::Radian, AngleMeasure::Radian);
    Real one(deg180.GetBitPrecision(), 1);
    Real half(deg180.GetBitPrecision(), 0.5f);

    Real res = deg180 - 1;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (rad_pi - one).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = 1 - deg180;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (one - rad_pi).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = deg180 + 1;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (rad_pi + one).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = 1 + deg180;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (one + rad_pi).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = deg180 - 0.5f;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (rad_pi - half).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = 0.5f - deg180;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (half - rad_pi).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = deg180 + 0.5f;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (rad_pi + half).ToStdString(3, 3)) << res.ToStdString(3, 3);

    res = 0.5f + deg180;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == (half + rad_pi).ToStdString(3, 3)) << res.ToStdString(3, 3);

    Real plain = parser.Parse(LogicalId{0, 0, 1}, U"5;", AngleMeasure::Radian, AngleMeasure::Radian);
    res = plain - 2;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"3;", AngleMeasure::Radian, AngleMeasure::Radian).ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = deg180 * 2;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"360deg;", AngleMeasure::Radian, AngleMeasure::Degree).ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = deg180 / 2;
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree) << (int)res.angle_measure;
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90deg;", AngleMeasure::Radian, AngleMeasure::Degree).ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

//Grad conversions must stay exact at high precision: 1 grad = 9/10 degree = pi/200 radian
TEST_F(CalcTestReal, trigonometric10)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"10grad;", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "9.E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1grad;", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "0.9E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"90deg;", AngleMeasure::Radian, AngleMeasure::Grad, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Grad && res.ToStdString(3, 10) == "100.E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1rad;", AngleMeasure::Radian, AngleMeasure::Grad, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Grad && res.ToStdString(3, 10) == "63.6619772368E+0") << res.ToStdString(3, 10);
}

//Arc minutes and seconds
TEST_F(CalcTestReal, trigonometric11)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"1';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "0.01666666667E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1'';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "2.7777777778E-4") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"90';", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 10) == "0.02617993878E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1';", AngleMeasure::Radian, AngleMeasure::Grad, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Grad && res.ToStdString(3, 10) == "0.01851851852E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(30');", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 10) == "0.008726535498E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1°+30';", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "1.5E+0") << res.ToStdString(3, 3);

    //arc minutes must stay exact at high precision: 1' = 1/60 degree
    res = parser.Parse(LogicalId{0, 0, 1}, U"1';", AngleMeasure::Radian, AngleMeasure::Degree, 30);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 30) == "0.0166666666666666666666666666667E+0") << res.ToStdString(3, 30);

    //DMS components are number literals, so 1°20' is computed exactly like 1+20/60
    res = parser.Parse(LogicalId{0, 0, 1}, U"1°20';", AngleMeasure::Radian, AngleMeasure::Degree, 30);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree &&
        res.ToStdString(3, 30) == parser.Parse(LogicalId{0, 0, 1}, U"1+20/60;", AngleMeasure::Radian, AngleMeasure::Degree, 30).ToStdString(3, 30)) <<
        res.ToStdString(3, 30);
}

//Degree-minute-second notation
TEST_F(CalcTestReal, trigonometric12)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"1°30';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "1.5E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1°30'30'';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "1.5083333333E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1°0'30'';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "1.0083333333E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1°30';", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 10) == "0.02617993878E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(1°30');", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 10) == "0.02617694831E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"-1°30';", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "-1.5E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"2*1°30';", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);

    //minutes with seconds and no degrees: 22'3'' = 22/60 + 3/3600 = 0.3675 degrees
    res = parser.Parse(LogicalId{0, 0, 1}, U"22'3'';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "0.3675E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"22'3'';", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 10) == "0.006414085001E+0") << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"-22'3'';", AngleMeasure::Radian, AngleMeasure::Degree, 10);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 10) == "-0.3675E+0") << res.ToStdString(3, 10);

    //plain minutes keep working when not followed by seconds
    res = parser.Parse(LogicalId{0, 0, 1}, U"22';", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == "0.367E+0") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"22'3;"), yutovo_calculator::SyntaxException);

    //temperature units must still parse next to the degree sign
    res = parser.Parse(LogicalId{0, 0, 1}, U"1°C;", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0(K)") << res.ToStdString(3, 3);

    //seconds without minutes are ambiguous and rejected (1°0'30'' must be used instead)
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"1°30'';"), yutovo_calculator::SyntaxException);
}

//sin/cos special cases must be exact in rad, degrees and grads
TEST_F(CalcTestReal, trigonometric13)
{
    Real zero = parser.Parse(LogicalId{0, 0, 1}, U"0;", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    Real one = parser.Parse(LogicalId{0, 0, 1}, U"1;", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    Real minus_one = parser.Parse(LogicalId{0, 0, 1}, U"-1;", AngleMeasure::Radian, AngleMeasure::Radian, 30);

    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(180deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(180°);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(-180deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(360deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(0deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(90deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(180deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == minus_one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(270deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(360deg);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    //default degree measure must snap exact zeros too
    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(180);", AngleMeasure::Degree, AngleMeasure::Degree, 30);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(200grad);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(100grad);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(100grad);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(200grad);", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == minus_one.ToStdString(3, 30)) << res.ToStdString(3, 30);

    //DMS values that reduce to exact multiples of 90 degrees
    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(180°0');", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(179°60');", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    //self-consistency with the radian path at the same precision
    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(rad(pi));", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_TRUE(res.ToStdString(3, 30) == zero.ToStdString(3, 30)) << res.ToStdString(3, 30);

    //near-special values must not be snapped
    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(180.001deg);", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_FALSE(res == Real(3, 0)) << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(89.9deg);", AngleMeasure::Radian, AngleMeasure::Radian, 10);
    ASSERT_FALSE(res == Real(3, 0) || res == Real(3, 1) || res == Real(3, -1)) << res.ToStdString(3, 10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(deg(pow(10,20)));", AngleMeasure::Radian, AngleMeasure::Radian, 30);
    ASSERT_FALSE(res == Real(3, 0)) << res.ToStdString(3, 10);
}

TEST_F(CalcTestReal, trigonometric_special)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(pi);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(pi);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"-1;").ToStdString(3, 3)) << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 1}, U"sin((pi)/(2));");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}

}
