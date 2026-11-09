/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, sum1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;

    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 0, 0, 2}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, sum2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=0,(j<=20),j=j+1,t2=0,t2=t2+i+j));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.465E+3") << s;
}

TEST_F(CalcTestReal, prod1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.048E+3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, prod2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"loop(i=1,(i<=10),i=i+1,i_=1,i_=i_*(i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, max_time1)
{
    yutovo_calculator::ParserContext parser_context;
#ifdef NDEBUG
    parser_context.Init(100);
#elif defined(_WIN32)
    parser_context.Init(200);
#else
    parser_context.Init(1000);
#endif
    parser.SetMaxCastUnitSize(3);
    EXPECT_THROW(parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));", 
        &parser_context)).ToStdString(3, 3), yutovo_calculator::TimeExceedException);
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    parser_context.Init(20000);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));", 
        &parser_context)).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(N)") << s;
}

TEST_F(CalcTestReal, max_time2)
{
    yutovo_calculator::ParserContext parser_context;
#ifdef NDEBUG
    parser_context.Init(2000);
#else
    parser_context.Init(5000);
#endif
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(234)/((3)/((4)/((6)/((7)/((6)/((7)/((2)/(4))))))));", &parser_context),
        yutovo_calculator::TimeExceedException);
    parser_context.Init(0);
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"123;", &parser_context).ToStdString(3, 3);
    ASSERT_TRUE(s == "123.E+0") << s;
}

TEST_F(CalcTestReal, max_time3)
{
#ifdef NDEBUG
    parser.SetDefaultMaxTime(100);
#elif defined(_WIN32)
    parser.SetDefaultMaxTime(200);
#else
    parser.SetDefaultMaxTime(1000);
#endif
    //the loop condition never becomes false (the counter overshoots 5), but the default time limit is armed even without a passed context
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<>5),i=i+2,t=0,t=t+i);"), yutovo_calculator::TimeExceedException);
    parser.SetDefaultMaxTime(60000);
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
}

