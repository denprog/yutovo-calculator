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

TEST_F(CalcTestSymbolicComplex, numbers1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 1}, U"1+2*i;");
    ASSERT_TRUE(res.ToStdString(10) == "1.+2.*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, numbers2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890.123;");
    ASSERT_TRUE(res.ToStdString(10) == "1.235E+19") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, numbers3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/3;", 3);
    ASSERT_TRUE(res.ToStdString(10) == "0.333") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, eval1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    ASSERT_TRUE(res.ToStdString(10) == "x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, addition1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 1;");
    ASSERT_TRUE(res.ToStdString(10) == "1.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, subtraction1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x - 1;");
    ASSERT_TRUE(res.ToStdString(10) == "-1.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, multiplication1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x * 2;");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, division1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x / 3;", 3);
    ASSERT_TRUE(res.ToStdString(10) == "0.333*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, mixed_arithmetic1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 2*x;");
    ASSERT_TRUE(res.ToStdString(10) == "3.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, mixed_arithmetic2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*x^2 + 3*x - 5;");
    ASSERT_TRUE(res.ToStdString(10) == "-5.+3.*x+2.*pow(x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, power1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^2;");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, precision1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/3;", 10);
    ASSERT_TRUE(res.ToStdString(10) == "0.3333333333") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, precision2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/3;", 20);
    ASSERT_TRUE(res.ToStdString(10) == "0.33333333333333333333") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^2, x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, expand1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)^2);");
    ASSERT_TRUE(res.ToStdString(10) == "1.+2.*x+pow(x,2.)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, subs1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x^2, x, 5);");
    ASSERT_TRUE(res.ToStdString(10) == "25.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imaginary1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*i;");
    ASSERT_TRUE(res.ToStdString(10) == "3.*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, variables1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=2;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 2}, U"a*x;");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 2}, U"f(x);");
    ASSERT_TRUE(res.ToStdString(10) == "1.+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, eval2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x+x;");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, eval3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"y+2*y+2;");
    ASSERT_TRUE(res.ToStdString(10) == "2.+3.*y") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, unary_plus1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;");
    ASSERT_TRUE(res.ToStdString(10) == "x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, unary_minus1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-x;");
    ASSERT_TRUE(res.ToStdString(10) == "-x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, sin1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToStdString(10) == "sin(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, cos1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, log1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2, 8);");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, root1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, simplify1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x+x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x+x, x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^2, x);");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, diff4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(sin(x), x);");
    ASSERT_TRUE(res.ToStdString(10) == "cos(x)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, subs2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(yy^3, yy, 5);");
    ASSERT_TRUE(res.ToStdString(10) == "125.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString(10);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a*x;") == parser.Parse(LogicalId{0, 0, 2}, U"5*x;")) << parser.Parse(LogicalId{0, 0, 2}, U"a*x;").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"b=x+1;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a*b;") == parser.Parse(LogicalId{0, 0, 4}, U"5*(1+x);")) << parser.Parse(LogicalId{0, 0, 4}, U"a*b;").ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, long_number_with_symbol)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890.123*x;");
    ASSERT_TRUE(res.ToStdString(10) == "1.235E+19*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sin(x;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, errors2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sinx);"), yutovo_calculator::SyntaxException);
}

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

TEST_F(CalcTestSymbolicComplex, sqrt1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(-1);");
    ASSERT_TRUE(res.ToStdString(10) == "1.*i") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"I"})"
            R"(]})"
        R"(]})" ) << json;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(x);");
    json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":15,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"0.5"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, pow1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x, 3);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,3.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
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

TEST_F(CalcTestSymbolicComplex, evalf1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);", 3);
    ASSERT_TRUE(res.ToStdString(10) == "3.142") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"3.142"})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, diff_complex1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^3+2*x^2-5*x+1, x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "-5.+4.*x+3.*pow(x,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
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

TEST_F(CalcTestSymbolicComplex, subs_complex1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x^2+pow(y,2), x, 3);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "9.+pow(y,2.)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
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

TEST_F(CalcTestSymbolicComplex, tojson_power1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^(-1);");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":15,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":12,"symbol":"-"},)"
                    R"({"type":8,"elements":"1"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, imaginary_variable1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2+3*i)*x;");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "x*(2.+3.*i)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"x"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"2"},)"
                    R"({"type":11,"symbol":"+"},)"
                    R"({"type":8,"elements":"3"},)"
                    R"({"type":13,"symbol":"·"},)"
                    R"({"type":8,"elements":"i"})"
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
                R"({"type":8,"elements":"x"},)"
                R"({"type":13,"symbol":"·"},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"2"},)"
                    R"({"type":11,"symbol":"+"},)"
                    R"({"type":8,"elements":"3"},)"
                    R"({"type":13,"symbol":"·"},)"
                    R"({"type":8,"elements":"i"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, root2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"2"})"
            R"(]})"
        R"(]})" ) << json;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(x, 3);");
    json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":47,"elements":[)"
            R"({"type":15,"elements":[)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]},)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"0.333"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicComplex, expand3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)^3);");
    ASSERT_TRUE(res.ToStdString(10) == "1.+3.*x+3.*pow(x,2.)+pow(x,3.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, trig_power1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(sin(x)^2+cos(x)^2);");
    ASSERT_TRUE(res.ToStdString(10) == "sinpow(x,2.)+cospow(x,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"cos"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, division_nested1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(x+1)/(x-1);");
    ASSERT_TRUE(res.ToStdString(10) == "(1.+x)/(-1.+x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"-1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, diff_power5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(x^5,x);");
    ASSERT_TRUE(res.ToStdString(10) == "5.*pow(x,4.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"4"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, diff_cos)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(cos(x),x);");
    ASSERT_TRUE(res.ToStdString(10) == "-sin(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":7,"elements":[{"type":8,"elements":"sin"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, frac_power_mult)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x^(1/2)*x^(1/3);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,0.833)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.833"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, expand_diff_squares)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+y)*(x-y));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,2.)-pow(y,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":12,"symbol":"-"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"y"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, log_ratio)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log(4)"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":7,"elements":[{"type":8,"elements":"log(2)"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, sqrt_half)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(1/2);");
    ASSERT_TRUE(res.ToStdString(10) == "0.707") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"0.5"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"0.5"}]}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, subs_add)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x+y,y,2);");
    ASSERT_TRUE(res.ToStdString(10) == "2.+x") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, evalf_exp)
{
    Parser<Symbolic<Complex>> parser10(10, Language::English);
    Symbolic<Complex> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(100));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"2.6881171418"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"10"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"43"}]}]}]}]})r" ) << json;
}

}
