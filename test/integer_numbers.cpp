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

TEST_F(CalcTestInteger, integers1)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+1;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;"));
}

TEST_F(CalcTestInteger, integers2)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(2/3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(2/3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-1)/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-1)/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-1)/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-1)/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-2)/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-2)/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-2)/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-2)/(-3);");
}

TEST_F(CalcTestInteger, integers3)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(4/3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(4/3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-4/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-4/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-4)/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-4)/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-4)/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-4)/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-7/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-7/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-7/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-7/(-3);");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-7)/3;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-7)/3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-7)/(-3);") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(-7)/(-3);");
}

TEST_F(CalcTestInteger, division_int_operands)
{
    // Non-exact division must use truncated division, not mpz_divexact.
    Integer a(7);
    Integer b = a / 3;
    ASSERT_TRUE(b == 2) << b.ToStdString(10);

    Integer c = 7 / Integer(3);
    ASSERT_TRUE(c == 2) << c.ToStdString(10);

    Integer d(7);
    d /= Integer(3);
    ASSERT_TRUE(d == 2) << d.ToStdString(10);

    // Exact division must still work.
    Integer e(6);
    Integer f = e / 3;
    ASSERT_TRUE(f == 2) << f.ToStdString(10);

    Integer g = 6 / Integer(3);
    ASSERT_TRUE(g == 2) << g.ToStdString(10);

    // Negative operands.
    ASSERT_TRUE(Integer(-7) / 3 == -2) << (Integer(-7) / 3).ToStdString(10);
    ASSERT_TRUE(Integer(7) / -3 == -2) << (Integer(7) / -3).ToStdString(10);
    ASSERT_TRUE(Integer(-7) / -3 == 2) << (Integer(-7) / -3).ToStdString(10);
    ASSERT_TRUE(-7 / Integer(3) == -2) << (-7 / Integer(3)).ToStdString(10);
    ASSERT_TRUE(7 / Integer(-3) == -2) << (7 / Integer(-3)).ToStdString(10);
    ASSERT_TRUE(-7 / Integer(-3) == 2) << (-7 / Integer(-3)).ToStdString(10);

    // Dividend smaller than divisor.
    ASSERT_TRUE(Integer(2) / 5 == 0) << (Integer(2) / 5).ToStdString(10);
    ASSERT_TRUE(2 / Integer(5) == 0) << (2 / Integer(5)).ToStdString(10);
    ASSERT_TRUE(Integer(-2) / 5 == 0) << (Integer(-2) / 5).ToStdString(10);
    ASSERT_TRUE(-2 / Integer(5) == 0) << (-2 / Integer(5)).ToStdString(10);

    // Division by one.
    ASSERT_TRUE(Integer(123456) / 1 == 123456) << (Integer(123456) / 1).ToStdString(10);
    ASSERT_TRUE(123456 / Integer(1) == 123456) << (123456 / Integer(1)).ToStdString(10);

    // Division by zero throws.
    ASSERT_THROW(Integer(5) / 0, MathException);
    ASSERT_THROW(5 / Integer(0), MathException);
}

TEST_F(CalcTestInteger, integers4)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+5;");
    ASSERT_TRUE(res.ToString(10) == U"5") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-5;");
    ASSERT_TRUE(res.ToString(10) == U"-5") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, integers5)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5(4*2);");
    ASSERT_TRUE(res.ToString(10) == U"40") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, integers6)
{
    parser.SetLocale(Language::German);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+1;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;"));
}

TEST_F(CalcTestInteger, integers7)
{
    parser.SetLocale(Language::French);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+1;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;"));
}

TEST_F(CalcTestInteger, integers8)
{
    parser.SetLocale(Language::Italian);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+1;") == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;"));
}

TEST_F(CalcTestInteger, logical1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬5;");
    ASSERT_TRUE(res.ToString(10) == U"-6") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬0;");
    ASSERT_TRUE(res.ToString(10) == U"-1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬-5;");
    ASSERT_TRUE(res.ToString(10) == U"4") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical2)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10∧12;");
    ASSERT_TRUE(res.ToString(10) == U"8") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical3)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10∨12;");
    ASSERT_TRUE(res.ToString(10) == U"14") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical4)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10⊕12;");
    ASSERT_TRUE(res.ToString(10) == U"6") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical5)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬¬7;");
    ASSERT_TRUE(res.ToString(10) == U"7") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬-1;");
    ASSERT_TRUE(res.ToString(10) == U"0") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬255;");
    ASSERT_TRUE(res.ToString(10) == U"-256") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬(10∧12);");
    ASSERT_TRUE(res.ToString(10) == U"-9") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"¬10∨¬12;");
    ASSERT_TRUE(res.ToString(10) == U"-9") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, power1)
{
    Integer res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(2,3);");
    ASSERT_TRUE(res.ToString(10) == U"8") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, compare1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10==10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10<>10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
}

TEST_F(CalcTestInteger, compare2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0}, U"a=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a==10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<>10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
}

TEST_F(CalcTestInteger, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"2a;");
    ASSERT_TRUE(r.ToStdString(10) == "10") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 3}, U"2(a+1);");
    ASSERT_TRUE(r.ToStdString(10) == "12") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 4}, U"2abs(-3);");
    ASSERT_TRUE(r.ToStdString(10) == "6") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 5}, U"abs(5)a;");
    ASSERT_TRUE(r.ToStdString(10) == "25") << r.ToStdString(10);
}

TEST_F(CalcTestInteger, error1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"123+(24/2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"123+3;45"), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestInteger, errors2)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U";");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::ExpressionExpected && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


TEST_F(CalcTestInteger, errors5)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"(1)/(0);"), yutovo_calculator::MathException);
}


TEST_F(CalcTestInteger, errors6)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"2**5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::SyntaxError && ex.pos == 2 && ex.size == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}


}
