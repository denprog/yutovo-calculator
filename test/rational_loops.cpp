/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, sum1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString();
    ASSERT_TRUE(s == "11") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString();
    ASSERT_TRUE(s == "55") << s;
}

TEST_F(CalcTestRational, sum2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=1,(j<=10),j=j+1,t2=0,t2=t2+i/j));").ToStdString();
    ASSERT_TRUE(s == "81191/504") << s;
}

TEST_F(CalcTestRational, prod1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2/3);").ToStdString();
    ASSERT_TRUE(s == "2048/177147") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i/5);").ToStdString();
    ASSERT_TRUE(s == "145152/390625") << s;
}

TEST_F(CalcTestRational, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+1/i));").ToStdString();
    ASSERT_TRUE(s == "25/12") << s;
}

TEST_F(CalcTestRational, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u*1/i));").ToStdString();
    ASSERT_TRUE(s == "1/24") << s;
}

TEST_F(CalcTestRational, for3)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(f(x)=2*x,u=f(i)));").ToStdString();
    ASSERT_TRUE(s == "6") << s;
}

TEST_F(CalcTestRational, for4)
{
    //the canonical nested-sum idiom with exact fraction accumulation
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(v=0,for(j=1,j<=4,j=j+1,(v=v+i/j)),u=u+v));").ToStdString();
    ASSERT_TRUE(s == "25/2") << s;
}

TEST_F(CalcTestRational, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (1/2, 2/3), (3/4, 4/5));").ToStdString();
    ASSERT_TRUE(s == "2/3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (1/2, 2/3), (3/4, 4/5));").ToStdString();
    ASSERT_TRUE(s == "4/5") << s;
}

TEST_F(CalcTestRational, if2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1/2;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+1/3, u*2), (u*100));").ToStdString();
    ASSERT_TRUE(s == "5/3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString();
    ASSERT_TRUE(s == "1/2") << s;
}

TEST_F(CalcTestRational, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(1/2)), (0));").ToStdString();
    ASSERT_TRUE(s == "1") << s;
}

TEST_F(CalcTestRational, if4)
{
    //a for() body selects its per-iteration value with if(); the exact harmonic sum skips i=2
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<>2, (1/i), (0))));").ToStdString();
    ASSERT_TRUE(s == "19/12") << s;
}

TEST_F(CalcTestRational, if5)
{
    //the condition is a fraction expression over a user variable; nested fraction branches
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=1/2;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(a+a+1/3 == 4/3, (1/2+1/3, 5/6), (0));").ToStdString();
    ASSERT_TRUE(s == "5/6") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(1/3 > 1/2, (1), (if(2/3 > 1/2, (3/4), (4/5))));").ToStdString();
    ASSERT_TRUE(s == "3/4") << s;
    //a user function computes exact fractions in the condition and the branches
    parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"f(x)=x/2;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"if(f(3) > 1, (f(3), 3/2), (1/3));").ToStdString();
    ASSERT_TRUE(s == "3/2") << s;
}

TEST_F(CalcTestRational, if6)
{
    //a piecewise fraction-valued sum inside a loop with two nesting levels
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<=2,(1/i),(if(i==3,(1/2),(1/4))))));").ToStdString();
    ASSERT_TRUE(s == "9/4") << s;
}

}
