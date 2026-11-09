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

TEST_F(CalcTestComplex, definite_integral1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.5E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(-1,4,sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.1939459267E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral3)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,pi,sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "2.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral4)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,2,pow(x,2),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "2.6666666667E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral5)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,pow(x,3),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.25E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral6)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,pi,cos(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.5022025553E-16") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral7)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,10,pow(x,2),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "333.33333333E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral8)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,exp(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.7182818285E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestComplex, definite_integral9)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,(1)/(x+i),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.3465735903E+0+i*-0.7853981634E+0") << res.ToStdString(10, 10);

    parser.SetLocale(Language::Russian);
    Complex res_russian = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,(1)/(x+j),x);", 10);
    ASSERT_TRUE(res_russian.ToStdString(10, 10) == "0.3465735903E+0+i*-0.7853981634E+0") << res_russian.ToStdString(10, 10);
}

}
