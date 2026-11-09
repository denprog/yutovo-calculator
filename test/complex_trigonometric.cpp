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

TEST_F(CalcTestComplex, trigonometric1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sin(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.175E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.841E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "9.154E+0+i*-4.169E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"cos(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.543E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.54E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cos(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-4.19E+0+i*-9.109E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric3)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"tg(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.762E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"tg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.557E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"tg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.00376E+0+i*1.003E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric4)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"ctg(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.313E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"ctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.642E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"ctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.00374E+0+i*-0.997E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric5)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.648E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.0417E+0+i*0.0906E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric6)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"cosec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cosec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.188E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cosec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.0905E+0+i*0.0412E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric7)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sh(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.841E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sh(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.175E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sh(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-3.591E+0+i*0.531E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric8)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"ch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.54E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"ch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.543E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"ch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-3.725E+0+i*0.512E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric9)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"th(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.557E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"th(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.762E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"th(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.965E+0+i*-0.00988E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric10)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"cth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.642E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cth(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.313E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"cth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.036E+0+i*0.0106E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric11)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.648E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.264E+0+i*-0.0362E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric12)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"csch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.188E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"csch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"csch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.273E+0+i*-0.0403E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric13)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "i*0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.571E+0+i*1.983E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric14)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arccos(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0+i*-0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arccos(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian);
    ASSERT_TRUE(abs(res.GetRe()) <= Real(3, 1e-15f)) << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arccos(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.E+0+i*-1.983E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric15)
{
    Complex res;
    EXPECT_THROW(res = parser.Parse(LogicalId{0, 0, 1}, U"arctg(i);"), yutovo_calculator::MathException);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.785E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.41E+0+i*0.229E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric16)
{
    Complex res;
    EXPECT_THROW(res = parser.Parse(LogicalId{0, 0, 1}, U"arcctg(i);"), yutovo_calculator::MathException);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.785E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.161E+0+i*-0.229E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric17)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0+i*0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian);
    ASSERT_TRUE(abs(res.GetRe()) <= Real(3, 1e-15f)) << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.42E+0+i*0.231E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric18)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arccosec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "i*-0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arccosec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arccosec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.15E+0+i*-0.231E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric19)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arsh(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arsh(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arsh(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.969E+0+i*0.965E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric20)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0+i*1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.983E+0+i*1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric21)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.785E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(res = parser.Parse(LogicalId{0, 0, 1}, U"arth(1);"), yutovo_calculator::MathException);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.147E+0+i*1.339E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric22)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.785E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(res = parser.Parse(LogicalId{0, 0, 1}, U"arcth(1);"), yutovo_calculator::MathException);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.147E+0+i*-0.232E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcth(0.5);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.549E+0+i*1.571E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric23)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arsch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0+i*-1.571E+0") << res.ToStdString(3, 3) << (int)res.GetAngleMeasure();

    res = parser.Parse(LogicalId{0, 0, 1}, U"arsch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arsch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.231E+0+i*-1.42E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric24)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.157E+0+i*-0.23E+0") << res.ToStdString(3, 3);
}

}