TEST_F(CalcTestReal, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=10,i=i+1,(u=u+i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
    //a single parse works too: the block statements run with a synthesized id bigger than the statement's id,
    //so a variable defined by a previous statement of the same script is visible inside the block
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v=0;for(i=1,i<=10,i=i+1,(v=v+i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;

    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 0, 0, 2}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "120.E+0") << s;
    //an assignment in the block resets the variable on every iteration
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"for(i=1,i<=5,i=i+1,(u=1,u=u*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestReal, for3)
{
    //a function defined inside the block is used by the following statements of the block
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(f(x)=2*x,u=f(i)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
}

TEST_F(CalcTestReal, for4)
{
    //a function defined inside the block survives the loop and is callable from a later statement
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=0,i<=1,i=i+1,(f(x)=x+1,u=f(i)));");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"f(3);").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
}

TEST_F(CalcTestReal, for5)
{
    //the value of the loop is the value of the last block statement of the last iteration
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(u=u+i,u*2));").ToStdString(3, 3);
    ASSERT_TRUE(s == "12.E+0") << s;
}

TEST_F(CalcTestReal, for6)
{
    //the condition in brackets is accepted as well
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,(i<=10),i=i+1,(u=u+i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
}

TEST_F(CalcTestReal, for7)
{
    //the loop result is a regular expression operand; an inner sum loop runs inside the block
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*for(i=1,i<=4,i=i+1,(u=u+i))+1;").ToStdString(3, 3);
    ASSERT_TRUE(s == "21.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"w=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,i<=3,i=i+1,(w=w+loop(j=1,(j<=3),j=j+1,t=0,t=t+i*j)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "36.E+0") << s;
}

TEST_F(CalcTestReal, for8)
{
    //the canonical nested-sum idiom: the outer block resets the inner accumulator v once per outer iteration,
    //the inner block writes through to the outer temp v, and u accumulates the per-iteration sums;
    //an assignment inside an inner block without an enclosing temp would be local to one run of the inner loop
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(v=0,for(j=1,j<=4,j=j+1,(v=v+i*j)),u=u+v));").ToStdString(3, 3);
    ASSERT_TRUE(s == "60.E+0") << s;
    //a for() inside the accumulator body of the old loop(); the user variable v re-seeds every run of the inner block
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"v=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"loop(i=1,(i<=3),i=i+1,t=0,t=t+for(j=1,j<=2,j=j+1,(v=v+j)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0") << s;
}

TEST_F(CalcTestReal, for9)
{
    //the body may modify the counter; the increment is still applied after the body
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=0,i<6,i=i+1,(i=i+1,u=u+i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0") << s;
    //a bare function call as the last statement; the last case is a parenthesized expression statement
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"f(x)=x+1;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,i<=3,i=i+1,(u=i,f(u)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"u=0;for(i=1,i<=2,i=i+1,(u=u+i,(u)*2));").ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
    //a function defined in a nested inner block survives both loops and is callable after them
    s = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"for(i=1,i<=1,i=i+1,(for(j=1,j<=1,j=j+1,(g(x)=x*3,u=g(j)))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"g(2);").ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
}

TEST_F(CalcTestReal, for_threads)
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
                Parser<Real> p(3, Language::English);
                for (int iter = 0; iter < iterations; ++iter)
                {
                    p.Parse(LogicalId{0, 0, 1}, U"u=0;");
                    std::string s = p.Parse(LogicalId{0, 0, 2}, U"for(i=1,i<=10,i=i+1,(u=u+i));").ToStdString(3, 3);
                    if (s != "55.E+0")
                        ++failures[t];
                }
            });
    }
    for (std::thread& thread : threads)
        thread.join();
    for (int failure : failures)
        ASSERT_TRUE(failure == 0);
}

TEST_F(CalcTestReal, for_identifier)
{
    //'for' without a call is still a valid identifier
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for;").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestReal, for_errors)
{
    //the body never executes, so the value of the loop is undefined
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i>1,i=i+1,(u=5));"), yutovo_calculator::MathException);
    //the block cannot end with a function definition
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(u=i,f(x)=x));"), yutovo_calculator::MathException);
    //an empty block is a syntax error
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"for(i=1,i<=3,i=i+1,());"), yutovo_calculator::SyntaxException);
    //the condition must be a comparison
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,10,i=i+1,(u=1));"), yutovo_calculator::SyntaxException);
    //an unknown name on the right side of an assignment
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"for(i=1,i<=3,i=i+1,(u=u+i));"), yutovo_calculator::SyntaxException);
    //a missing argument and an extra argument after the block
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"for(i=1,i<=3);"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"for(i=1,i<=3,i=i+1,(u=1),5);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (10, 20), (30, 40));").ToStdString(3, 3);
    ASSERT_TRUE(s == "20.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (10, 20), (30, 40));").ToStdString(3, 3);
    ASSERT_TRUE(s == "40.E+0") << s;
    //a single statement in a branch; the condition in brackets is accepted as well
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if((1==1), (5), (6));").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestReal, if2)
{
    //an assignment in the branch writes to an if-local variable read by the following statements of the
    //branch, while the outer user variable keeps its old value
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+1, u*10), (u*100));").ToStdString(3, 3);
    ASSERT_TRUE(s == "60.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
    //the false branch runs when the condition fails
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(1>2, (u=u+1, u*10), (u*100));").ToStdString(3, 3);
    ASSERT_TRUE(s == "500.E+0") << s;
}

TEST_F(CalcTestReal, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(4)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "8.E+0") << s;
    //a function defined inside the branch survives the if and is callable from a later statement
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1==1, (g(x)=x+1, g(0)), (0));");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"g(3);").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
}

TEST_F(CalcTestReal, if4)
{
    //the if result is a regular expression operand
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*if(5>=4, (10), (20))+1;").ToStdString(3, 3);
    ASSERT_TRUE(s == "21.E+0") << s;
}

