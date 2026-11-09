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

TEST_F(CalcTestSymbolicRational, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=4,i=i+1,(u=u+1/i));").ToStdString(0);
    ASSERT_TRUE(s == "25/12") << s;
}

TEST_F(CalcTestSymbolicRational, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=5,i=i+1,(u=u*i));").ToStdString(0);
    ASSERT_TRUE(s == "120") << s;
}

TEST_F(CalcTestSymbolicRational, for3)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(i=1,i<=3,i=i+1,(f(x)=2*x,u=f(i)));").ToStdString(0);
    ASSERT_TRUE(s == "6") << s;
}

TEST_F(CalcTestSymbolicRational, for4)
{
    //the canonical nested-sum idiom with exact fraction accumulation
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(i=1,i<=3,i=i+1,(v=0,for(j=1,j<=4,j=j+1,(v=v+i/j)),u=u+v));").ToStdString(0);
    ASSERT_TRUE(s == "25/2") << s;
}

TEST_F(CalcTestSymbolicRational, if1)
{
    //the value of the expression is the value of the last statement of the executed branch; fractions stay exact
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (1/2, 2/3), (3/4, 4/5));").ToStdString(0);
    ASSERT_TRUE(s == "2/3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (1/2, 2/3), (3/4, 4/5));").ToStdString(0);
    ASSERT_TRUE(s == "4/5") << s;
}

TEST_F(CalcTestSymbolicRational, if2)
{
    //the branches keep symbolic results exact
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1<2, (x+x, 3*x), (0));").ToStdString(0);
    ASSERT_TRUE(s == "3*x") << s;
    //an assignment in the branch writes to an if-local variable; the outer user variable keeps its value
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"u=1/2;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if(1<2, (u=u+1/3, u*2), (u*100));").ToStdString(0);
    ASSERT_TRUE(s == "5/3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"u;").ToStdString(0);
    ASSERT_TRUE(s == "1/2") << s;
}

TEST_F(CalcTestSymbolicRational, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(1/2)), (0));").ToStdString(0);
    ASSERT_TRUE(s == "1") << s;
    //a for() body selects its per-iteration value with if(); the harmonic sum skips i=2
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"u=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<>2, (1/i), (0))));").ToStdString(0);
    ASSERT_TRUE(s == "19/12") << s;
}

TEST_F(CalcTestSymbolicRational, if4)
{
    //the condition compares exact fractions; the branches keep them exact
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(1/2 > 1/3, (1/6+1/6, 1/3), (0));").ToStdString(0);
    ASSERT_TRUE(s == "1/3") << s;
    //a nested if in the else-branch decides between exact fractions
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1/3 > 1/2, (0), (if(1/4 > 1/5, (1/20), (1))));").ToStdString(0);
    ASSERT_TRUE(s == "1/20") << s;
    //a piecewise exact-fraction sum inside a loop
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(i=1,i<=4,i=i+1,(u=u+if(i<=2,(1/i),(1/2))));").ToStdString(0);
    ASSERT_TRUE(s == "5/2") << s;
}

}
