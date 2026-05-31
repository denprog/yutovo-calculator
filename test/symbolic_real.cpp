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

TEST_F(CalcTestSymbolicReal, numbers1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;", 3);
    ASSERT_TRUE(res.ToString(10) == U"1.") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"0.2;", 3);
    ASSERT_TRUE(res.ToString(10) == U"0.2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, numbers2)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123456789012345678901234567890;");
    std::string s = res.ToStdString(30);
    ASSERT_TRUE(s == "123456789012345678901234567890") << s;
}

TEST_F(CalcTestSymbolicReal, numbers4)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123456789012345678901234567890.123;");
    std::string s = res.ToStdString(20);
    ASSERT_TRUE(s == "1.235E+29") << s;
}

TEST_F(CalcTestSymbolicReal, numbers5)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"7890;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "7.89E+3") << s;
}

TEST_F(CalcTestSymbolicReal, numbers6)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-7890;");
    std::string s = res.ToStdString(3);
    ASSERT_TRUE(s == "-7.89E+3") << s;
}

TEST_F(CalcTestSymbolicReal, numbers7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3;", 3);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "2.3") << s;
}

TEST_F(CalcTestSymbolicReal, numbers8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-2.3;", 3);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "-2.3") << s;
}

TEST_F(CalcTestSymbolicReal, numbers9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3*x;", 3);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s.find("2.3") != std::string::npos) << s;
}

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
}

TEST_F(CalcTestSymbolicReal, eval8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+-x;");
    ASSERT_TRUE(res.ToString(10) == U"-x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"x\"}]}]}") << res.ToJson(10);
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

TEST_F(CalcTestSymbolicReal, inf1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inf;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf10)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((x^2-1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, inf11)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-∞;");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan+1;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x*nan/3;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(0)/(0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(∞-∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(0*∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((∞)/(∞),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(∞,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, nan9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(0,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
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

TEST_F(CalcTestSymbolicReal, sin1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToString(10) == U"sin(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, cos1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToString(10) == U"cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, log1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2, 8);");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, log2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(x,y);");
    ASSERT_TRUE(res.ToStdString(10, Language::Russian) == "ln(y)/ln(x)") << res.ToStdString(10, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"y"},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicReal, root1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToString(10) == U"2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, expand1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow(x + 1, 2));");
    ASSERT_TRUE(res.ToString(10) == U"1.+2.*x+pow(x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, simplify1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x + x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, sin_pi_12)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(pi/12));");
    ASSERT_TRUE(res.ToStdString(10) == "0.259") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json == R"xxx({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"0.25"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.5"}]}]},{"type":13,"symbol":"·"},{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"3"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.5"}]}]}]}]}]})xxx") << json;
}

TEST_F(CalcTestSymbolicReal, simplify2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"simplify(f(x));");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, simplify3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(x)^2+cos(x)^2);");
    ASSERT_TRUE(res.ToStdString(10) == "sinpow(x,2.)+cospow(x,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, simplify4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(x+y,2));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicReal, simplify5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(x+y,2)+pow(x+y,2));");
    ASSERT_TRUE(res.ToStdString(10) == "2.*pow(x+y,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})json" ) << json;
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

TEST_F(CalcTestSymbolicReal, diff1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(pow(x, 2), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, diff2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"diff(f(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, diff3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"diff(f(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, diff4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(sin(x), x);");
    ASSERT_TRUE(res.ToString(10) == U"cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subs1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(x, 2), x, 5);");
    ASSERT_TRUE(res.ToString(10) == U"25.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subs2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(yy, 3), yy, 5);");
    ASSERT_TRUE(res.ToStdString(10) == "125.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subs3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x^2+pow(y,2), x, 3);");
    ASSERT_TRUE(res.ToString(10) == U"9.+pow(y,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"9\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"y\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2\"}]}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, subs4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(x,2),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, subs5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x+y,y,2);");
    ASSERT_TRUE(res.ToStdString(10) == "2.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subs6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(2,x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subs7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(x,2),x,2);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subscript1)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript2)
{
    // Text subscript
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{us};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript3)
{
    // Multi-character subscript
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{ab};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript4)
{
    // Expression with multiple variables, one subscripted
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2}+y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript5)
{
    // Subscript in implicit multiplication
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript6)
{
    // Subscript as function argument
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x{2});"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, evalf1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);", 3);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "3.142") << s;
}

TEST_F(CalcTestSymbolicReal, evalf2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "3.142") << s;
}

TEST_F(CalcTestSymbolicReal, precision1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);", 15);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s.length() == 17) << s;
    ASSERT_TRUE(s.find("3.141592653589793") != std::string::npos) << s;
}

TEST_F(CalcTestSymbolicReal, precision2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23456789012345678;", 15);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s.length() == 17) << s;
    ASSERT_TRUE(s.find("1.234567890123457") != std::string::npos) << s;
}

