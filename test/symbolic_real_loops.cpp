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

TEST_F(CalcTestSymbolicReal, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=10,i=i+1,(u=u+i));").ToStdString(3);
    ASSERT_TRUE(s == "55.") << s;
    //the bracketed condition form works the same way
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"for(i=1,(i<=10),i=i+1,(u=u+i));").ToStdString(3);
    ASSERT_TRUE(s == "55.") << s;
}

TEST_F(CalcTestSymbolicReal, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u*i));").ToStdString(3);
    ASSERT_TRUE(s == "120.") << s;
}

TEST_F(CalcTestSymbolicReal, for3)
{
    //a function defined inside the block is used by the following statements of the block
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(f(x)=2*x,u=f(i)));").ToStdString(3);
    ASSERT_TRUE(s == "6.") << s;
}

TEST_F(CalcTestSymbolicReal, for4)
{
    //the value of the last block statement; the loop result is a regular expression operand
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(u=u+i,u*2));").ToStdString(3);
    ASSERT_TRUE(s == "12.") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"2*for(i=1,i<=4,i=i+1,(u=u+i))+1;").ToStdString(3);
    ASSERT_TRUE(s == "21.") << s;
}

TEST_F(CalcTestSymbolicReal, for5)
{
    //the canonical nested-sum idiom (the outer block resets v, the inner block writes through); exact accumulation
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(v=0,for(j=1,j<=4,j=j+1,(v=v+i*j)),u=u+v));").ToStdString(3);
    ASSERT_TRUE(s == "60.") << s;
    //a decimal bound compares exactly as well
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"w=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,i<=2.5,i=i+1,(w=w+i));").ToStdString(3);
    ASSERT_TRUE(s == "3.") << s;
}

TEST_F(CalcTestSymbolicReal, comparison1)
{
    //the ordering comparison operators work for numeric symbolic values
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*(3<=5);").ToStdString(3);
    ASSERT_TRUE(s == "2.") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*(6<=5);").ToStdString(3);
    ASSERT_TRUE(s == "0.") << s;
    //an undecidable comparison of two symbols is an error
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"(x<=y);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestSymbolicReal, for_threads)
{
    constexpr int threads_count = 50;
    constexpr int iterations = 20;

    std::vector<int> failures(threads_count, 0);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int t = 0; t < threads_count; ++t)
    {
        threads.emplace_back(
            [&failures, t]()
            {
                Parser<Symbolic<Real>> p(3, Language::English);
                for (int iter = 0; iter < iterations; ++iter)
                {
                    p.Parse(LogicalId{0, 0, 1}, U"u=0;", 3);
                    std::string s = p.Parse(LogicalId{0, 0, 2}, U"for(i=1,i<=10,i=i+1,(u=u+i));", 10).ToStdString(10);
                    if (s != "55.")
                        ++failures[t];
                }
            });
    }
    for (std::thread& thread : threads)
        thread.join();
    for (int failure : failures)
        ASSERT_TRUE(failure == 0);
}

TEST_F(CalcTestSymbolicReal, for_errors)
{
    //the condition cannot be decided for a symbolic bound
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=n,i=i+1,(u=u+i));"), yutovo_calculator::MathException);
    //the body never executes, so the value of the loop is undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i>1,i=i+1,(u=5));"), yutovo_calculator::MathException);
    //the old loop() still does not evaluate in the symbolic parsers
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"for(i=1,i<=2,i=i+1,(u=u+loop(j=1,(j<=2),j=j+1,t=0,t=t+1)));"),
        yutovo_calculator::MathException);
}

TEST_F(CalcTestSymbolicReal, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (10, 20), (30, 40));").ToStdString(3);
    ASSERT_TRUE(s == "20.") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (10, 20), (30, 40));").ToStdString(3);
    ASSERT_TRUE(s == "40.") << s;
}

