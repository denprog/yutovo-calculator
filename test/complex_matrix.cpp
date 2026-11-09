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

TEST_F(CalcTestMatrixComplex, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2i],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,i*2.E+0],[3.E+0,4.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1+1i,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0+i*1.E+0,2.E+0],[3.E+0,4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, det1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2i],[0,1]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det([[i,0],[0,i]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-1.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, det2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1+1i,2],[3,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-2.E+0+i*4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, arithmetic1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,i],[0,1]]*[[1,i],[0,1]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,i*2.E+0],[0.E+0,1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i*[[1,0],[0,1]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*1.E+0,0.E+0],[0.E+0,i*1.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, inverse1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inverse([[2i,0],[0,4]])*[[2i,0],[0,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,0.E+0],[0.E+0,1.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"inverse([[i,i],[1,1]]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixComplex, trace_and_rank)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"trace([[i,0],[0,2i]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"rank([[i,0],[0,i]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2i],[3,4]];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A{0,1};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*2.E+0]]") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"j0=1;");
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"A{j0,1};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, linsolve1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"linsolve([[i,0],[0,2]],[[2i],[4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixComplex, errors)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2i],[3]]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det([[1,2],[3,4]],1);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestMatrixComplex, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 1}, U"A=[[2i,0],[0,4]];");
    parser.Parse(LogicalId{0, 0, 2}, U"x=1;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=2;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*4.E+0,0.E+0],[0.E+0,8.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"2A{0,0};");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*4.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(x)/(y)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*1.E+0,0.E+0],[0.E+0,2.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"(1/2)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*1.E+0,0.E+0],[0.E+0,2.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2(A);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*4.E+0,0.E+0],[0.E+0,8.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"2det(A);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[i*16.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 10}, U"det(A)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[-16.E+0,0.E+0],[0.E+0,i*32.E+0]]") << r.ToStdString(3, 3);
}

}