TEST_F(CalcTestSymbolicReal, member_checks1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0;");
    ASSERT_TRUE(res.IsZero());
    ASSERT_TRUE(res.IsNumber());

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5;");
    ASSERT_FALSE(res.IsZero());
    ASSERT_TRUE(res.IsNumber());

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    ASSERT_FALSE(res.IsZero());
    ASSERT_FALSE(res.IsNumber());

    //parse without explicit precision uses -1; use explicit precision to check setter
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;", 10);
    ASSERT_EQ(res.GetPrecision(), 10);
}

TEST_F(CalcTestSymbolicReal, logical_not)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"!x;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, logical_and)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x && y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, logical_or)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x || y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_eq)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x == y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_ne)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x != y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_lt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x < y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_le)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x <= y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_gt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x > y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, relational_ge)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x >= y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, variables1)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v;") == parser.Parse(LogicalId{0, 0, 2}, U"555;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"v;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"vp=x;", &dependencies);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;", &dependencies) == parser.Parse(LogicalId{0, 0, 3, 0}, U"x+5;")) <<
        parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;").ToStdString(10);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestSymbolicReal, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString(10);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables3)
{
    parser.Parse(LogicalId{0, 0, 2}, U"a=77;");
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"a;") == parser.Parse(LogicalId{0, 0, 3}, U"77;")) << parser.Parse(LogicalId{0, 0, 3}, U"a;").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d+5;") == parser.Parse(LogicalId{0, 0, 2}, U"9;")) << parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"d=45+d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;") == parser.Parse(LogicalId{0, 0, 4}, U"54;")) << parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a*x;") == parser.Parse(LogicalId{0, 0, 2}, U"5*x;")) << parser.Parse(LogicalId{0, 0, 2}, U"a*x;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"b=x+1;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a*b;") == parser.Parse(LogicalId{0, 0, 4}, U"5*(1+x);")) << parser.Parse(LogicalId{0, 0, 4}, U"a*b;").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables7)
{
    //in Symbolic mode, unknown identifiers become symbols
    Symbolic res = parser.Parse(LogicalId{0, 0, 1}, U"sin;");
    ASSERT_TRUE(res.ToStdString(10) == "sin") << res.ToStdString(10);

    parser.Parse(LogicalId{0, 0, 1}, U"sin=2;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"sin;");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 3}, U"sin(2);");
    ASSERT_TRUE(res.ToStdString(10) == "0.909") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables8)
{
    parser.Parse(LogicalId{0, 0, 5}, U"b=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 1}, U"b=2;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "4.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);

    parser.Parse(LogicalId{0, 0, 1}, U"b=5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "5.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "4.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, variables10)
{
    yutovo_calculator::ParserContext parser_context;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    parser.Parse(LogicalId{0, 0, 2}, U"v;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestSymbolicReal, variables11)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=1/2;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"a+x;");
    ASSERT_TRUE(res.ToStdString(10) == "0.5+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) <<
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) <<
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString(10);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString(10);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) <<
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestSymbolicReal, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString(10) == "5.") << res.ToStdString(10);

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString(10) == "9.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "6.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "10.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "20.") << res.ToStdString(10);
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "10.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "6.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "10.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "20.") << res.ToStdString(10);
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(10) == "20.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString(10) == "25.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString(10) == "4.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, user_functions9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"y=x+1;");
    parser.Parse(LogicalId{0, 0, 2}, U"r=pow(y,2);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 3}, U"r;");
    ASSERT_TRUE(res.ToStdString(10) == "pow(1.+x,2.)") << res.ToStdString(10);
    parser.Parse(LogicalId{0, 0, 4}, U"f(t)=t+r;");
    res = parser.Parse(LogicalId{0, 0, 5}, U"f(3);");
    ASSERT_TRUE(res.ToStdString(10) == "3.+pow(1.+x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, real1)
{
    Real re(53, 3.5f);
    Symbolic s(53, re);
    ASSERT_TRUE(s.ToStdString(10) == "3.5") << s.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, real2)
{
    Real re(53, 1.5f);
    Unit u(U"m");
    re.SetUnit(u);
    ASSERT_THROW(Symbolic(53, re), ParserException);
}

TEST_F(CalcTestSymbolicReal, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sin(x;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicReal, errors2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sinx);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicReal, tojson1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/x;");
    std::string json = res.ToJson(10);
    std::string expected = 
        "{\"type\":45,\"elements\":["
            "{\"type\":15,\"elements\":["
                "{\"type\":7,\"elements\":["
                    "{\"type\":8,\"elements\":\"x\"}"
                "]},"
                "{\"type\":10,\"elements\":[]},"
                "{\"type\":7,\"elements\":["
                    "{\"type\":12,\"symbol\":\"-\"},"
                    "{\"type\":8,\"elements\":\"1\"}"
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
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1.235\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"10\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"18\"}]}]}]},{\"type\":13,\"symbol\":\"·\"},{\"type\":8,\"elements\":\"x\"}]}]}"
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

TEST_F(CalcTestSymbolicReal, sqrt1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(4);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, sqrt2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "sqrt(x)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0.5\"}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, pow_operator1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^3;");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,3.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"3\"}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, diff_complex1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^3+2*x^2-5*x+1, x);");
    ASSERT_TRUE(res.ToString(10) == U"-5.+4.*x+3.*pow(x,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":12,\"symbol\":\"-\"},{\"type\":8,\"elements\":\"5\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":8,\"elements\":\"4\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":8,\"elements\":\"x\"},{\"type\":11,\"symbol\":\"+\"},{\"type\":8,\"elements\":\"3\"},{\"type\":13,\"symbol\":\"·\"},{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"2\"}]}]}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, evalf3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);", 20);
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "3.14159265358979323846") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"3.14159265358979323846\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicReal, root2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(x, 3);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "pow(x,0.333)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":15,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0.333\"}]}]}]}"
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

TEST_F(CalcTestSymbolicReal, expand3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)^3);");
    ASSERT_TRUE(res.ToStdString(10) == "1.+3.*x+3.*pow(x,2.)+pow(x,3.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]}]})r" ) << res.ToJson(10);
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
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, diff_power5)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^5,x);");
    ASSERT_TRUE(res.ToStdString(10) == "5.*pow(x,4.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, diff_cos)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(cos(x),x);");
    ASSERT_TRUE(res.ToStdString(10) == "-sin(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, diff_nonsymbol)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x, x+1);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x, 5);"), yutovo_calculator::ParserException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x, sin(x));"), yutovo_calculator::ParserException);
}

TEST_F(CalcTestSymbolicReal, frac_power_mult)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^(1/2)*x^(1/3);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,0.833)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.833"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, expand_diff_squares)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+y)*(x-y));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,2.)-pow(y,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":12,"symbol":"-"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"y"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, log_ratio)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"4"},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sqrt_half)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(1/2);");
    ASSERT_TRUE(res.ToStdString(10) == "0.707") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"0.5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.5"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, evalf_exp)
{
    Parser<Symbolic<Real>> parser10(10, Language::English);
    Symbolic<Real> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(100));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"2.6881171418"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"10"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"43"}]}]}]}]})r" ) << json;
}

