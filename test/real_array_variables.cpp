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

TEST_F(CalcTestArrayReal, variables1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"t;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.E+0,3.E+0,4.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"t{0};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[2.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"t{1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[3.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"t{2};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{3};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::ArgumentIsOver && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{-1};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::ArgumentIsOver && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{n};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::ArgumentIsOver && ex.pos == 0) << LogicalIdToString(ex.id);
    }
}

TEST_F(CalcTestArrayReal, subscript_expression1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"t{1+1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"t{size(t)-1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"2*t{0+1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"2t{0+1};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"(2)/(2)t{2/2};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[3.E+0]") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"t{t{0}};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, subscript_expression2)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"var=[1,2,3];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"loop(i=0,(i<=(size(var)-2)),i=i+1,t=0,t=t+var{i}+var{i+1});", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[8.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, subscript_expression3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1;", 3);
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"t{f(1)};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[4.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, subscript_expression4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);
    parser.Parse(LogicalId{0, 0, 2}, U"n=1;", 3);
    auto r = parser.Parse(LogicalId{0, 0, 3}, U"t{n};", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[3.E+0]") << r.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 4}, U"m=1.5;", 3);
    try
    {
        parser.Parse(LogicalId{0, 0, 5}, U"t{m};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 5}) && ex.ex_id == ParserExceptionCode::IncorrectOperation && ex.pos == 0) << LogicalIdToString(ex.id);
    }
}

TEST_F(CalcTestArrayReal, subscript_expression_errors)
{
    parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{1.5};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::IncorrectOperation && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{size(t)};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::ArgumentIsOver && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{[1,2]};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::IncorrectOperation && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    try
    {
        parser.Parse(LogicalId{0, 0, 2}, U"t{1=3};", 3);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 2}) && ex.ex_id == ParserExceptionCode::SyntaxError) << LogicalIdToString(ex.id);
    }
}

TEST_F(CalcTestArrayReal, subscript_dependencies)
{
    parser.Parse(LogicalId{0, 0, 1}, U"t=[2,3,4];", 3);
    parser.Parse(LogicalId{0, 0, 2}, U"n=1;", 3);

    //the element access depends on the array itself and on the declared index variable
    Dependencies dependencies;
    parser.Parse(LogicalId{0, 0, 3}, U"t{n};", &dependencies, AngleMeasure::Radian, AngleMeasure::None);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"t") != dependencies.end());
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"n") != dependencies.end());

    //identifiers inside an expression subscript become dependencies too
    dependencies.clear();
    parser.Parse(LogicalId{0, 0, 3}, U"t{n+1};", &dependencies, AngleMeasure::Radian, AngleMeasure::None);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"t") != dependencies.end());
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"n") != dependencies.end());

    dependencies.clear();
    parser.Parse(LogicalId{0, 0, 3}, U"t{size(t)-1};", &dependencies, AngleMeasure::Radian, AngleMeasure::None);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"t") != dependencies.end());
}

}
