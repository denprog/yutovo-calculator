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

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestComplex, sum1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=10),k=k+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(k=1,(k<=10),k=k+1,t=0,t=t+k);").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
}

TEST_F(CalcTestComplex, sum2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=10),k=k+1,t1=0,t1=t1+k*i+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0+i*55.E+0") << s;
}

TEST_F(CalcTestComplex, prod1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=5),k=k+1,t=1,t=t*(2));").ToStdString(3, 3);
    ASSERT_TRUE(s == "64.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(k=1,(k<=3),k=k+1,t=1,t=t*(i+5));").ToStdString(3, 3);
    ASSERT_TRUE(s == "110.E+0+i*74.E+0") << s;
}

TEST_F(CalcTestComplex, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=10,k=k+1,(u=u+k));").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
}

TEST_F(CalcTestComplex, for2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=3,k=k+1,(u=u+k+2*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0+i*6.E+0") << s;
}

TEST_F(CalcTestComplex, for3)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(k=1,k<=3,k=k+1,(f(x)=2*x,u=f(k)));").ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
}

TEST_F(CalcTestComplex, for4)
{
    //the canonical nested-sum idiom; the inner sums are multiplied by the imaginary unit
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=2,k=k+1,(v=0,for(j=1,j<=2,j=j+1,(v=v+k*j)),u=u+v*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "i*9.E+0") << s;
}

TEST_F(CalcTestComplex, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (i, 2+3*i), (4, 5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0+i*3.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (i, 2+3*i), (4, 5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "i*5.E+0") << s;
}

TEST_F(CalcTestComplex, if2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1+i;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+i, u*2), (u*100));").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0+i*4.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0+i*1.E+0") << s;
}

TEST_F(CalcTestComplex, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(2+i)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "4.E+0+i*2.E+0") << s;
}

TEST_F(CalcTestComplex, if4)
{
    //a for() body selects its per-iteration value with if()
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=5,k=k+1,(u=u+if(k<>2, (k), (0))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "13.E+0") << s;
}

TEST_F(CalcTestComplex, if5)
{
    //the condition takes the module of a complex user variable; the branches compute complex values
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=3+4*i;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(abs(u) == 5, (u*u, (3+4*i)*(1+i)), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "-1.E+0+i*7.E+0") << s;
    //a parenthesized complex sum in the condition and a nested if in the branch
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"if((1+i) == 1+i, (5, if(abs(1+i) >= 1, ((1+i)*(1-i), 2*i), (0))), (0));").ToStdString(3, 3);
    ASSERT_TRUE(s == "i*2.E+0") << s;
}

TEST_F(CalcTestComplex, if6)
{
    //a piecewise complex sum inside a loop: the condition is a product expression over the counter
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=4,k=k+1,(u=u+if((k-1)*(k-3) == 0,(k+i),(if(k==2,(2*k*i),(k*k))))));").ToStdString(3, 3);
    ASSERT_TRUE(s == "20.E+0+i*6.E+0") << s;
}

}
