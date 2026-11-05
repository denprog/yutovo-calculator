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
using namespace std::chrono_literals;

TEST_F(CalcTestMatrixRational, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToString() == U"[[1,2],[3,4]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1.5,2],[3,4]];");
    ASSERT_TRUE(res.ToString() == U"[[3/2,2],[3,4]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, det_exact_fractions)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1/2,1/3],[1/4,1/5]]);");
    ASSERT_TRUE(res.ToString() == U"[[1/60]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, det_exact_big_integers)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[123456789,987654321],[555555555,444444444]]);");
    ASSERT_TRUE(res.ToString() == U"[[-493827160506172839]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, det2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2],[3,4]]);");
    ASSERT_TRUE(res.ToString() == U"[[-2]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det([[1,2,3],[4,5,6],[7,8,10]]);");
    ASSERT_TRUE(res.ToString() == U"[[-3]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, inverse_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inverse([[4,7],[2,6]]);");
    ASSERT_TRUE(res.ToString() == U"[[3/5,-7/10],[-1/5,2/5]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"inverse([[4,7],[2,6]])*[[4,7],[2,6]];");
    ASSERT_TRUE(res.ToString() == U"[[1,0],[0,1]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"inverse([[1,2],[2,4]]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixRational, rref_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"rref([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToString() == U"[[1,2],[0,0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"rref([[2,4],[1,1]]);");
    ASSERT_TRUE(res.ToString() == U"[[1,0],[0,1]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, rank_and_trace)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"rank([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToString() == U"[[1]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"trace([[1/2,1],[1,1/3]]);");
    ASSERT_TRUE(res.ToString() == U"[[5/6]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, linsolve_exact)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"linsolve([[2,1],[1,3]],[[5],[10]]);");
    ASSERT_TRUE(res.ToString() == U"[[1],[3]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, variables1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2],[3,4]];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A*A;");
    ASSERT_TRUE(res.ToString() == U"[[7,10],[15,22]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"det(A*A);");
    ASSERT_TRUE(res.ToString() == U"[[4]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1/2,1/3],[1/4,1/5]];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A{0,1};");
    ASSERT_TRUE(res.ToString() == U"[[1/3]]") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"i=1;");
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"A{i,0};");
    ASSERT_TRUE(res.ToString() == U"[[1/4]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, transpose_and_size)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"transpose([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToString() == U"[[1,4],[2,5],[3,6]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"size([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToString() == U"[[2,3]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"idn(2);");
    ASSERT_TRUE(res.ToString() == U"[[1,0],[0,1]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixRational, errors)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2],[3]]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det();"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"A{0,0};"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestMatrixRational, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 1}, U"A=[[1,2],[3,4]];");
    parser.Parse(LogicalId{0, 0, 2}, U"x=1;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=2;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2A;");
    ASSERT_TRUE(r.ToString() == U"[[2,4],[6,8]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"2A{0,1};");
    ASSERT_TRUE(r.ToString() == U"[[4]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(x)/(y)A;");
    ASSERT_TRUE(r.ToString() == U"[[1/2,1],[3/2,2]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"(1/2)A;");
    ASSERT_TRUE(r.ToString() == U"[[1/2,1],[3/2,2]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2(A);");
    ASSERT_TRUE(r.ToString() == U"[[2,4],[6,8]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"2det(A);");
    ASSERT_TRUE(r.ToString() == U"[[-4]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 10}, U"det(A)A;");
    ASSERT_TRUE(r.ToString() == U"[[-2,-4],[-6,-8]]") << r.ToStdString(3, 3);
}

}
