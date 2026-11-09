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

TEST_F(CalcTestReal, functions1)
{
    Real r = parser.Parse(LogicalId{0, 0, 1}, U"sin(1);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.841E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"ln(4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.386E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"lg(4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.602E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"log(2, 4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"2.0E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log(2, 3);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log:2,3;");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log:2,1+3;");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"3.E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log:2,(1+3);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"2.E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions2)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"rad(pi/2);", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"deg(180);", AngleMeasure::Degree, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"pi;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"grad(1);", AngleMeasure::Grad, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.9;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions3)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5.!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"120.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(5+2)!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.04E+3") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions4)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(4,2);");
    ASSERT_TRUE(res.ToString(3, 3) == U"2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions5)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"exp(1);", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, functions6)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2exp(1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.437E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions7)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "24.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions8)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions9)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.3E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"abs(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.3E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions10)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"integer(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(-2.7);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions11)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"fract(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-0.3E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"fract(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.3E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions12)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(-2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions13)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"floor(-2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions14)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions15)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(2.1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions16)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,0);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, functions17)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(0, 2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions18)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,-2);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, functions19)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(5.2940000000000005e-23);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "-51.293E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(0.00069399999999999996);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "-7.273E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestReal, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "9.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Real res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Real res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "25.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"y=(sin((pi)/(2)))/(3.4);");
    parser.Parse(LogicalId{0, 0, 2}, U"r=sqrt(y);");
    Real res = parser.Parse(LogicalId{0, 0, 3}, U"r;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.542E+0") << res.ToStdString(3, 3);
    ASSERT_TRUE(res.GetBitPrecision() < 1000);
    parser.Parse(LogicalId{0, 0, 4}, U"f(t)=t*cos(r*2);");
    res = parser.Parse(LogicalId{0, 0, 5}, U"f(3.4);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.589E+0") << res.ToStdString(3, 3);
    ASSERT_TRUE(res.GetBitPrecision() < 2000) << res.GetBitPrecision();
}

TEST_F(CalcTestReal, user_functions10)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=1/x;");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 2}, U"f(0);"), yutovo_calculator::MathException);
    Real res = parser.Parse(LogicalId{0, 0, 3}, U"f(2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.5E+0") << res.ToStdString(3, 3);
}

//Remove function after error of its declaring
TEST_F(CalcTestReal, user_functions11)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"f(x)=sin();"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"f(x)=(3)/();"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"f(x)=;"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }
}

TEST_F(CalcTestReal, sin_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sin(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 0)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-0.645E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, cos_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"cos(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 1)) << r.ToStdString(3, 10);
    ASSERT_FALSE(r == Real(3, -1)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.764E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, tg_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"tg(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 0)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-0.845E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, ctg_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"ctg(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 0)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-1.184E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, sec_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sec(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 1)) << r.ToStdString(3, 10);
    ASSERT_FALSE(r == Real(3, -1)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.309E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, cosec_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"cosec(pow(10,20));");
    ASSERT_FALSE(r == Real(3, 0)) << r.ToStdString(3, 10);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-1.55E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, ln_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"ln(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "46.052E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, lg_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"lg(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "20.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, log_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"log(2,pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "66.439E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, exp_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"exp(pow(10,20));");
    ASSERT_TRUE(r.IsInfinity()) << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arctg_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arctg(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.571E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arcctg_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arcctg(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arcsec_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arcsec(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.571E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arccosec_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arccosec(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E-20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, th_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"th(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, cth_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"cth(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, sch_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sch(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, csch_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"csch(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arsh_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arsh(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "46.745E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arch_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arch(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "46.745E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arcth_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arcth(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E-20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arsch_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"arsch(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, arcsch_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arcsch(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E-20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, sqrt_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sqrt(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+10") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, root_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"root(pow(10,20),3);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "4.642E+6") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, pow_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(10,20);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, abs_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, floor_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"floor(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+20") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, fract_large_arg)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"fract(pow(10,20));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, fact1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "120.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.629E+6") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, fact2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
#ifdef _WIN32
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5000000!;", &parser_context), yutovo_calculator::TimeExceedException);
#else
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1000000!;", &parser_context), yutovo_calculator::TimeExceedException);
#endif
}

TEST_F(CalcTestReal, min_max1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"min(2.5,3.7);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.5E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(2.5,3.7);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.7E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"min(-1.2,-1.5);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-1.5E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(-1.2,-1.5);", 3);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-1.2E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, errors5)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"sqrt();");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::WrongArgumentsCount && ex.pos == 0 && ex.size == 6) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