TEST_F(CalcTestSymbolicReal, if2)
{
    //the branches keep symbolic results exact
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, (x+x, 3*x), (0));").ToStdString(3);
    ASSERT_TRUE(s == "3.*x") << s;
    //a user function is callable inside the branches
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(t)=2*t;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(3<=3, (f(1/2)), (f(x)));").ToStdString(3);
    ASSERT_TRUE(s == "1.") << s;
}

TEST_F(CalcTestSymbolicReal, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (g(x)=2*x, g(4)), (0));").ToStdString(3);
    ASSERT_TRUE(s == "8.") << s;
    //the if result is a regular expression operand
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*if(5>=4, (10), (20))+1;").ToStdString(3);
    ASSERT_TRUE(s == "21.") << s;
}

TEST_F(CalcTestSymbolicReal, if4)
{
    //a for() body selects its per-iteration value with if(); the exact fraction result prints in the decimal
    //style at the parser precision (like the numeric Symbolic<Real> results do)
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<>2, (1/i), (0))));").ToStdString(3);
    ASSERT_TRUE(s == "1.583") << s;
}

TEST_F(CalcTestSymbolicReal, if5)
{
    //an assignment in the branch writes to an if-local variable; the outer user variable keeps its value
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+1, u*10), (u*100));").ToStdString(3);
    ASSERT_TRUE(s == "20.") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString(3);
    ASSERT_TRUE(s == "1.") << s;
}

TEST_F(CalcTestSymbolicReal, if_threads)
{
    constexpr int threads_count = 50;
    constexpr int iterations = 20;

    std::vector<int> failures(threads_count, 0);
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int t = 0; t < threads_count; ++t)
    {
        threads.emplace_back(
            [&failures, t]()
            {
                Parser<Symbolic<Real>> p(3, Language::English);
                for (int iter = 0; iter < iterations; ++iter)
                {
                    p.Parse(LogicalId{0, 0, 1}, U"u=0;", 3);
                    std::string s = p.Parse(LogicalId{0, 0, 2}, U"if(1<2, (u=u+5, u*2), (u=100));", 10).ToStdString(10);
                    if (s != "10.")
                        ++failures[t];
                }
            });
    }
    for (std::thread& thread : threads)
        thread.join();
    for (int failure : failures)
        ASSERT_TRUE(failure == 0);
}

TEST_F(CalcTestSymbolicReal, if6)
{
    //the condition runs symbolic computations; the branches return symbolic values
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(sqrt(pow(2,2)) == 2, (x+x, 3*x), (0));").ToStdString(3);
    ASSERT_TRUE(s == "3.*x") << s;
    //a nested if over exact fractions decides the branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1/2 > 2/3, (0), (if(2/3 > 1/2, (1/4, 1/4+1/4), (1))));").ToStdString(3);
    ASSERT_TRUE(s == "0.5") << s;
    //a user function in the condition and the branch; the if-local temp shadows the user variable
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"f(x)=2*x;");
    parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"u=1;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"if(f(3) > 5, (u=f(3), if(u>5, (u*10), (0)), u), (0));").ToStdString(3);
    ASSERT_TRUE(s == "6.") << s;
}

TEST_F(CalcTestSymbolicReal, if7)
{
    //a piecewise fraction-valued sum inside a loop with two nesting levels stays exact
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<=2,(1/i),(if(i==3,(1/2),(1/4))))));").ToStdString(3);
    ASSERT_TRUE(s == "2.25") << s;
}

TEST_F(CalcTestSymbolicReal, if_errors)
{
    //the condition cannot be decided for two unknown symbols
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(x<=y, (1), (0));"), yutovo_calculator::MathException);
    //neither branch can end with a function definition
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (f(x)=x), (0));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestSymbolicReal, relational_eq)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x == y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, relational_ne)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x != y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, relational_lt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x < y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, relational_le)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x <= y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, relational_gt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x > y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, relational_ge)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x >= y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, logical_not)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"!x;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, logical_and)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x && y;"), SyntaxException);
}


TEST_F(CalcTestSymbolicReal, logical_or)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x || y;"), SyntaxException);
}


}
