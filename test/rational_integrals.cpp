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

TEST_F(CalcTestRational, definite_integral1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,x);");
    ASSERT_TRUE(res.ToStdString() == "1/2") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,pow(x,2),x);");
    ASSERT_TRUE(res.ToStdString() == "1/3") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,2,pow(x,2),x);");
    ASSERT_TRUE(res.ToStdString() == "8/3") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,pow(x,3),x);");
    ASSERT_TRUE(res.ToStdString() == "1/4") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral5)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,pow((x+1),2),x);");
    ASSERT_TRUE(res.ToStdString() == "7/3") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral_unit_limits)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0m,1m,x,x);");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::IncorrectOperation) << ex.ex_id;
    }
}

TEST_F(CalcTestRational, definite_integral6)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,10,x,x);");
    ASSERT_TRUE(res.ToStdString() == "50") << res.ToStdString();
}

TEST_F(CalcTestRational, definite_integral7)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,2*pow(x,2)+3*x+1,x);");
    ASSERT_TRUE(res.ToStdString() == "19/6") << res.ToStdString();
}

}
