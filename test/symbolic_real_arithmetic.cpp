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

TEST_F(CalcTestSymbolicReal, eval1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, eval2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x+x;");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, eval3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"y+2*y+2;");
    ASSERT_TRUE(res.ToString(10) == U"2.+3.*y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, eval4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123456789012345678901234567890.123+x;");
    std::string s = res.ToStdString(20);
    ASSERT_TRUE(s == "1.235E+29+x") << s;
}

TEST_F(CalcTestSymbolicReal, eval5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x)+2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "2.357E+3*x+sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, eval6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x)-2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "-2.357E+3*x+sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, eval7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x+(x)/(3);");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "1.333*x") << s;
    std::string j = res.ToJson(3);
    ASSERT_TRUE(j.find("1333") == std::string::npos) << j;
}

TEST_F(CalcTestSymbolicReal, eval8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+-x;");
    ASSERT_TRUE(res.ToString(10) == U"-x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"x\"}]}]}") << 
        res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, eval9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, eval10)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"--x;");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, eval11)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"++x;");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, eval12)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x)--2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "2.357E+3*x+sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, eval13)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x)-+2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "-2.357E+3*x+sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, eval14)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-sin(x)-2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "-2.357E+3*x-sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, eval15)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"--+sin(x)--+2356.78*x;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "2.357E+3*x+sin(x)") << s;
}

TEST_F(CalcTestSymbolicReal, unary_plus1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;", 3);
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, unary_minus1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-x;");
    ASSERT_TRUE(res.ToString(10) == U"-x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"x\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, addition1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 1;");
    ASSERT_TRUE(res.ToString(10) == U"1.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subtraction1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x - 1;");
    ASSERT_TRUE(res.ToString(10) == U"-1.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, multiplication1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x * 2;");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, division1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x / 3;");
    ASSERT_TRUE(res.ToString(10) == U"0.333*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, division2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1 / 0;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, division3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0 / 0;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"nan\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, mixed_arithmetic1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 2*x;");
    ASSERT_TRUE(res.ToString(10) == U"3.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, power1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x, 2);");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,2.)") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x + 1, 2);");
    ASSERT_TRUE(res.ToString(10) == U"pow(1.+x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, power2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(pow(10,100));");
    ASSERT_TRUE(res.ToStdString(10) == "1E-100") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"100\"}]}]}]}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, power3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-(1)/pow(10,100);");
    ASSERT_TRUE(res.ToStdString(10) == "-1E-100") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"1\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"100\"}]}]}]}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, mul_neg_add)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x*(-1+y);");
    ASSERT_TRUE(res.ToStdString(10) == "x*(-1.+y)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json == R"xxx({"type":45,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"x"},{"type":13,"symbol":"·"},{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]}]}]})xxx") << json;
}

TEST_F(CalcTestSymbolicReal, power_mul_base1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x*y, z);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x*y,z)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":13,"symbol":"·"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"z"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, power_add_exp1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x+y, z);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x+y,z)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"z"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, power_neg_exp1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x+y, -2);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x+y,-2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"2"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, nested_power1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(pow(x+y, 2), z);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(pow(x+y,2.),z)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"z"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, pow_operator1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x,3);");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,3.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"3\"}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, complex_mul_symbol1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*i;");
    ASSERT_TRUE(res.ToStdString(10) == "2.*i") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":13,"symbol":"·"},{"type":8,"elements":"i"}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, division_nested1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(x+1)/(x-1);");
    ASSERT_TRUE(res.ToStdString(10) == "(1.+x)/(-1.+x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, frac_power_mult)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x,(1/2))*pow(x,(1/3));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,0.833)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.833"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, log_ratio)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"4"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sqrt_half)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(1/2);");
    ASSERT_TRUE(res.ToStdString(10) == "0.707") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"0.5"},{"type":13,"symbol":"·"},{"type":16,"elements":[{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

}
