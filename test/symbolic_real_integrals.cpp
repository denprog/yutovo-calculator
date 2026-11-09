/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include <clocale>
#include <condition_variable>
#include <cstdlib>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestSymbolicReal, indefinite_integral1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(pow(x,2), x);");
    ASSERT_TRUE(res.ToStdString(10) == "0.333*pow(x,3.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, indefinite_integral2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(sin(x), x);");
    ASSERT_TRUE(res.ToStdString(10) == "-cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, indefinite_integral3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x*y, y);");
    ASSERT_TRUE(res.ToStdString(10) == "0.5*x*pow(y,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, indefinite_integral_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, 1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, x+1);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, definite_integral_simplify1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(2,34,3y,x);");
    ASSERT_TRUE(res.ToStdString(10) == "96.*y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x*y,y);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "0.5*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral_unit_limits1)
{
    parser.SetLocale(Language::Russian);
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"t1=0мс;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"t2=1мс;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"definite_integral(t1,t2,t,t);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "0.5*pow(мс,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"0.5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"мс"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,x+y);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, definite_integral_inf_divergent)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(1,∞,1/x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral_inf6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,1/x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral_user_function)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"iff(t)=5*pow(e,-2t);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"definite_integral(0,1,iff(t),t);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "2.1616617919") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, definite_integral_constant_with_e)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,5*pow(e,-2t),t);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "2.1616617919") << res.ToStdString(10);
}

}
