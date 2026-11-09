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

TEST_F(CalcTestSymbolicRational, tojson11)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"y+3.4*y;");
    ASSERT_TRUE(res.ToStdString(0) == "(22/5)*y") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"22"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"5"}]}]},{"type":13,"symbol":"·"},{"type":8,"elements":"y"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, cot_tojson)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cot(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"cot"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, sec_tojson)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sec(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"sec"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

}