TEST_F(CalcTestReal, if5)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    //a for() body selects its per-iteration value with if()
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u+if(i<>2, (1), (0))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
    //an if() branch runs a whole loop
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(1<2, (for(i=1,i<=4,i=i+1,(u=u+i))), (u=100));").ToStdString(3, 3);
    ASSERT_TRUE(s == "10.E+0") << s;
}

TEST_F(CalcTestReal, if6)
{
    //a nested if() selects between three values
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x=7;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(x<0, (-1), (if(x==0, (0), (1))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"x=-2;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(x<0, (-1), (if(x==0, (0), (1))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "-1.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"x=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"if(x<0, (-1), (if(x==0, (0), (1))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestReal, if7)
{
    //user functions are callable inside the branches and in the condition
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"f(x)=x*x;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<>3, (f(3)), (f(4)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(f(2)==5, (f(3)), (f(4)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "16.E+0") << s;
}

TEST_F(CalcTestReal, if8)
{
    //an assignment as the last statement: the value of the branch is the assigned value
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, (v=42), (v=0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "42.E+0") << s;
    //v was an if-local temp and is gone after the if
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"v;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 0, 0, 2}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, if9)
{
    //an if() inside the accumulator body of the old loop()
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=4),i=i+1,t=0,t=t+if(i<=2, (i), (2*i)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "17.E+0") << s;
}

TEST_F(CalcTestReal, if_threads)
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
                Parser<Real> p(3, Language::English);
                for (int iter = 0; iter < iterations; ++iter)
                {
                    p.Parse(LogicalId{0, 0, 1}, U"u=0;");
                    std::string s = p.Parse(LogicalId{0, 0, 2}, U"if(1<2, (u=u+5, u*2), (u=100));").ToStdString(3, 3);
                    if (s != "10.E+0")
                        ++failures[t];
                }
            });
    }
    for (std::thread& thread : threads)
        thread.join();
    for (int failure : failures)
        ASSERT_TRUE(failure == 0);
}

TEST_F(CalcTestReal, if_identifier)
{
    //'if' without a call is still a valid identifier
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if;").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestReal, if_errors)
{
    //neither branch can end with a function definition
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, (u=1,f(x)=x), (0));"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (0), (f(x)=x));"), yutovo_calculator::MathException);
    //an empty branch is a syntax error
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(1<2, (), (0));"), yutovo_calculator::SyntaxException);
    //the condition must be a comparison
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(10, (1), (0));"), yutovo_calculator::SyntaxException);
    //an unknown name in a branch
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"if(1<2, (u=u+1), (0));"), yutovo_calculator::SyntaxException);
    //a missing branch and an extra argument after the last branch
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"if(1<2, (1));"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"if(1<2, (1), (0), 5);"), yutovo_calculator::SyntaxException);
    //a variable defined by an earlier statement of the SAME script is not visible in the condition
    //(engine-wide rule: only the for/if block statements get synthesized ids, same as for()'s own condition;
    //define the variable in a separate Parse instead)
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 8}, U"v=1+1;if(v < 3, (5), (6));"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, if10)
{
    //the condition is a complex expression over user variables and builtin functions
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"b=4;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(pow(a,2)+pow(b,2) == 25, (sqrt(pow(a,2)+pow(b,2))), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(pow(a,2)+pow(b,2) == 24.5, (1), (2));").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0") << s;
    //a user function participates in the condition and in the branches
    parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"f(x)=x*x;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"if(f(a)+f(b) > 20, (f(a)+f(b)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "25.E+0") << s;
    //min/max combine both variables in the condition; the branch value is the last statement
    s = parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"if(min(a,b)*max(a,b) == 12, (a+b, a*b), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "12.E+0") << s;
    //rounding functions in the condition and in the branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 8}, U"if(floor(7.9) == 7, (round(2.5), trunc(7.9)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "7.E+0") << s;
}

TEST_F(CalcTestReal, if11)
{
    //the condition reads the loop counter through a rounding expression; the branches square the even values only
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=6,i=i+1,(u=u+if(trunc(i/2)*2 == i,(i*i),(0))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "56.E+0") << s;
    //a two-level if inside the loop body builds a piecewise function of the counter
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"s=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,i<=9,i=i+1,(s=s+if(i>=7,(700+i),(if(i>=4,(40+i),(i))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.265E+3") << s;
}

