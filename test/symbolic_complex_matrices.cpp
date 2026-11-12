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

TEST_F(CalcTestSymbolicComplex, matrix1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[i,0],[0,i]]);");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
}

//Symbolic vectors over complex elements
TEST_F(CalcTestSymbolicComplex, vectors1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1+i,1-i],[1-i,1+i]);");
    ASSERT_TRUE(res.ToStdString(10) == "4") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([3,4*i]);");
    ASSERT_TRUE(res.ToStdString(10) == "5") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"cross([i,0,0],[0,i,0]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[0,0,-1]]") << res.ToStdString(10);
}

//Complex vector variables in the dot product
TEST_F(CalcTestSymbolicComplex, vectors2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=[1+i,1];");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v=[1-i,1];");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"dot(u,v);");
    ASSERT_TRUE(res.ToStdString(10) == "3") << res.ToStdString(10);
}

//The matrix-vector product with complex entries
TEST_F(CalcTestSymbolicComplex, vectors3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[i,0],[0,1]]*[[1],[2]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[i],[2]]") << res.ToStdString(10);
}

}
