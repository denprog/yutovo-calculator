/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestVectorRational, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3];");
    ASSERT_TRUE(res.ToString() == U"[[1],[2],[3]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1.5,2];");
    ASSERT_TRUE(res.ToString() == U"[[3/2],[2]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[[1],[2]];");
    ASSERT_TRUE(res.ToString() == U"[[1],[2]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, arithmetic1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1/2,1/3]+[1/4,1/5];");
    ASSERT_TRUE(res.ToString() == U"[[3/4],[8/15]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*[1,2,3];");
    ASSERT_TRUE(res.ToString() == U"[[2],[4],[6]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, dot_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1/2,1/3],[1/4,1/5]);");
    ASSERT_TRUE(res.ToString() == U"[[23/120]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1,2,3]*[4,5,6];");
    ASSERT_TRUE(res.ToString() == U"[[32]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, cross_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cross([1,2,3],[4,5,6]);");
    ASSERT_TRUE(res.ToString() == U"[[-3],[6],[-3]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"cross([1/2,0,0],[0,1/3,0]);");
    ASSERT_TRUE(res.ToString() == U"[[0],[0],[1/6]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, norm_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"norm([3,4]);");
    ASSERT_TRUE(res.ToString() == U"[[5]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([1,2,2]);");
    ASSERT_TRUE(res.ToString() == U"[[3]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"norm([3/10,4/10]);");
    ASSERT_TRUE(res.ToString() == U"[[1/2]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"abs([3,4]);");
    ASSERT_TRUE(res.ToString() == U"[[5]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, norm_irrational)
{
    //the square root of 2 and of 5 is not rational, so the norm is not representable
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"norm([1,1]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([1,2]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorRational, normalize_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"normalize([3,4]);");
    ASSERT_TRUE(res.ToString() == U"[[3/5],[4/5]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"normalize([1,1]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"normalize([0,0]);"), yutovo_calculator::MathException);
}

//Angle is not registered for rational vectors: the arccosine is generally not rational
TEST_F(CalcTestVectorRational, angle_not_registered)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"angle([1,0],[0,1]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestVectorRational, size1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"size([4,5,6]);");
    ASSERT_TRUE(res.ToString() == U"[[3]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorRational, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=[1/2,2/3,3/4];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v{1};");
    ASSERT_TRUE(res.ToString() == U"[[2/3]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v{3};"), yutovo_calculator::SyntaxException);
}

}
