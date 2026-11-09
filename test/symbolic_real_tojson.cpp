/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include <clocale>
#include <condition_variable>
#include <cstdlib>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "mock.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

TEST_F(CalcTestSymbolicReal, tmp_editor_json)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123*ee/п;");
    std::cerr << "TEXT: " << res.ToStdString(10) << std::endl;
    std::cerr << "JSON: " << res.ToJson(10) << std::endl;
}

TEST_F(CalcTestSymbolicReal, tojson1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/x;");
    std::string json = res.ToJson(10);
    std::string expected =
        "{\"type\":45,\"elements\":["
            "{\"type\":14,\"elements\":["
                "{\"type\":7,\"elements\":["
                    "{\"type\":8,\"elements\":\"1\"}"
                "]},"
                "{\"type\":10,\"elements\":[]},"
                "{\"type\":7,\"elements\":["
                    "{\"type\":8,\"elements\":\"x\"}"
                "]}"
            "]}"
        "]}";
    ASSERT_EQ(json, expected) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_negative_power_fraction)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(root(y,3)),y,(pi)/(2));", 10);
    std::string json = res.ToJson(10);
    std::string expected =
        "{\"type\":45,\"elements\":["
            "{\"type\":14,\"elements\":["
                "{\"type\":7,\"elements\":["
                    "{\"type\":8,\"elements\":\"1\"}"
                "]},"
                "{\"type\":10,\"elements\":[]},"
                "{\"type\":7,\"elements\":["
                    "{\"type\":17,\"elements\":["
                        "{\"type\":7,\"elements\":["
                            "{\"type\":8,\"elements\":\"3\"}"
                        "]},"
                        "{\"type\":10,\"elements\":[]},"
                        "{\"type\":7,\"elements\":["
                            "{\"type\":8,\"elements\":\"1.5707963268\"}"
                        "]}"
                    "]}"
                "]}"
            "]}"
        "]}";
    ASSERT_EQ(json, expected) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345;");
    std::string json = res.ToJson(2);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1.234\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"4\"}]}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"12345\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.00123;");
    std::string json = res.ToJson(2);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0.001\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123.45;");
    std::string json = res.ToJson(2);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1.235\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2\"}]}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp5)
{
    Parser<Symbolic<Real>> parser15(15, Language::English);
    Symbolic<Real> res = parser15.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.000000000000123;");
    std::string json = res.ToJson(2);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0.000000000000123\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_exp6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1234567890123456787*x;");
    std::string json = res.ToJson(2);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1.235\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"18\"}]}]},{\"type\":13,\"symbol\":\"·\"},{\"type\":8,\"elements\":\"x\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_decimal1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2.3\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_decimal2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"2.3\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_sin)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"sin\"},{\"type\":19,\"symbol\":\"(\"},{\"type\":8,\"elements\":\"x\"},{\"type\":20,\"symbol\":\")\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, tojson_sincos)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x)+cos(x);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"cos\"},{\"type\":19,\"symbol\":\"(\"},{\"type\":8,\"elements\":\"x\"},{\"type\":20,\"symbol\":\")\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":8,\"elements\":\"sin\"},{\"type\":19,\"symbol\":\"(\"},{\"type\":8,\"elements\":\"x\"},{\"type\":20,\"symbol\":\")\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, cot_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cot(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"cot"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sec_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sec(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sec"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, csc_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"csc(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"csc"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

}
