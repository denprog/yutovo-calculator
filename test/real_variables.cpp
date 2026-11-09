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

TEST_F(CalcTestReal, str1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"в;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, str2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"33t;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, underscore_variables)
{
    parser.Parse(LogicalId{0, 0, 1}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 2}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 3}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 4}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 5}, U"a_=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 6}, U"_+_a+_a_+__+a_;") == parser.Parse(LogicalId{0, 0, 6}, U"15;")) <<
        parser.Parse(LogicalId{0, 0, 6}, U"_+_a+_a_+__+a_;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 7}, U"_a*_a;") == parser.Parse(LogicalId{0, 0, 7}, U"4;")) <<
        parser.Parse(LogicalId{0, 0, 7}, U"_a*_a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables1)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v;") == parser.Parse(LogicalId{0, 0, 2}, U"555;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"v;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;") == parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;")) << 
        parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v=55.5;v;") == parser.Parse(LogicalId{0, 0, 2}, U"55.5;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"v=55.5;v;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"vp=55.5;", &dependencies);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;", &dependencies) == parser.Parse(LogicalId{0, 0, 3, 0}, U"60.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;").ToStdString(3, 3);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestReal, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"п=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"п;") == parser.Parse(LogicalId{0, 0, 1}, U"555;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"п;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"п=55.5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 3}, U"п;") == parser.Parse(LogicalId{0, 0, 3, 3}, U"55.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 3}, U"п;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3, 3, 0}, U"пр=55.5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 3, 1}, U"пр+5;") == parser.Parse(LogicalId{0, 0, 1}, U"60.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 3, 1}, U"пр+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables4)
{
    parser.Parse(LogicalId{0, 0, 2}, U"a=77;");
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"a;") == parser.Parse(LogicalId{0, 0, 3}, U"77;")) << parser.Parse(LogicalId{0, 0, 3}, U"a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d+5;") == parser.Parse(LogicalId{0, 0, 2}, U"9;")) << parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"d=45+d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;") == parser.Parse(LogicalId{0, 0, 4}, U"54;")) << parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1d;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d*5;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20.4;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"d=4.1*d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 4}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a2_=3.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2*a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2*a2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables10)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a22=3.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables11)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"3.75;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables12)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 4}, U"12;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables13)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pi;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.142E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"pi;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
    r = parser.Parse(LogicalId{0, 0, 1}, U"π;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables14)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"e;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.718E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"e;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables15)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "6.674E-11((m^3)/(kg*s^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables16)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5.5;");
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);

    parser.SetLocale(Language::Russian);
    r = parser.Parse(LogicalId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables17)
{
    parser.SetLocale(Language::Russian);
    parser.Parse(LogicalId{0, 0, 1}, U"α=1;");
    parser.Parse(LogicalId{0, 0, 2}, U"a=1см;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=2см;");
    parser.Parse(LogicalId{0, 0, 4}, U"S=a*b*sin(α);");
    auto r = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"S;"));
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"1.683E+0(см^2)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables18)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"6.674E-11((м^3)/(кг*с^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables19)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"g_a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"9.807E+0((м)/(с^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables20)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"A=5;");
    r = parser.Parse(LogicalId{0, 0, 2}, U"B=45;");
    r = parser.Parse(LogicalId{0, 0, 3}, U"A%B;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.25E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables21)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"sin;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    parser.Parse(LogicalId{0, 0, 1}, U"sin=2;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"sin;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 3}, U"sin(2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.909E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables22)
{
    parser.Parse(LogicalId{0, 0, 5}, U"b=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 1}, U"b=2;");
    Real res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 1}, U"b=5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 5}, U"b=6;");
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables23)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v{12}=555;");
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"v{12};", &dependencies).ToStdString(3, 3);
    ASSERT_TRUE(res == "555.E+0") << res;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"v{12}") != dependencies.end());
}

TEST_F(CalcTestReal, variables24)
{
    yutovo_calculator::ParserContext parser_context;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    parser.Parse(LogicalId{0, 0, 2}, U"v;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestReal, variables25)
{
    parser.SetLocale(Language::Russian);
    yutovo_calculator::ParserContext parser_context;
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k;", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"k=1мk;"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k;", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestReal, variables26)
{
    parser.SetLocale(Language::Russian);
    yutovo_calculator::ParserContext parser_context;
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"k{e}=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k{e};", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);

    parser.RemoveIdentifier(LogicalId{0, 0, 1}, U"k{e}");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 2}, U"k{e};"), yutovo_calculator::SyntaxException);
}

//Remove variable after error of its declaring
TEST_F(CalcTestReal, variables27)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "55.E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=sin();"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"b;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=(3)/();"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"b;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=;"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"b;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }
}

TEST_F(CalcTestReal, variables28)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=(45)/();"), yutovo_calculator::SyntaxException);
    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"b;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }
}

