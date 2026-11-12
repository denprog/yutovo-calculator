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

TEST_F(CalcTestSymbolicRational, matrix1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1/2,1/3],[1/4,1/5]]);");
    ASSERT_TRUE(res.ToStdString(10) == "1/60") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det([[x,y],[z,w]]);");
    ASSERT_TRUE(res.ToStdString(10) == "x*w-y*z") << res.ToStdString(10);
}

//Symbolic vectors stay exact for rational elements
TEST_F(CalcTestSymbolicRational, vectors1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1/2,1/3],[1/4,1/5]);");
    ASSERT_TRUE(res.ToStdString(10) == "23/120") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([3,4]);");
    ASSERT_TRUE(res.ToStdString(10) == "5") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"normalize([3,4]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[3/5,4/5]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"dot([x,y],[y,x]);");
    ASSERT_TRUE(res.ToStdString(10) == "x*y+y*x") << res.ToStdString(10);
}

//A vector variable stays exact through the rational symbolic parser
TEST_F(CalcTestSymbolicRational, vectors2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=[3,4];");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm(u);");
    ASSERT_TRUE(res.ToStdString(10) == "5") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"normalize(u);");
    ASSERT_TRUE(res.ToStdString(10) == "[[3/5,4/5]]") << res.ToStdString(10);
}

//A user function returning a vector of fractions
TEST_F(CalcTestSymbolicRational, vectors3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(t)=t*[1,2];");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot(f(1/2),[1,1]);");
    ASSERT_TRUE(res.ToStdString(10) == "3/2") << res.ToStdString(10);
}

}
