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

TEST(UtilsTest, ElementIdFromString1)
{
    auto id = ElementIdFromString("1,2,3");
    ASSERT_EQ(id.size(), 3u);
    ASSERT_EQ(id[0], 1);
    ASSERT_EQ(id[1], 2);
    ASSERT_EQ(id[2], 3);
}

TEST(UtilsTest, ElementIdFromString2)
{
    auto id = ElementIdFromString("");
    ASSERT_TRUE(id.empty());
}

TEST(UtilsTest, ElementIdFromString3)
{
    EXPECT_THROW(ElementIdFromString("1,a,3"), yutovo_calculator::ParserException);
}

TEST(UtilsTest, ElementIdFromString4)
{
    EXPECT_THROW(ElementIdFromString("1,2,abc"), yutovo_calculator::ParserException);
}

TEST(UtilsTest, LogicalIdFromString5)
{
    auto id = LogicalIdFromString("0,1,2,3");
    ASSERT_EQ(id.size(), 4u);
    ASSERT_EQ(id[0], 0);
    ASSERT_EQ(id[1], 1);
    ASSERT_EQ(id[2], 2);
    ASSERT_EQ(id[3], 3);
}

TEST(UtilsTest, LogicalIdFromString6)
{
    auto id = LogicalIdFromString("");
    ASSERT_TRUE(id.empty());
}

TEST(UtilsTest, LogicalIdFromString7)
{
    EXPECT_THROW(LogicalIdFromString("0,1,abc"), yutovo_calculator::ParserException);
}

}
