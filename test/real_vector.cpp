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

TEST_F(CalcTestVectorReal, literals1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0],[3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1],[2],[3]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0],[3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[[1],[2],[3]];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0],[3.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, literals_scalar)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.25;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.25E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, literals_errors)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[];"), yutovo_calculator::MathException);
    //a nested group with several elements is a matrix shape, not a vector literal
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[[1,2],[3]];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[[1,2],[3,4]];"), yutovo_calculator::MathException);
}

//Loops are impossible in the vector parsers: the vector comparison operators always throw IncorrectComparison
TEST_F(CalcTestVectorReal, for1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(u=u+i));"), yutovo_calculator::MathException);
}

//if() is impossible in the vector parsers for the same reason: the condition comparison always throws
TEST_F(CalcTestVectorReal, if1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, ([1,2]), ([3,4]));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, arithmetic1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2]+[10,20];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[11.E+0],[22.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[10,20]-[1,2];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[9.E+0],[18.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, arithmetic2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*[1,2,3];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[4.E+0],[6.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1,2,3]*2;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[4.E+0],[6.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[1,2,3]/2;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.5E+0],[1.E+0],[1.5E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, arithmetic3)
{
    //the product of two multi-element vectors is the dot product
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3]*[4,5,6];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[32.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*([1,2,3]*[4,5,6]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[64.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"[1,2]*[1,2,3];"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, arithmetic4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-[1,2];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-1.E+0],[-2.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[1,2]+0;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0],[2.E+0]]") << res.ToStdString(3, 3);
    //a scalar does not broadcast over a multi-element vector
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"5+[1,2];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"[1,2]/[3,4];"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, dot1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1,2,3],[4,5,6]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[32.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dot([[1],[2]],[[3],[4]]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[11.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"dot([1,2],[1,2,3]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, cross1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cross([1,0,0],[0,1,0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.E+0],[0.E+0],[1.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"cross([2,3,4],[5,6,7]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[-3.E+0],[6.E+0],[-3.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"cross([1,2],[3,4]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, norm1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"norm([3,4]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm([1,2,2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"abs([3,4]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"norm(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[5.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"norm([2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, norm_precision)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"norm([1,2]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.236E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, normalize1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"normalize([3,4]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.6E+0],[0.8E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"norm(normalize([3,4]));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"normalize([0,0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, angle1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"angle([1,0],[0,1]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.571E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"angle([1,0],[1,0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"angle([1,1],[1,0]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[0.785E+0]]") << res.ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"angle([1,0],[0,0]);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"angle([1,0],[1,0,0]);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestVectorReal, size1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"size([4,5,6]);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[3.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"size(7);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[1.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, element_access1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=[10,20,30];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v{0};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[10.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v{2};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[30.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"v{1+1};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[30.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"2*v{0};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[20.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, element_access2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=[10,20,30];");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i=1;");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v{i};");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[20.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, element_access_errors)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=[10,20,30];");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v{3};"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v{-1};"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"v{0.5};"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestVectorReal, variables1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=[1,2];");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"w=[3,4];");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u+w;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[4.E+0],[6.E+0]]") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"dot(u,w);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[11.E+0]]") << res.ToStdString(3, 3);
    //implicit string multiplication
    res = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"2u;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*2;");
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"[f(1),f(2)];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[[2.E+0],[4.E+0]]") << res.ToStdString(3, 3);
}

TEST_F(CalcTestVectorReal, unknown_identifier)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot(q,w);"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"sin([1,2]);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestVectorReal, threads_dot)
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
                    Parser<Vector<Real>> thread_parser(3, Language::English);
                    auto res = thread_parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dot([1,2,3],[4,5,6])+norm([3,4]);");
                    results[thread_index] = res.ToStdString(3, 3);
                }
            });
    }
    for (auto& thread : threads)
        thread.join();
    for (auto& result : results)
    {
        ASSERT_TRUE(result == "[[37.E+0]]") << result;
    }
}

}