TEST_F(CalcTestSymbolicReal, evalf_exp1000)
{
    Parser<Symbolic<Real>> parser10(10, Language::English);
    Symbolic<Real> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(1000));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"∞"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, limit_cot0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cot(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sec_pi2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sec(x),x,(pi)/(2));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_csc0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csc(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sin_x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"nan\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, limit_exp_m1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((exp(x)-1)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_ln_x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x)/x,x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"∞\"}]}]}") << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, limit_exp_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sin_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(sin(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_cos_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(cos(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_ln_1x)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(1/x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, limit_sqrt_noo)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, scientific1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e-5;");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "1.23E-5" || s == "0.0000123") << s;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e-5;");
    s = res.ToStdString(10);
    ASSERT_TRUE(s == "-1.23E-5" || s == "-0.0000123") << s;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e+5;");
    ASSERT_TRUE(res.ToStdString(10) == "-123000") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e+5;");
    ASSERT_TRUE(res.ToStdString(10) == "123000") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_sinh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_cosh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_tanh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"tanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_coth0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(coth(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_sech)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_csch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(csch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asinh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsinh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acosh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_atanh)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(0);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acoth1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcoth(x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asech)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acsch0)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(arcsch(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_symbolic)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sinh(x)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(x);");
    ASSERT_TRUE(res.ToStdString(10) == "acosh(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_alias)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosech(0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosech(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.881") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_acosh_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcosh(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, hyperbolic_asech_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arsech(2);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "nan" || s == "-nan.") << s;
}

TEST_F(CalcTestSymbolicReal, hyperbolic_artanh_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"artanh(2);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "nan" || s == "-nan.") << s;
}

TEST_F(CalcTestSymbolicReal, hyperbolic_arcoth_nan)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"arcoth(nan);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, log_x_base)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(x,2);");
    ASSERT_TRUE(res.ToStdString(10) == "log2./log(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]}]})r" ) << res.ToJson(10);
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

TEST_F(CalcTestSymbolicReal, sinh_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sinh(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sinh"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, cosh_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cosh(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"cosh"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, sin_expression_brackets)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x+1);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(1.+x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, nested_functions_brackets)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cot(sec(x));");
    ASSERT_TRUE(res.ToStdString(10) == "cot(sec(x))") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"cot"},{"type":19,"symbol":"("},{"type":8,"elements":"sec"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, ln_russian)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(10, Language::Russian) == "ln(x)") << res.ToStdString(10, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicReal, ln_in_expression_russian)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x)+1;");
    ASSERT_TRUE(res.ToStdString(10, Language::Russian) == "1.+ln(x)") << res.ToStdString(10, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicReal, ln_english)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(10) == "log(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicReal, log_change_of_base_russian)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(10, Language::Russian) == "2.") << res.ToStdString(10, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"4"},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]}]}]})r" ) << res.ToJson(10, Language::Russian);
}

}