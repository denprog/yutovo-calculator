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

TEST_F(CalcTestSymbolicComplex, for1)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=3,k=k+1,(u=u+k));").ToStdString(10);
    ASSERT_TRUE(s == "6") << s;
    //the counter name must not shadow the imaginary unit
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"w=0;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"for(k=1,k<=3,k=k+1,(w=w+k*i));").ToStdString(10);
    ASSERT_TRUE(s == "6.*i") << s;
}

TEST_F(CalcTestSymbolicComplex, for_errors)
{
    //a condition on a complex value cannot be decided
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"for(k=1,k<=i,k=k+1,(u=1));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestSymbolicComplex, for2)
{
    //the canonical nested-sum idiom; the inner sums are multiplied by the imaginary unit
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=0;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"for(k=1,k<=2,k=k+1,(v=0,for(j=1,j<=2,j=j+1,(v=v+k*j)),u=u+v*i));").ToStdString(10);
    ASSERT_TRUE(s == "9.*i") << s;
}

TEST_F(CalcTestSymbolicComplex, if1)
{
    //the value of the expression is the value of the last statement of the executed branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2>1, (i, 2+3*i), (4, 5*i));").ToStdString(10);
    ASSERT_TRUE(s == "2.+3.*i") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(2<1, (i, 2+3*i), (4, 5*i));").ToStdString(10);
    ASSERT_TRUE(s == "5.*i") << s;
}

TEST_F(CalcTestSymbolicComplex, if2)
{
    //an assignment in the branch writes to an if-local variable; the outer user variable keeps its value
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"u=1+i;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1<2, (u=u+i, u*2), (u*100));").ToStdString(10);
    ASSERT_TRUE(s == "2.+4.*i") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"u;").ToStdString(10);
    ASSERT_TRUE(s == "1.+i") << s;
}

TEST_F(CalcTestSymbolicComplex, if3)
{
    //a function defined inside the branch is used by the following statements of the branch
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(3<=3, (f(x)=2*x, f(2+i)), (0));").ToStdString(10);
    ASSERT_TRUE(s == "4.+2.*i") << s;
    //a condition on a complex value cannot be decided
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(i<2, (1), (0));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestSymbolicComplex, if4)
{
    //the branches compute complex values from the imaginary unit
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"if(2*2 == 4, (i*i, -1+2*i), (0));").ToStdString(10);
    ASSERT_TRUE(s == "-1.+2.*i") << s;
    //a nested if in the else-branch multiplies complex conjugates
    s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"if(1 > 2, (0), (if(3 <= 3, ((1+i)*(1-i), 5*i), (0))));").ToStdString(10);
    ASSERT_TRUE(s == "5.*i") << s;
    //a user function in the condition and complex branches
    parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"f(x)=x*x;");
    s = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"if(f(1+i) == 2*i, (f(1+i), 4*i), (0));").ToStdString(10);
    ASSERT_TRUE(s == "4.*i") << s;
}

}
