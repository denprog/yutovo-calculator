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

TEST_F(CalcTestVectorComplex, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1+i,2];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0+i*1.E+0],[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, imaginary_unit)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*1.E+0]]") << res.ToStdString(3, 3);
    //implicit string multiplication 2i
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2i;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, arithmetic1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1+i,2-i]+[1-i,2+i];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[4.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*[1+i,1];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0+i*2.E+0],[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, dot_complex)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1+i,1-i],[1-i,1+i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot([i,0],[i,0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-1.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, cross_complex)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cross([i,0,0],[0,i,0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.E+0],[0.E+0],[-1.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, norm_complex)
{
    //the norm of a complex vector is the square root of the sum of the squared modules
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"norm([3,4*i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([1+i,1-i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"abs([3,4*i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, normalize_complex)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"normalize([3,4*i]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.6E+0],[i*0.8E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, angle1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"angle([1,0],[0,1]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.571E+0]]") << res.ToStdString(3, 3);
    //the complex arccos of an exact 1 carries the engine's numeric noise, like arccos(1) itself
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"angle([1,0],[1,0]);");
    ASSERT_TRUE(abs(res.GetScalar().GetRe()) <= Real(3, 1e-15f)) << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, size1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"size([4*i,5,6]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorComplex, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=[1+i,2,3*i];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v{0};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0+i*1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v{2};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[i*3.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"v{3};"), yutovo_calculator::SyntaxException);
}

}
