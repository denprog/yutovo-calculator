/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, functions1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 2, 0}, U"f(2);") == parser.Parse(LogicalId{0, 0, 0, 0, 2, 0}, U"7;"));
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a=5+6;");
    parser.Parse(LogicalId{0, 0, 0, 2, 3, 0}, U"f(t)=a*t;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"f(2);") == parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"22;")) << 
        parser.Parse(LogicalId{0, 0, 0, 2, 3, 1}, U"f(2);").ToStdString();
}

TEST_F(CalcTestInteger, functions2)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString() == "24") << r.ToStdString();
}

TEST_F(CalcTestInteger, functions3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a=5;");
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(2,3)a;", 3);
    ASSERT_TRUE(r.ToStdString() == "40") << r.ToStdString();
}

TEST_F(CalcTestInteger, functions4)
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

TEST_F(CalcTestInteger, functions5)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(0, 2);");
    ASSERT_TRUE(res.ToStdString() == "0") << res.ToStdString();
}

TEST_F(CalcTestInteger, functions6)
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

TEST_F(CalcTestInteger, functions7)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"reminder(3, 2);");
    ASSERT_TRUE(res.ToStdString() == "1") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"reminder(35, 3);");
    ASSERT_TRUE(res.ToStdString() == "2") << res.ToStdString();
}

TEST_F(CalcTestInteger, functions8)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(1, -5);");
    ASSERT_TRUE(res.ToStdString() == "1") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(-1, -4);");
    ASSERT_TRUE(res.ToStdString() == "1") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(-1, -3);");
    ASSERT_TRUE(res.ToStdString() == "-1") << res.ToStdString();

    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(2, -1);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestInteger, builtin_functions1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToString(10) == U"1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToString(10) == U"120") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(5+2)!;");
    ASSERT_TRUE(res.ToString(10) == U"5040") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, builtin_functions2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a!;");
    ASSERT_TRUE(res.ToString(10) == U"2") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, builtin_functions3)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-2);", 3);
    ASSERT_TRUE(r.ToStdString(10) == "2") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 1}, U"abs(2);", 3);
    ASSERT_TRUE(r.ToStdString(10) == "2") << r.ToStdString(10);
}

TEST_F(CalcTestInteger, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString();
}

TEST_F(CalcTestInteger, user_functions2)
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

TEST_F(CalcTestInteger, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestInteger, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Integer res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString() == "5") << res.ToStdString();

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString() == "9") << res.ToStdString();
}

TEST_F(CalcTestInteger, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Integer res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "6") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
}

TEST_F(CalcTestInteger, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Integer res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
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

TEST_F(CalcTestInteger, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Integer res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString() == "25") << res.ToStdString();
}

TEST_F(CalcTestInteger, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Integer res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
}

TEST_F(CalcTestInteger, fact1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0!;");
    ASSERT_TRUE(res.ToString(10) == U"1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10!;");
    ASSERT_TRUE(res.ToString(10) == U"3628800") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, fact2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1000000!;", &parser_context),
        yutovo_calculator::TimeExceedException);
}

TEST_F(CalcTestInteger, min_max1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(2,3);");
    ASSERT_TRUE(res.ToString(10) == U"2") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(2,3);");
    ASSERT_TRUE(res.ToString(10) == U"3") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(-5,-1);");
    ASSERT_TRUE(res.ToString(10) == U"-5") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(-5,-1);");
    ASSERT_TRUE(res.ToString(10) == U"-1") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(7,7);");
    ASSERT_TRUE(res.ToString(10) == U"7") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, errors4)
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
