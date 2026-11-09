/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, sum1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString();
    ASSERT_TRUE(s == "11") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString();
    ASSERT_TRUE(s == "55") << s;
}

TEST_F(CalcTestInteger, sum2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=0,(j<=20),j=j+1,t2=0,t2=t2+i+j));").ToStdString();
    ASSERT_TRUE(s == "3465") << s;
}

TEST_F(CalcTestInteger, prod1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2);").ToStdString();
    ASSERT_TRUE(s == "2048") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i);").ToStdString();
    ASSERT_TRUE(s == "3628800") << s;
}

TEST_F(CalcTestInteger, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=10,i=i+1,(u=u+i));").ToStdString();
    ASSERT_TRUE(s == "55") << s;
}

TEST_F(CalcTestInteger, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u*i));").ToStdString();
    ASSERT_TRUE(s == "120") << s;
}

TEST_F(CalcTestInteger, for3)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(f(x)=2*x,u=f(i)));").ToStdString();
    ASSERT_TRUE(s == "6") << s;
}

TEST_F(CalcTestInteger, for4)
{
    //the canonical nested-sum idiom: the outer block resets v once per iteration, the inner block writes through
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(v=0,for(j=1,j<=4,j=j+1,(v=v+i*j)),u=u+v));").ToStdString();
    ASSERT_TRUE(s == "60") << s;
}

TEST_F(CalcTestInteger, for5)
{
    //a for() inside the accumulator body of the old loop(); the user variable v re-seeds every run of the inner block
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"v=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"loop(i=1,(i<=3),i=i+1,t=0,t=t+for(j=1,j<=2,j=j+1,(v=v+j)));").ToStdString();
    ASSERT_TRUE(s == "9") << s;
}

TEST_F(CalcTestInteger, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (10, 20), (30, 40));").ToStdString();
    ASSERT_TRUE(s == "20") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (10, 20), (30, 40));").ToStdString();
    ASSERT_TRUE(s == "40") << s;
}

TEST_F(CalcTestInteger, if2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+1, u*10), (u*100));").ToStdString();
    ASSERT_TRUE(s == "60") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString();
    ASSERT_TRUE(s == "5") << s;
}

TEST_F(CalcTestInteger, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(4)), (0));").ToStdString();
    ASSERT_TRUE(s == "8") << s;
}

TEST_F(CalcTestInteger, if4)
{
    //a for() body selects its per-iteration value with if()
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u+if(i<>2, (1), (0))));").ToStdString();
    ASSERT_TRUE(s == "4") << s;
}

TEST_F(CalcTestInteger, if5)
{
    //the condition is an expression over a user variable; the branch chains if-local assignments
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=3;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(a*a+4 == 13, (t=a*2, v=t+1, v*3), (0));").ToStdString();
    ASSERT_TRUE(s == "21") << s;
    //a user function and a factorial in the condition and the branch
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"f(x)=x*x;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(f(3) > 5!, (0), (f(3)+4!));").ToStdString();
    ASSERT_TRUE(s == "33") << s;
}

TEST_F(CalcTestInteger, if6)
{
    //a two-level if inside a loop body builds a piecewise function of the counter
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=9,i=i+1,(u=u+if(i>=7,(700+i),(if(i>=4,(40+i),(i))))));").ToStdString();
    ASSERT_TRUE(s == "2265") << s;
    //three levels: the innermost condition reads a variable assigned by the outer branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(5>3, (b=12, if(b>10, (if(b>11, (b*100), (b*10))), (0))), (0));").ToStdString();
    ASSERT_TRUE(s == "1200") << s;
}

}
