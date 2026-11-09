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

TEST_F(CalcTestSymbolicRational, matrix1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"det([[1/2,1/3],[1/4,1/5]]);");
    ASSERT_TRUE(res.ToStdString(10) == "1/60") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"det([[x,y],[z,w]]);");
    ASSERT_TRUE(res.ToStdString(10) == "x*w-y*z") << res.ToStdString(10);
}

}
