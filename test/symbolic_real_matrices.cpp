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

TEST_F(CalcTestSymbolicReal, matrix1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[x,y],[z,w]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[x,y],[z,w]]") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2],[3,4]]);");
    ASSERT_TRUE(res.ToStdString(10) == "-2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[x,2],[3,4]]);");
    ASSERT_TRUE(res.ToStdString(10) == "-6.+4.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix4)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[x,y],[z,w]];");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det(A);");
    ASSERT_TRUE(res.ToStdString(10) == "x*w-y*z") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"transpose([[x,y],[z,w]]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[x,z],[y,w]]") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inverse([[x,0],[0,y]]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[1/x,0],[0,1/y]]") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"trace([[x,y],[z,w]]);");
    ASSERT_TRUE(res.ToStdString(10) == "w+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"rank([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"rref([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[1,2],[0,0]]") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]]*[[5,6],[7,8]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[19,22],[43,50]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[2,4],[6,8]]") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, matrix10)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[x,y]]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[x,y],[z]];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"det();"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicReal, matrix11)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[x,y],[z,w]];");
    std::string json = res.ToJson(10);
    std::string expected =
        "{\"type\":45,\"elements\":["
            "{\"type\":60,\"rows\":2,\"columns\":2,\"elements\":["
                "{\"type\":10,\"elements\":[]},"
                "{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},"
                "{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"y\"}]},"
                "{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"z\"}]},"
                "{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"w\"}]},"
                "{\"type\":10,\"elements\":[]}"
            "]}"
        "]}";
    ASSERT_TRUE(json == expected) << json;
}

TEST_F(CalcTestSymbolicReal, matrix_implicit)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[x,y],[z,w]];");
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2A;");
    ASSERT_TRUE(r.ToStdString(10) == "[[x*2,y*2],[z*2,w*2]]") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"2det(A);");
    ASSERT_TRUE(r.ToStdString(10) == "2.*(x*w-y*z)") << r.ToStdString(10);
}

//Symbolic vectors: a vector literal is a single-row matrix, the operations go through giac
TEST_F(CalcTestSymbolicReal, vectors1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1,2,3],[4,5,6]);");
    ASSERT_TRUE(res.ToStdString(10) == "32.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot([[1],[2]],[[3],[4]]);");
    ASSERT_TRUE(res.ToStdString(10) == "11.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"norm([3,4]);");
    ASSERT_TRUE(res.ToStdString(10) == "5.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"normalize([3,4]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[3/5,4/5]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"cross([1,0,0],[0,1,0]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[0,0,1]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"angle([1,0],[0,1]);");
    ASSERT_TRUE(res.ToStdString(10) == "1.571") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, vectors_symbolic_arguments)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([x,y],[y,x]);");
    ASSERT_TRUE(res.ToStdString(10) == "x*y+y*x") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"cross([x,y,z],[1,0,0]);");
    ASSERT_TRUE(res.ToStdString(10) == "[[0,z,-y]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"norm([x,y]);");
    ASSERT_TRUE(res.ToStdString(10) == "sqrt(pow(x,2.)+pow(y,2.))") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, vectors_matrix_product)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]]*[[1],[2]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[5],[11]]") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1,2,3]*[[1],[2],[3]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[14]]") << res.ToStdString(10);
}

//Symbolic vector variables: the dot product of a vector with itself
TEST_F(CalcTestSymbolicReal, vectors2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=[x,y];");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot(u,u);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,2.)+pow(y,2.)") << res.ToStdString(10);
}

//A user function returning a vector, used inside the dot product
TEST_F(CalcTestSymbolicReal, vectors3)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(t)=t*[1,2];");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot(f(2),[1,1]);");
    ASSERT_TRUE(res.ToStdString(10) == "6.") << res.ToStdString(10);
}

//Vector operations inside a bigger expression
TEST_F(CalcTestSymbolicReal, vectors4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*dot([1,2],[3,4])+norm([3,4]);");
    ASSERT_TRUE(res.ToStdString(10) == "27.") << res.ToStdString(10);
}

//The matrix-vector product with symbolic entries
TEST_F(CalcTestSymbolicReal, vectors5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[x,0],[0,y]]*[[1],[2]];");
    ASSERT_TRUE(res.ToStdString(10) == "[[x],[y*2]]") << res.ToStdString(10);
}

}