//A function replaces a variable with the same name
TEST_F(CalcTestReal, variables29)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=123;");
    parser.Parse(LogicalId{0, 0, 2}, U"b=234+a;");
    Real res = parser.Parse(LogicalId{0, 0, 3}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "357.E+0") << res.ToStdString(3, 3);

    try
    {
        parser.Parse(LogicalId{0, 0, 4}, U"b=b(k)=k!;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::SyntaxError) << ex.ex_id;
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 5}, U"b;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
    }
}

TEST_F(CalcTestReal, symbols1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 1}, U"•;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 1}, U"‣;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, list_identifiers1)
{
    std::vector<std::pair<std::u32string, std::u32string>> variables;
    parser.ListUserVariables(variables);
    ASSERT_TRUE(std::find_if(variables.begin(), variables.end(), 
        [](auto& p)
        {
            return p.first == U"c" && p.second == U"speed of light";
        }) != variables.end());
    
    variables.clear();
    parser.SetLocale(Language::Russian);
    parser.ListUserVariables(variables);
    ASSERT_TRUE(std::find_if(variables.begin(), variables.end(), 
        [](auto& p)
        {
            return p.first == U"ск_св" && p.second == U"скорость света";
        }) != variables.end());
}

TEST_F(CalcTestReal, list_identifiers2)
{
    std::vector<CustomUnit<Real>> units;
    parser.ListUserUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.name == U"km" && p.description == U"kilometre";
        }) != units.end());
    
    units.clear();
    parser.SetLocale(Language::Russian);
    parser.ListUserUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.name == U"км" && p.description == U"километр";
        }) != units.end());
}

TEST_F(CalcTestReal, list_identifiers3)
{
    std::vector<std::pair<std::u32string, std::u32string>> units;
    parser.ListBuiltinUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.first == U"m" && p.second == U"metre";
        }) != units.end());
    
    units.clear();
    parser.SetLocale(Language::Russian);
    parser.ListBuiltinUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.first == U"м" && p.second == U"метр";
        }) != units.end());
}

TEST_F(CalcTestReal, list1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"materials=\"cuprum\",\"ferrum\";", 3);
    parser.Parse(LogicalId{0, 0, 2}, U"ro{cuprum}=1.68E-8;", 3);
    parser.Parse(LogicalId{0, 0, 3}, U"ro{ferrum}=9.71E-8;", 3);
    parser.Parse(LogicalId{0, 0, 4}, U"material{materials}=\"cuprum\";", 3);
    auto r = parser.Parse(LogicalId{0, 0, 5}, U"ro{material};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.68E-8") << r.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 4}, U"material{materials}=\"ferrum\";", 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"ro{material};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "9.71E-8") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, list2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.include_document = true;
    parser.Parse(LogicalId{0, 0, 1}, U"materials=\"cuprum\",\"ferrum\";", &parser_context);
    parser.Parse(LogicalId{0, 0, 2}, U"ro{cuprum}=1.68E-8*(Ohm*m);", &parser_context);
    parser.Parse(LogicalId{0, 0, 3}, U"ro{ferrum}=9.71E-8*(Ohm*m);", &parser_context);
    parser.Parse(LogicalId{0, 0, 4}, U"length=1m;", &parser_context);
    parser.Parse(LogicalId{0, 0, 5}, U"section=1*pow(mm,2);", &parser_context);
    parser.Parse(LogicalId{0, 0, 6}, U"material{materials}=\"ferrum\";", &parser_context);
    parser.Parse(LogicalId{0, 0, 7}, U"resistance=ro{material}*(length)/(section);", &parser_context);
    auto r = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"resistance;", &parser_context));
    ASSERT_TRUE(r.ToStdString(3, 3) == "97.1E+0(mOhm)") << r.ToStdString(3, 3);

    parser_context.include_document = false;
    parser.Parse(LogicalId{0, 1, 1}, U"length=4m;", &parser_context);
    parser.Parse(LogicalId{0, 1, 2}, U"section=2*pow(mm,2);", &parser_context);
    parser.Parse(LogicalId{0, 1, 3}, U"material{materials}=\"cuprum\";", &parser_context);
    r = parser.GetSuitableUnit(LogicalId{0, 1, 4}, parser.Parse(LogicalId{0, 1, 4}, U"resistance;", &parser_context));
    ASSERT_TRUE(r.ToStdString(3, 3) == "33.6E+0(mOhm)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, errors2)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"d=4+f;");
        parser.Parse(LogicalId{0, 0, 2}, U"d+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}


TEST_F(CalcTestReal, errors3)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"5+d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}


TEST_F(CalcTestReal, errors4)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"d=5+d;");
        parser.Parse(LogicalId{0, 0, 2}, U"33455-d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}


TEST_F(CalcTestReal, errors7)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"2+d+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2 && ex.size == 1) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
