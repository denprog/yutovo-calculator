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

TEST_F(CalcTestArrayReal, units1)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[5м,3*(м/с)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0(м),3.E+0((м)/(с))]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, units_german)
{
    parser.SetLocale(Language::German);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[5m,3*(m/s)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0(m),3.E+0((m)/(s))]") << r.ToStdString(3, 3);

    //angle identifiers of the array parser under the German locale
    r = parser.Parse(LogicalId{0, 0, 2}, U"[sin(90deg)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, units_french)
{
    parser.SetLocale(Language::French);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[5m,3*(m/s)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0(m),3.E+0((m)/(s))]") << r.ToStdString(3, 3);

    //angle identifiers of the array parser under the French locale
    r = parser.Parse(LogicalId{0, 0, 2}, U"[sin(90deg)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0]") << r.ToStdString(3, 3);
}

TEST_F(CalcTestArrayReal, units_italian)
{
    parser.SetLocale(Language::Italian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"[5m,3*(m/s)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[5.E+0(m),3.E+0((m)/(s))]") << r.ToStdString(3, 3);

    //angle identifiers of the array parser under the Italian locale
    r = parser.Parse(LogicalId{0, 0, 2}, U"[sin(90deg)];");
    ASSERT_TRUE(r.ToStdString(3, 3) == "[1.E+0]") << r.ToStdString(3, 3);
}

}
