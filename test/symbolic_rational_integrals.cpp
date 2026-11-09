/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestSymbolicRational, indefinite_integral1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(pow(x,2), x);");
    ASSERT_TRUE(res.ToStdString(0) == "(1/3)*pow(x,3)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, indefinite_integral2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x*y, y);");
    ASSERT_TRUE(res.ToStdString(0) == "(1/2)*x*pow(y,2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, indefinite_integral_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, 1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"indefinite_integral(x, x+1);"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicRational, definite_integral1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(2,34,3y,x);");
    ASSERT_TRUE(res.ToStdString(0) == "96*y") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, definite_integral3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x*y,y);");
    ASSERT_TRUE(res.ToStdString(0) == "(1/2)*x") << res.ToStdString(0);
}

}
