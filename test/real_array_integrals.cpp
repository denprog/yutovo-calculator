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

TEST_F(CalcTestArrayReal, definite_integral1)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"definite_integral(0,1,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "[0.5E+0]") << res.ToStdString(10, 10);
}

TEST_F(CalcTestArrayReal, definite_integral2)
{
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"definite_integral(-1,4,sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "[1.1939459267E+0]") << res.ToStdString(10, 10);
}

}
