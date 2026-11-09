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

TEST_F(CalcTestInteger, underscore_variables)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"a_=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"_+_a+_a_+__+a_;") == parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"15;"));
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"_a*_a;") == parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"4;"));
}

TEST_F(CalcTestInteger, variables1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=5;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a=5+6;");
    parser.Parse(LogicalId{0, 0, 0, 1, 0}, U"b=a+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1, 1, 0}, U"b+3;") == parser.Parse(LogicalId{0, 0, 1, 1, 0}, U"21;"));
}

TEST_F(CalcTestInteger, variables2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"п=55;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"п;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"55;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"п;").ToStdString();
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"пр=55;");
    Integer r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"пр+5;");
    ASSERT_TRUE(r == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"60;")) << r.ToStdString();
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"fп=55;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"fп+5;");
    ASSERT_TRUE(r == parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"60;")) << r.ToStdString();
}

TEST_F(CalcTestInteger, variables3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1, 1, 0}, U"3a;") == parser.Parse(LogicalId{0, 0, 1, 1, 0}, U"15;")) << 
        parser.Parse(LogicalId{0, 0, 1, 1, 0}, U"3a;").ToStdString();
}

TEST_F(CalcTestInteger, variables4)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=5;");
    parser.Parse(LogicalId{0, 0, 0, 1, 1}, U"b=6;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2, 1, 0}, U"3a+4b+3;") == parser.Parse(LogicalId{0, 0, 2, 1, 0}, U"42;")) << 
        parser.Parse(LogicalId{0, 0, 2, 1, 0}, U"3a+4b+3;").ToStdString();
}

TEST_F(CalcTestInteger, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=6;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString() == parser.Parse(LogicalId{0, 0, 2}, U"6;").ToStdString()) << 
        parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"a{2}=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a{2};").ToStdString() == parser.Parse(LogicalId{0, 0, 4}, U"13;").ToStdString()) << 
        parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a{2};").ToStdString();
}

//Remove variable after error of its declaring
TEST_F(CalcTestInteger, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"b;") == parser.Parse(LogicalId{0, 0, 2}, U"55;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"b;").ToStdString();

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

TEST_F(CalcTestInteger, symbols1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"•"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"•").ToStdString();
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"‣"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"‣").ToStdString();
}

TEST_F(CalcTestInteger, str1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"33ttt;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, errors3)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"d23+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0 && ex.size == 3) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
