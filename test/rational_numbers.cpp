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
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, arithmetic1)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10%75;");
    ASSERT_TRUE(res.ToString() == U"15/2") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"200%50;");
    ASSERT_TRUE(res.ToString() == U"100") << res.ToStdString();
}

TEST_F(CalcTestRational, arithmetic2)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(1)/(3);");
    ASSERT_TRUE(res.ToString() == U"-1/3") << res.ToStdString();
}

TEST_F(CalcTestRational, rationals1)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/4;") == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1/2;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/4;").ToStdString();
}

TEST_F(CalcTestRational, rationals2)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(1/4);") == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"9/4;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(1/4);").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*22(3/5);") == parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"339/5;")) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*22(3/5);").ToStdString();
}

TEST_F(CalcTestRational, rationals3)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+3/4;");
    ASSERT_TRUE(res.ToStdString() == "3/4") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-3/4;");
    ASSERT_TRUE(res.ToStdString() == "-3/4") << res.ToStdString();
}

TEST_F(CalcTestRational, rationals4)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(5/(4*2));");
    ASSERT_TRUE(res.ToStdString() == "5/4") << res.ToStdString();
}

TEST_F(CalcTestRational, rationals5)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.5;");
    ASSERT_TRUE(res.ToStdString() == "5/2") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"22.12;");
    ASSERT_TRUE(res.ToStdString() == "553/25") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".12;");
    ASSERT_TRUE(res.ToStdString() == "3/25") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.12;");
    ASSERT_TRUE(res.ToStdString() == "3/25") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.;");
    ASSERT_TRUE(res.ToStdString() == "12") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.0;");
    ASSERT_TRUE(res.ToStdString() == "12") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"54.990;");
    ASSERT_TRUE(res.ToStdString() == "5499/100") << res.ToStdString();
}

TEST_F(CalcTestRational, rationals9)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestRational, rationals10)
{
    Rational r;
    EXPECT_THROW(r = U"abc", yutovo_calculator::SyntaxException);
    EXPECT_THROW(r = U"x/y", yutovo_calculator::SyntaxException);
    EXPECT_THROW(r = U"1/2/3", yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestRational, rationals6)
{
    parser.SetLocale(Language::Russian);
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.5;");
    ASSERT_TRUE(res.ToStdString() == "5/2") << res.ToStdString();
}

TEST_F(CalcTestRational, rationals7)
{
    Rational res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;");
    ASSERT_TRUE(res.ToStdString() == "-23/10") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-0.12;");
    ASSERT_TRUE(res.ToStdString() == "-3/25") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-.12;");
    ASSERT_TRUE(res.ToStdString() == "-3/25") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.5;");
    ASSERT_TRUE(res.ToStdString() == "-5/2") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-12.0;");
    ASSERT_TRUE(res.ToStdString() == "-12") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-54.990;");
    ASSERT_TRUE(res.ToStdString() == "-5499/100") << res.ToStdString();
}

TEST_F(CalcTestRational, proper1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(5/4);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "1");
    ASSERT_TRUE(n.ToStdString() == "1");
    ASSERT_TRUE(d.ToStdString() == "4");
}

TEST_F(CalcTestRational, proper2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(3/4);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0");
    ASSERT_TRUE(n.ToStdString() == "3");
    ASSERT_TRUE(d.ToStdString() == "4");
}

TEST_F(CalcTestRational, proper3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(2/3);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "-2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(2/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper5)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"((-2)/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper6)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"((-7)/3);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "-2") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "1") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper7)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(7/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "-2") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "1") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper8)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "-2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, compare1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<(2/3));").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(2/3==2/3);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(4/3<>4/3);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
}

TEST_F(CalcTestRational, compare2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0}, U"a=5/2;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a==5/2);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<>10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
}

TEST_F(CalcTestRational, pow_int)
{
    Rational base(2);
    Rational res = pow(base, 0);
    ASSERT_TRUE(res.ToStdString() == "1") << res.ToStdString();
    res = pow(base, 1);
    ASSERT_TRUE(res.ToStdString() == "2") << res.ToStdString();
    res = pow(base, 2);
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
    res = pow(base, 3);
    ASSERT_TRUE(res.ToStdString() == "8") << res.ToStdString();
    res = pow(base, 10);
    ASSERT_TRUE(res.ToStdString() == "1024") << res.ToStdString();
    res = pow(base, -1);
    ASSERT_TRUE(res.ToStdString() == "1/2") << res.ToStdString();
    res = pow(base, -2);
    ASSERT_TRUE(res.ToStdString() == "1/4") << res.ToStdString();
    res = pow(base, -3);
    ASSERT_TRUE(res.ToStdString() == "1/8") << res.ToStdString();
}

TEST_F(CalcTestRational, implicit_mul_div1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5/2;");
    parser.Parse(LogicalId{0, 0, 2}, U"x=3;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=4;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2a;");
    ASSERT_TRUE(r.ToString() == U"5") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 5}, U"(x)/(y)a;");
    ASSERT_TRUE(r.ToString() == U"15/8") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 6}, U"(1/2)a;");
    ASSERT_TRUE(r.ToString() == U"5/4") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 7}, U"2(a+1);");
    ASSERT_TRUE(r.ToString() == U"7") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 8}, U"2abs(-2);");
    ASSERT_TRUE(r.ToString() == U"4") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 9}, U"abs(2)a;");
    ASSERT_TRUE(r.ToString() == U"5") << r.ToStdString();
}

TEST_F(CalcTestRational, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"123+(2/3)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"123+3;45"), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestRational, errors2)
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


TEST_F(CalcTestRational, errors5)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"(1)/(0);"), yutovo_calculator::MathException);
}


TEST_F(CalcTestRational, errors9)
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
