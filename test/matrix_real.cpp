/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "mock.h"
#include <thread>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestMatrixReal, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,2.E+0],[3.E+0,4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, literals2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,2.E+0,3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1],[2],[3]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0],[3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, literals_scalar)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.25;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.25E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, literals_errors)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3]];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1,2],3];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[];"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixReal, arithmetic1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]]+[[10,20],[30,40]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[11.E+0,22.E+0],[33.E+0,44.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[10,20],[30,40]]-[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[9.E+0,18.E+0],[27.E+0,36.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, arithmetic2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0,4.E+0],[6.E+0,8.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1,2],[3,4]]*2;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0,4.E+0],[6.E+0,8.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, arithmetic3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[1,2],[3,4]]*[[5,6],[7,8]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[19.E+0,22.E+0],[43.E+0,50.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1,2],[3,4]]*[[1,0],[0,1]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,2.E+0],[3.E+0,4.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[[1,2]]*[[1,2]];"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixReal, arithmetic4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-[[1,2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-1.E+0,-2.E+0],[-3.E+0,-4.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[4,7],[2,6]]/2;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0,3.5E+0],[1.E+0,3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, det1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2],[3,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, det2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2,3],[4,5,6],[7,8,10]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, det_singular)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, det_errors)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2,3],[4,5,6]]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det();"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"det([[1,2],[3,4]],[[1,2],[3,4]]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestMatrixReal, det_precision)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[[0.333333333333333333333333333333,0.666666666666666666666666666667]];");
    ASSERT_TRUE(res.ToStdString(30, 30) == "[[0.333333333333333333333333333333E+0,0.666666666666666666666666666667E+0]]")
        << res.ToStdString(30, 30);
}

TEST_F(CalcTestMatrixReal, trace)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"trace([[1,2],[3,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"trace([[1,2,3],[4,5,6]]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixReal, transpose)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"transpose([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,4.E+0],[2.E+0,5.E+0],[3.E+0,6.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, identity_and_size)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"idn(3);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,0.E+0,0.E+0],[0.E+0,1.E+0,0.E+0],[0.E+0,0.E+0,1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"rows([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"cols([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"size([[1,2,3],[4,5,6]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0,3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, inverse1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inverse([[2,0],[0,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.5E+0,0.E+0],[0.E+0,0.25E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, inverse2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inverse([[4,7],[2,6]])*[[4,7],[2,6]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,0.E+0],[0.E+0,1.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"inverse([[1,2],[2,4]]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixReal, rref_and_rank)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"rref([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0,2.E+0],[0.E+0,0.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"rank([[1,2],[2,4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"rank(idn(4));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, linsolve1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"linsolve([[2,0],[0,4]],[[2],[8]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"linsolve([[1,2],[2,4]],[[1],[2]]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestMatrixReal, variables1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2],[3,4]];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A*A;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[7.E+0,10.E+0],[15.E+0,22.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"det(A*A);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*2;");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[f(1),2],[3,4]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0,2.E+0],[3.E+0,4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2],[3,4]];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A{0,1};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"A{1+0,1*1};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"2*A{0,0};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, element_access2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2],[3,4]];");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i=1;");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"A{i,i};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestMatrixReal, element_access_errors)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"A=[[1,2],[3,4]];");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"A{2,0};"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"A{0,0.5};"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"A{0};"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestMatrixReal, unknown_identifier)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det(B);"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"sin([[1,2],[3,4]]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestMatrixReal, threads_det)
{
    std::vector<std::string> results(8);
    std::vector<std::thread> threads;
    for (int thread_index = 0; thread_index < 8; ++thread_index)
    {
        threads.emplace_back(
            [&results, thread_index]()
            {
                for (int iteration = 0; iteration < 20; ++iteration)
                {
                    Parser<Matrix<Real>> thread_parser(3, Language::English);
                    auto res = thread_parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1,2,3],[4,5,6],[7,8,10]]);");
                    results[thread_index] = res.ToStdString(3, 3);
                }
            });
    }
    for (auto& thread : threads)
        thread.join();
    for (auto& result : results)
    {
        ASSERT_TRUE(result == "[[-3.E+0]]") << result;
    }
}

TEST_F(CalcTestMatrixReal, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 1}, U"A=[[1,2],[3,4]];");
    parser.Parse(LogicalId{0, 0, 2}, U"x=2;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=4;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[2.E+0,4.E+0],[6.E+0,8.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"2A{0,1};");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[4.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(x)/(y)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[0.5E+0,1.E+0],[1.5E+0,2.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"(1/2)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[0.5E+0,1.E+0],[1.5E+0,2.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2(A);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[2.E+0,4.E+0],[6.E+0,8.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"2det(A);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[-4.E+0]]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 10}, U"det(A)A;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[[-2.E+0,-4.E+0],[-6.E+0,-8.E+0]]") << r.ToStdString(3, 3);
}

}
