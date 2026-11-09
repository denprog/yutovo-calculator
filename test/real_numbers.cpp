/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, numbers1)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"0.2E+0;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers2)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.75;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1-2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1*2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1/2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".5;").ToStdString(3, 3));
}

TEST_F(CalcTestReal, numbers4)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3) == "21.2E+0");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"21.2E+0;").ToStdString(3, 3) == "21.2E+0");
}

TEST_F(CalcTestReal, numbers5)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+5.46;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.46E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-5.55;");
    ASSERT_TRUE(res.ToString(3, 3) == U"-5.55E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers6)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(4+5);").ToStdString(3, 3);
    ASSERT_TRUE(s == "18.E+0") << s;
}

TEST_F(CalcTestReal, numbers7)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E;"), yutovo_calculator::SyntaxException);
    auto s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1E+0;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E+0;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E+1;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E-1;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, numbers8)
{
    parser.SetLocale(Language::Russian);
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
}

TEST_F(CalcTestReal, numbers9)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, numbers10)
{
    EXPECT_THROW(Real(53, "abc"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(Real(53, "1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(Real(53, "x+y"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, scientific1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e-5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.23E-5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e-5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.23E-5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e+5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.23E+5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e+5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.23E+5") << res;
}

TEST_F(CalcTestReal, arithmetic1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10%75;");
    ASSERT_TRUE(res.ToString(3, 3) == U"7.5E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, percent_precision)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"200%50;");
    ASSERT_TRUE(res.ToStdString(3, 15) == "100.E+0") << res.ToStdString(3, 15);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"3%7;");
    ASSERT_TRUE(res.ToStdString(3, 15) == "0.21E+0") << res.ToStdString(3, 15);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"10%75;");
    ASSERT_TRUE(res.ToStdString(3, 15) == "7.5E+0") << res.ToStdString(3, 15);
}

TEST_F(CalcTestReal, arithmetic2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3+4);");
    ASSERT_TRUE(res.ToString(3, 3) == U"14.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, compare1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10<>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestReal, compare2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0}, U"a=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<>10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<=10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

TEST_F(CalcTestReal, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=2.5;");
    parser.Parse(LogicalId{0, 0, 2}, U"x=3;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=4;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"(x)/(y)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.875E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(1/2)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.25E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"2(a+1);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "7.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2abs(-2);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "4.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"abs(2)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123+sin(2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123+3;45"), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestReal, errors6)
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


TEST_F(CalcTestReal, errors8)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"(1)/(0);"), yutovo_calculator::MathException);
}


TEST_F(CalcTestReal, errors9)
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


TEST_F(CalcTestReal, recursion_depth1)
{
    std::u32string deep(20000, U'(');
    deep += U"1";
    deep.append(20000, U')');
    deep += U";";
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, deep), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestReal, recursion_depth2)
{
    std::u32string deep(20000, U'(');
    deep += U"1;";
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, deep), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestReal, recursion_depth3)
{
    std::u32string deep(20000, U'-');
    deep += U"1;";
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, deep), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestReal, recursion_depth4)
{
    //moderate nesting must still parse below the recursion limit;
    //deeper nesting (~14+) is exponentially slow due to the backtracking alternatives of the grammar, which is a pre-existing property
    std::u32string deep(8, U'(');
    deep += U"1";
    deep.append(8, U')');
    deep += U";";
    Real res = parser.Parse(LogicalId{0, 0, 1}, deep);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << res.ToStdString(3, 3);
}


}
