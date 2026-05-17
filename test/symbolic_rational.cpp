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

TEST_F(CalcTestSymbolicRational, numbers1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1/2;");
    ASSERT_TRUE(res.ToStdString(0) == "1/2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890;");
    ASSERT_TRUE(res.ToStdString(0) == "12345678901234567890") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890/9876543210987654321;");
    ASSERT_TRUE(res.ToStdString(0) == "137174210/109739369") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3;");
    ASSERT_TRUE(res.ToStdString(0) == "23/10") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;");
    ASSERT_TRUE(res.ToStdString(0) == "-23/10") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.12;");
    ASSERT_TRUE(res.ToStdString(0) == "3/25") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".12;");
    ASSERT_TRUE(res.ToStdString(0) == "3/25") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.0;");
    ASSERT_TRUE(res.ToStdString(0) == "12") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"54.990;");
    ASSERT_TRUE(res.ToStdString(0) == "5499/100") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, numbers5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3*x;");
    ASSERT_TRUE(res.ToStdString(0) == "(23/10)*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, eval1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    ASSERT_TRUE(res.ToStdString(0) == "x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, addition1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 1;");
    ASSERT_TRUE(res.ToStdString(0) == "1+x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subtraction1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x - 1;");
    ASSERT_TRUE(res.ToStdString(0) == "-1+x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, multiplication1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x * 2;");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, division1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x / 3;");
    ASSERT_TRUE(res.ToStdString(0) == "(1/3)*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, mixed_arithmetic1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 2*x;");
    ASSERT_TRUE(res.ToStdString(0) == "3*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, mixed_arithmetic2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*x^2 + 3*x - 5;");
    ASSERT_TRUE(res.ToStdString(0) == "-5+3*x+2*pow(x,2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, power1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^2;");
    ASSERT_TRUE(res.ToStdString(0) == "pow(x,2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, power2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^10;");
    ASSERT_TRUE(res.ToStdString(0) == "pow(x,10)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, diff1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^2, x);");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, expand1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)^2);");
    ASSERT_TRUE(res.ToStdString(0) == "1+2*x+pow(x,2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subs1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x^2, x, 5);");
    ASSERT_TRUE(res.ToStdString(0) == "25") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, variables1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=2;");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 2}, U"a*x;");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1;");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 2}, U"f(x);");
    ASSERT_TRUE(res.ToStdString(0) == "1+x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, eval2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x+x;");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, eval3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"y+2*y+2;");
    ASSERT_TRUE(res.ToStdString(0) == "2+3*y") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, unary_plus1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;");
    ASSERT_TRUE(res.ToStdString(0) == "x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, unary_minus1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-x;");
    ASSERT_TRUE(res.ToStdString(0) == "-x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, sin1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sin(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, cos1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(0) == "cos(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, log1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2, 8);");
    ASSERT_TRUE(res.ToStdString(0) == "log(8)/log(2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, root1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToStdString(0) == "2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, simplify1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x+x);");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, diff2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x+x, x);");
    ASSERT_TRUE(res.ToStdString(0) == "2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, diff3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^2, x);");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, diff4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(sin(x), x);");
    ASSERT_TRUE(res.ToStdString(0) == "cos(x)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subs2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(yy^3, yy, 5);");
    ASSERT_TRUE(res.ToStdString(0) == "125") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString(0);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString(0);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a*x;") == parser.Parse(LogicalId{0, 0, 2}, U"5*x;")) << parser.Parse(LogicalId{0, 0, 2}, U"a*x;").ToStdString(0);
    parser.Parse(LogicalId{0, 0, 3}, U"b=x+1;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a*b;") == parser.Parse(LogicalId{0, 0, 4}, U"5*(1+x);")) << parser.Parse(LogicalId{0, 0, 4}, U"a*b;").ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, long_number_with_symbol)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890*x;");
    ASSERT_TRUE(res.ToStdString(0) == "12345678901234567890*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sin(x;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicRational, errors2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sinx);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicRational, tojson1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/2;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":14,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"1"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"2"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+x;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1/2;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"1"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":14,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"23"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"10"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson6)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"23"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"10"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson7)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(3456)/(34)*x;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"1728"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"17"})"
                    R"(]})"
                R"(]},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson8)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x*(234)/(67);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"234"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"67"})"
                    R"(]})"
                R"(]},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson9)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x-1/2;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"1"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, tojson10)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x-3/4*y;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"x"},)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"3"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"4"})"
                    R"(]})"
                R"(]},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"y"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, sqrt1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(4);");
    ASSERT_TRUE(res.ToStdString(0) == "2") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"2"})"
            R"(]})"
        R"(]})" ) << json;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sqrt(x)") << res.ToStdString(0);
    json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":15,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":14,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"1"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, pow1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x, 3);");
    ASSERT_TRUE(res.ToStdString(0) == "pow(x,3)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":15,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"3"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, expand2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)*(x-1));");
    ASSERT_TRUE(res.ToStdString(0) == "-1+pow(x,2)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"-1"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":15,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"x"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, log3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(8, 2);");
    std::string s = res.ToStdString(0);
    ASSERT_TRUE(s == "log(2)/log(8)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":14,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"str({"type":7,"elements":[{"type":8,"elements":"log(2)"}]})str"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"str({"type":7,"elements":[{"type":8,"elements":"log(8)"}]})str"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, diff_tojson)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^3+2*x^2-5*x+1, x);");
    std::string s = res.ToStdString(0);
    ASSERT_TRUE(s == "-5+4*x+3*pow(x,2)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"-5"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"4"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"x"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"3"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":15,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"x"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, subs_complex1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x^2+pow(y,2), x, 3);");
    ASSERT_TRUE(res.ToStdString(0) == "9+pow(y,2)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"9"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":15,"elements":[)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"y"})"
                    R"(]},)"
                    R"({"type":10,"elements":[]},)"
                    R"({"type":7,"elements":[)"
                        R"({"type":8,"elements":"2"})"
                    R"(]})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, expand3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)^3);");
    ASSERT_TRUE(res.ToStdString(0) == "1+3*x+3*pow(x,2)+pow(x,3)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, trig_power1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(x)^2+cos(x)^2);");
    ASSERT_TRUE(res.ToStdString(0) == "sinpow(x,2)+cospow(x,2)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, division_nested1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(x+1)/(x-1);");
    ASSERT_TRUE(res.ToStdString(0) == "(1+x)/(-1+x)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"-1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, diff_power5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^5,x);");
    ASSERT_TRUE(res.ToStdString(0) == "5*pow(x,4)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, diff_cos)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(cos(x),x);");
    ASSERT_TRUE(res.ToStdString(0) == "-sin(x)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, frac_power_mult)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^(1/2)*x^(1/3);");
    ASSERT_TRUE(res.ToStdString(0) == "pow(x,5/6)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"6"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, expand_diff_squares)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+y)*(x-y));");
    ASSERT_TRUE(res.ToStdString(0) == "pow(x,2)-pow(y,2)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":12,"symbol":"-"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"y"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, log_ratio)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(0) == "log(4)/log(2)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log(4)"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log(2)"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, sqrt_half)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(1/2);");
    ASSERT_TRUE(res.ToStdString(0) == "(1/2)*sqrt(2)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"1/2"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, subs_add)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x+y,y,2);");
    ASSERT_TRUE(res.ToStdString(0) == "2+x") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, evalf_exp)
{
    Parser<Symbolic<Rational>> parser10(10, Language::English);
    Symbolic<Rational> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(100));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"2.6881171418"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"10"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"43"}]}]}]}]})r" ) << json;
}

}
