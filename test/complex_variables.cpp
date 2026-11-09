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

TEST_F(CalcTestComplex, underscore_variables)
{
    parser.Parse(LogicalId{0, 0, 1}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 2}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 3}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 4}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 5}, U"a_=5;");
    std::string sum = parser.Parse(LogicalId{0, 0, 6}, U"_+_a+_a_+__+a_;").ToStdString(3, 3);
    ASSERT_TRUE(sum == "15.E+0") << sum;
    std::string power = parser.Parse(LogicalId{0, 0, 7}, U"_a*_a;").ToStdString(3, 3);
    ASSERT_TRUE(power == "4.E+0") << power;
}

TEST_F(CalcTestComplex, variables1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"v=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"v;").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestComplex, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"v=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"(1/2)v;").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.5E+0") << s;
}

TEST_F(CalcTestComplex, variables3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"y=pow(e,2);");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"y;").ToStdString(3, 3);
    ASSERT_TRUE(s == "7.389E+0") << s;
}

TEST_F(CalcTestComplex, variables4)
{
    parser.Parse(LogicalId{0, 0, 2}, U"a=77;");
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 3}, U"a;").ToStdString(3, 3);
    ASSERT_TRUE(s == "77.E+0") << s;
}

TEST_F(CalcTestComplex, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0") << s;
    parser.Parse(LogicalId{0, 0, 3}, U"d=45+d;");
    s = parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
    ASSERT_TRUE(s == "54.E+0") << s;
}

TEST_F(CalcTestComplex, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    s = parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3);
    ASSERT_TRUE(s == "12.E+0") << s;
}

TEST_F(CalcTestComplex, variables7)
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
    Complex res = parser.Parse(LogicalId{0, 0, 2}, U"sin;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 3}, U"sin(2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.909E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, variables8)
{
    parser.Parse(LogicalId{0, 0, 5}, U"b=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 1}, U"b=2;");
    Complex res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
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
}

TEST_F(CalcTestComplex, variables9)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v{12}=555;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"v{12};", &dependencies).ToStdString(3, 3);
    ASSERT_TRUE(s == "555.E+0") << s;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"v{12}") != dependencies.end());
}

TEST_F(CalcTestComplex, variables10)
{
    yutovo_calculator::ParserContext parser_context;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    parser.Parse(LogicalId{0, 0, 2}, U"v;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == false);
}

//Remove variable after error of its declaring
TEST_F(CalcTestComplex, variables11)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    std::string s = parser.Parse(LogicalId{0, 0, 2}, U"b;").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;

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

TEST_F(CalcTestComplex, errors2)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"2/dj;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2 && ex.size == 2) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