TEST_F(CalcTestReal, if12)
{
    //the branch chains several assignments through intermediate if-local variables
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(pow(2,3) == 8, (t=3, v=t*4, w=v+t, w*2), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "30.E+0") << s;
    //a function defined in the branch is used by a later expression of the same branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1==1, (f(x)=pow(x,2)+1, f(2)+f(3)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "15.E+0") << s;
    //factorial and power in the condition and in the branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(5! == 120, (5!+pow(2,5)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "152.E+0") << s;
    //the if result feeds a definition of a user variable
    parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"r=if(2>1,(7),(8));");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"r;").ToStdString(3, 3);
    ASSERT_TRUE(s == "7.E+0") << s;
}

TEST_F(CalcTestReal, if13)
{
    //three levels of nesting: the inner condition reads a variable assigned by the outer branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, (b=12, if(b>10, (b-10, b*100), (b+1, b+100))), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.2E+3") << s;
    //a function defined in a nested branch is called by a later statement of the outer branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (if(2>1, (h(x)=x*3, h(4)), (0)), h(1)+h(2)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0") << s;
    //grade classification through nested else-branches with all boundaries
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"score=95;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(score>=90,(5),(if(score>=75,(4),(if(score>=60,(3),(2))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"score=82;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"if(score>=90,(5),(if(score>=75,(4),(if(score>=60,(3),(2))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"score=75;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 8}, U"if(score>=90,(5),(if(score>=75,(4),(if(score>=60,(3),(2))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 9}, U"score=60;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 10}, U"if(score>=90,(5),(if(score>=75,(4),(if(score>=60,(3),(2))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.E+0") << s;
    parser.Parse(LogicalId{0, 0, 0, 0, 11}, U"score=59;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 12}, U"if(score>=90,(5),(if(score>=75,(4),(if(score>=60,(3),(2))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0") << s;
}

TEST_F(CalcTestReal, if14)
{
    //the condition contains if() expressions as operands of its comparison
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(if(1<2,(2),(3)) == 2, (10), (20));").ToStdString(3, 3);
    ASSERT_TRUE(s == "10.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(if(2>1,(1),(0)) + if(3>1,(1),(0)) == 2, (100), (200));").ToStdString(3, 3);
    ASSERT_TRUE(s == "100.E+0") << s;
}

TEST_F(CalcTestReal, if15)
{
    //an if-local assignment shadows the user variable for the whole branch, including nested conditions
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2>1, (u=10, if(u>5, (u*2), (u*3))), (u*4));").ToStdString(3, 3);
    ASSERT_TRUE(s == "20.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    //the false branch never sees the then-branch temps and reads the untouched user variable
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(2<1, (u=10, u), (if(u==1, (7), (8))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "7.E+0") << s;
}

TEST_F(CalcTestReal, if16)
{
    //a piecewise sum over negative and positive values with three nesting levels inside the loop
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=-3,i<=3,i=i+1,(u=u+if(i<0,(abs(i)),(if(i==0,(100),(i*i))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "120.E+0") << s;
}

TEST_F(CalcTestReal, if17)
{
    //regression: a parenthesized sum as a condition operand used to duplicate its last addition operation
    //(a Spirit alternative-attribute leak in for_condition, fixed with hold[compare]); the bug also broke
    //the same form in the for() condition, which predates if()
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if((2+1) < 4, (1), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if((5-2) < 4, (1), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if((2+1*5) < 10, (1), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    //a parenthesized sum on the right side of the condition
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(4 < (2+1), (1), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    //the bracketed condition form still wins when the whole comparison is parenthesized
    s = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"if((2+1 < 4), (1), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    //the same left-operand form inside a for() condition
    parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"u=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"for(k=1,(k+1)<=2,k=k+1,(u=u+1));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

}
