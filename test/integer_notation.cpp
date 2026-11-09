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

TEST_F(CalcTestInteger, notation1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123;");
    ASSERT_TRUE(res.ToString(2) == U"1111011") << res.ToStdString(2);
    ASSERT_TRUE(res.ToString(8) == U"173") << res.ToStdString(8);
    ASSERT_TRUE(res.ToString(10) == U"123") << res.ToStdString(10);
    ASSERT_TRUE(res.ToString(16) == U"7b") << res.ToStdString(16);
}

TEST_F(CalcTestInteger, notation2)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"bin[1010];");
    ASSERT_TRUE(res.ToString(2) == U"1010") << res.ToStdString(2);
    ASSERT_TRUE(res.ToString(10) == U"10") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"bin[123];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"bin[101+111];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation3)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"oct[2345];");
    ASSERT_TRUE(res.ToString(10) == U"1253") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"oct[985];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"oct[10/345];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation4)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dec[4569];");
    ASSERT_TRUE(res.ToString(10) == U"4569") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dec[3ef];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dec[101+35];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation5)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"hex[56fe];");
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"hex[dt45];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"hex[101+f1];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation6)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1010;", Notation::Binary);
    ASSERT_TRUE(res.ToString(10) == U"10") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123;", Notation::Binary), yutovo_calculator::MathException);
}

TEST_F(CalcTestInteger, notation7)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2345;", Notation::Octal);
    ASSERT_TRUE(res.ToString(10) == U"1253") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"128;", Notation::Octal), yutovo_calculator::MathException);
}

TEST_F(CalcTestInteger, notation8)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1010;", Notation::Decimal);
    ASSERT_TRUE(res.ToString(10) == U"1010") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123f;", Notation::Decimal), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation9)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"56fe;", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"56FE;", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, notation10)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"56fe+dec[10];", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22280") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, notation11)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"hex[da]+1011;", Notation::Binary);
    ASSERT_TRUE(res.ToString(10) == U"229") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, notation12)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"hex[da]+1011;", Notation::Decimal);
    ASSERT_TRUE(res.ToString(10) == U"1229") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, notation13)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"da;", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"218") << res.ToStdString(10);
}

}
