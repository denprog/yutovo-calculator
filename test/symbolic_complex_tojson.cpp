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

TEST_F(CalcTestSymbolicComplex, tojson1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+i;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+2*i;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"2"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.234-2.234*i;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1.234"},)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"2.234"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.234-2.234*i;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"1.234"},)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"2.234"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson6)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(1+2*i);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1"},)"
                R"({"type":11,"symbol":"+"},)"
                R"({"type":8,"elements":"2"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson7)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(1-2*i);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"1"},)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"2"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson8)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(-1.1-2*i);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"1.1"},)"
                R"({"type":12,"symbol":"-"},)"
                R"({"type":8,"elements":"2"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"i"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson_power1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x,(-1));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":14,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"1"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, tojson_imaginary1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2+3*i)*x;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"2"},)"
                    R"({"type":11,"symbol":"+"},)"
                    R"({"type":8,"elements":"3"},)"
                    R"({"type":13,"symbol":"·"},)"
                    R"({"type":8,"elements":"i"})"
                R"(]},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":8,"elements":"x"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, csc_tojson)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"csc(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"csc"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

}
