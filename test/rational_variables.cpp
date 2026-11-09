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

TEST_F(CalcTestRational, underscore_variables)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"a_=5;");
    Rational sum = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"_+_a+_a_+__+a_;");
    ASSERT_TRUE(sum.ToString() == U"15") << sum.ToStdString();
    Rational power = parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"_a*_a;");
    ASSERT_TRUE(power.ToString() == U"4") << power.ToStdString();
}

TEST_F(CalcTestRational, variables1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"п=3/2;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"п;") == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"3/2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"п;").ToStdString();
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"пр=3/2;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1, 0, 1}, U"пр+5;") == parser.Parse(LogicalId{0, 0, 1, 0, 1}, U"13/2;")) << 
        parser.Parse(LogicalId{0, 0, 1, 0, 1}, U"пр+5;").ToStdString();
}

TEST_F(CalcTestRational, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;") == parser.Parse(LogicalId{0, 0, 2}, U"(15)/(4);")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;");
}

TEST_F(CalcTestRational, variables3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5d;") == parser.Parse(LogicalId{0, 0, 2}, U"20;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5d;");
    parser.Parse(LogicalId{0, 0, 3}, U"d2=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"5d+3d2;") == parser.Parse(LogicalId{0, 0, 4}, U"35;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"5d+3d2;");
}

TEST_F(CalcTestRational, variables4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=1/4;");
    parser.Parse(LogicalId{0, 0, 2}, U"d{2}=3/4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"d+d{2};") == parser.Parse(LogicalId{0, 0, 3}, U"1;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"d+d{2};");
    parser.Parse(LogicalId{0, 0, 4}, U"d{2}=5/2;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 5}, U"5d{2}+3d;") == parser.Parse(LogicalId{0, 0, 5}, U"13(1/4);")) << 
        parser.Parse(LogicalId{0, 0, 5}, U"5d{2}+3d;");
}

//Remove variable after error of its declaring
TEST_F(CalcTestRational, variables5)
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

TEST_F(CalcTestRational, symbols1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"•;").ToStdString();
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"‣;").ToStdString();
}

TEST_F(CalcTestRational, list1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"materials=\"cuprum\",\"ferrum\";");
    parser.Parse(LogicalId{0, 0, 2}, U"ro{cuprum}=1.68*pow(10,-8);");
    parser.Parse(LogicalId{0, 0, 3}, U"ro{ferrum}=9.71*pow(10,-8);");
    parser.Parse(LogicalId{0, 0, 4}, U"material{materials}=\"cuprum\";");
    auto r = parser.Parse(LogicalId{0, 0, 5}, U"ro{material};");
    ASSERT_TRUE(r.ToStdString() == "21/1250000000") << r.ToStdString();

    parser.Parse(LogicalId{0, 0, 4}, U"material{materials}=\"ferrum\";");
    r = parser.Parse(LogicalId{0, 0, 5}, U"ro{material};");
    ASSERT_TRUE(r.ToStdString() == "971/10000000000") << r.ToStdString();
}

TEST_F(CalcTestRational, list2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.include_document = true;
    parser.Parse(LogicalId{0, 0, 1}, U"materials=\"cuprum\",\"ferrum\";", &parser_context);
    parser.Parse(LogicalId{0, 0, 2}, U"ro{cuprum}=1.68*pow(10,-8)*(Ohm*m);", &parser_context);
    parser.Parse(LogicalId{0, 0, 3}, U"ro{ferrum}=9.71*pow(10,-8)*(Ohm*m);", &parser_context);
    parser.Parse(LogicalId{0, 0, 4}, U"length=1m;", &parser_context);
    parser.Parse(LogicalId{0, 0, 5}, U"section=1*pow(mm,2);", &parser_context);
    parser.Parse(LogicalId{0, 0, 6}, U"material{materials}=\"ferrum\";", &parser_context);
    parser.Parse(LogicalId{0, 0, 7}, U"resistance=ro{material}*(length)/(section);", &parser_context);
    auto r = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"resistance;", &parser_context));
    ASSERT_TRUE(r.ToStdString() == "97100(mcOhm)") << r.ToStdString();

    parser_context.include_document = false;
    parser.Parse(LogicalId{0, 1, 1}, U"length=4m;", &parser_context);
    parser.Parse(LogicalId{0, 1, 2}, U"section=2*pow(mm,2);", &parser_context);
    parser.Parse(LogicalId{0, 1, 3}, U"material{materials}=\"cuprum\";", &parser_context);
    r = parser.GetSuitableUnit(LogicalId{0, 1, 4}, parser.Parse(LogicalId{0, 1, 4}, U"resistance;", &parser_context));
    ASSERT_TRUE(r.ToStdString() == "168/5(mOhm)") << r.ToStdString();
}

}
