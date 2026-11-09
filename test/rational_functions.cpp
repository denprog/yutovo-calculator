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

TEST_F(CalcTestRational, functions1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(x)=2*x/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 0, 0, 2, 0}, U"4/3;"));
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x/4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"f(2);") == parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"4/3;")) << 
        parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"f(2);").ToStdString();
}

TEST_F(CalcTestRational, functions2)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString() == "24") << r.ToStdString();
}

TEST_F(CalcTestRational, functions3)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"pow(2,1/3);", 3), yutovo_calculator::MathException) << 
        parser.Parse(LogicalId{0, 0, 1}, U"pow(2,1/3);").ToStdString();

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(2,6/3);", 3);
    ASSERT_TRUE(r.ToStdString() == "4") << r.ToStdString();
}

TEST_F(CalcTestRational, functions4)
{
    parser.Parse(LogicalId{0, 0, 0, 2}, U"f(x)=(x)/(3);");
    auto r = parser.Parse(LogicalId{0, 0, 0, 3}, U"f(4);", 3);
    ASSERT_TRUE(r.ToStdString() == "4/3") << r.ToStdString();
}

TEST_F(CalcTestRational, functions5)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-(2)/(3));", 3);
    ASSERT_TRUE(r.ToStdString() == "2/3") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 1}, U"abs((2)/(3));", 3);
    ASSERT_TRUE(r.ToStdString() == "2/3") << r.ToStdString();
}

TEST_F(CalcTestRational, functions6)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow((5)/(2),-3);");
    ASSERT_TRUE(res.ToString() == U"8/125") << res.ToStdString();
}

TEST_F(CalcTestRational, functions7)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow((5)/(2),0);");
    ASSERT_TRUE(res.ToString() == U"1") << res.ToStdString();
}

TEST_F(CalcTestRational, functions8)
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

TEST_F(CalcTestRational, functions9)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(0, 2);");
    ASSERT_TRUE(res.ToString() == U"0") << res.ToStdString();
}

TEST_F(CalcTestRational, functions10)
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

TEST_F(CalcTestRational, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString();
}

TEST_F(CalcTestRational, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString();
}

TEST_F(CalcTestRational, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestRational, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Rational res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString() == "5") << res.ToStdString();

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString() == "9") << res.ToStdString();
}

TEST_F(CalcTestRational, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Rational res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "6") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
}

TEST_F(CalcTestRational, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Rational res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "6") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
}

TEST_F(CalcTestRational, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Rational res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString() == "25") << res.ToStdString();
}

TEST_F(CalcTestRational, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Rational res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
}

TEST_F(CalcTestRational, min_max1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"min(1/2,1/3);", 3);
    ASSERT_TRUE(res.ToStdString() == "1/3") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(1/2,1/3);", 3);
    ASSERT_TRUE(res.ToStdString() == "1/2") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 1}, U"min(-3/2,-1/2);", 3);
    ASSERT_TRUE(res.ToStdString() == "-3/2") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(-3/2,-1/2);", 3);
    ASSERT_TRUE(res.ToStdString() == "-1/2") << res.ToStdString();
}

TEST_F(CalcTestRational, errors3)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"func(3)+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0 && ex.size == 4) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


TEST_F(CalcTestRational, errors4)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(23423423,234234234234234);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
