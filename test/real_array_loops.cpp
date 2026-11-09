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

TEST_F(CalcTestArrayReal, sum1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"var=[1,2,3];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"loop(i=0,(i<=(size(var)-1)),i=i+1,t=0,t=t+var{i});");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, sum2)
{
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"loop(i=1,(i<=3),i=i+1,t=0,t=t+[1,2,3]);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[3.E+0,6.E+0,9.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, sum3)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"d=[1.23,pow(2,3),-55,4];");
    r = parser.Parse(LogicalId{0, 0, 2}, U"loop(n=0,(n<=size(d)-1),n=n+1,n_=0,n_=n_+d{n});");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[-41.77E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, for1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"var=[1,2,3];", 3);
    //t=0 inside the block resets t on every iteration, so u is twice the last element
    r = parser.Parse(LogicalId{0, 0, 2}, U"for(i=0,i<=size(var)-1,i=i+1,(t=0,t=t+var{i},u=t*2));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, for2)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"u=[0,0];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"for(i=1,i<=3,i=i+1,(u=u+[i,2*i]));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0,12.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, for3)
{
    //a function defined inside the block with a subscript access in the call
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"var=[1,2,3];", 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"for(i=0,i<=2,i=i+1,(f(x)=2*x,u=f(var{i})));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[6.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, if1)
{
    //a branch may return an array; the value is the last statement of the executed branch
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"if(2>1, ([1,2], [3,4]), ([5,6]));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[3.E+0,4.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"if(2<1, ([1,2], [3,4]), ([5,6]));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0,6.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, if2)
{
    //an assignment in the branch accumulates arrays element-wise; the outer user variable keeps its old value
    parser.Parse(LogicalId{0, 0, 1}, U"u=[0,0];", 3);
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"if(1<2, (u=u+[1,2], u+[10,20]), (u));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[11.E+0,22.E+0]") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 3}, U"u;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[0.E+0,0.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, if3)
{
    //a piecewise array construction inside a loop: the branches return whole arrays
    parser.Parse(LogicalId{0, 0, 1}, U"u=[0,0];", 3);
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"for(i=1,i<=2,i=i+1,(u=u+if(i==1,([10,20]),([1,2]))));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[11.E+0,22.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, if4)
{
    //the condition compares subscript accesses; a three-level nesting selects the result
    parser.Parse(LogicalId{0, 0, 1}, U"t=[3,-1,2];", 3);
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"if(t{0} > t{2}, (if(t{0} > 3*t{2}, (t{0}*10), (t{0}+t{2}))), (t{1}));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0]") << r.ToStdString(3, 3);
    //the else-branch of the outer if runs when the elements compare the other way
    r = parser.Parse(LogicalId{0, 0, 3}, U"if(t{2} > t{0}, (100), (if(t{1} < 0, (abs(t{1})), (t{1}))));", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0]") << r.ToStdString(3, 3);
}

}
