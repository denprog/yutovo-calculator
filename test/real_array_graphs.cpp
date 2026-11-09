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

TEST_F(CalcTestArrayReal, graph1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,50);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3).substr(0, 100) == 
        "[0.E+0,1.E+0,0.E+0,1.E+0,0.E+0,0.E+0,0.02E+0,0.02E+0,0.04E+0,0.04E+0,0.06E+0,0.06E+0,0.08E+0,0.0799E") <<
        res.ToStdString(3, 3).substr(0, 100);
}

TEST_F(CalcTestArrayReal, graph2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,100,0,100,100);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3).substr(0, 100) == 
        "[0.E+0,100.E+0,0.E+0,100.E+0,0.E+0,0.E+0,1.E+0,0.841E+0,2.E+0,0.909E+0,3.E+0,0.141E+0,4.E+0,-0.757E+") << 
        res.ToStdString(3, 3).substr(0, 100);
}

TEST_F(CalcTestArrayReal, graph3)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,1,1,0,100,100);", &parser_context), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,-1,1,10,10,10);", &parser_context), yutovo_calculator::MathException);
}

TEST_F(CalcTestArrayReal, graph4)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"graph_line(abs(x),x{y},-1,1,0,1,100);", &parser_context);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 18);
    }
}

TEST_F(CalcTestArrayReal, graph5)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line((pow(x,2))/(sqrt(x)),x,0,10,0,10,100);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3).substr(0, 100) == 
        "[0.E+0,10.E+0,0.E+0,10.E+0,0.E+0,NAN,0.1E+0,0.0316E+0,0.2E+0,0.0894E+0,0.3E+0,0.164E+0,0.4E+0,0.253E") << 
        res.ToStdString(3, 3).substr(0, 100);
}

TEST_F(CalcTestArrayReal, graph6)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line((pow(x,2))/(sqrt(x)),x,-0.5,10,-1,10,100);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3).substr(0, 200) == 
        "[-0.5E+0,10.E+0,-1.E+0,10.E+0,-0.5E+0,NAN,-0.395E+0,NAN,-0.29E+0,NAN,-0.185E+0,NAN,-0.08E+0,NAN,0.025E+0,0.00395E+0,"\
        "0.13E+0,0.0469E+0,0.235E+0,0.114E+0,0.34E+0,0.198E+0,0.445E+0,0.297E+0,0.55E+0,0.408") << 
        res.ToStdString(3, 3).substr(0, 200);
}

TEST_F(CalcTestArrayReal, graph7)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,10000);", AngleMeasure::Radian, AngleMeasure::Radian);
    auto res_c = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,1000000000);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res_c.ToStdString(3, 3) == res.ToStdString(3, 3)) << res_c.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,2);", AngleMeasure::Radian, AngleMeasure::Radian);
    res_c = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,1);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res_c.ToStdString(3, 3) == res.ToStdString(3, 3)) << res_c.ToStdString(3, 3);

    res_c = parser.Parse(LogicalId{0, 0, 1}, U"graph_line(sin(x),x,0,1,0,1,0);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res_c.ToStdString(3, 3) == res.ToStdString(3, 3)) << res_c.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, graph8)
{
    std::u32string expression = U"x";
    for (int i = 0; i < 3000; ++i)
        expression += U"*x";

    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_line(" + expression + U",x,0,1,0,1,10000);", &parser_context), 
        yutovo_calculator::TimeExceedException);
}

TEST_F(CalcTestArrayReal, surface_graph1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(x+y,x,y,0,1,0,1,3,3);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.E+0,1.E+0,0.E+0,1.E+0,3.E+0,3.E+0,0.E+0,0.5E+0,1.E+0,0.5E+0,1.E+0,1.5E+0,1.E+0,1.5E+0,2.E+0]") <<
        res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, surface_graph2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(sin(x)+cos(y),x,y,0,1,0,1,2,2);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.E+0,1.E+0,0.E+0,1.E+0,2.E+0,2.E+0,1.E+0,1.841E+0,0.54E+0,1.382E+0]") <<
        res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, surface_graph3)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(sqrt(x),x,y,-1,1,0,1,3,3);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[-1.E+0,1.E+0,0.E+0,1.E+0,3.E+0,3.E+0,NAN,0.E+0,1.E+0,NAN,0.E+0,1.E+0,NAN,0.E+0,1.E+0]") <<
        res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, surface_graph4)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(x+y,x,y,1,1,0,1,10,10);", &parser_context), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(x+y,x,y,-1,1,0,0,10,10);", &parser_context), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(x+y,x,x,-1,1,0,1,10,10);", &parser_context), yutovo_calculator::MathException);
}

TEST_F(CalcTestArrayReal, surface_graph5)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_surface(x*y+x+y,x,y,0,1,0,1,1000,1000);", &parser_context),
        yutovo_calculator::TimeExceedException);
}

TEST_F(CalcTestArrayReal, graph_bar1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_bar([1,5,3,2]);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.5E+0,4.5E+0,0.E+0,5.E+0,4.E+0,1.E+0,5.E+0,3.E+0,2.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, graph_bar2)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_bar([-2,3]);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.5E+0,2.5E+0,-2.E+0,3.E+0,2.E+0,-2.E+0,3.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, graph_bar3)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"graph_bar([]);", &parser_context), yutovo_calculator::MathException);
}

TEST_F(CalcTestArrayReal, graph_bar4)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_bar(5);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.5E+0,1.5E+0,0.E+0,5.E+0,1.E+0,5.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, graph_bar5)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_bar([sqrt(-1),1]);", &parser_context);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.5E+0,2.5E+0,0.E+0,1.E+0,2.E+0,NAN,1.E+0]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, graph_bar6)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"graph_bar([0,0]);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.ToStdString(3, 3) == "[0.5E+0,2.5E+0,0.E+0,1.E+0,2.E+0,0.E+0,0.E+0]") << res.ToStdString(3, 3);
}

}
