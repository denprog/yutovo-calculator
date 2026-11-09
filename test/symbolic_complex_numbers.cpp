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
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);

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

TEST_F(CalcTestSymbolicComplex, numbers4)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, numbers5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"z+0.12345678901234567890123456789012345678901234567890+0.98765432109876543210987654321098765432109876543210*i;", 50);
    std::string s = res.ToStdString(50);
    ASSERT_TRUE(s.find("0.123456789012345678901234567890123456789012345678") != std::string::npos) << s;
    ASSERT_TRUE(s.find("0.987654321098765432109876543210987654321098765432") != std::string::npos) << s;
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

TEST_F(CalcTestSymbolicComplex, infinity_syntax_error)
{
    for (size_t n = 2; n <= 5; ++n)
    {
        std::u32string expr(n, U'∞');
        expr += U';';
        ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, expr), SyntaxException) << "n=" << n;
    }
}

TEST_F(CalcTestSymbolicComplex, infinity_in_identifier)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞x;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ab∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞ab;"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, infinity_in_number)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.3∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞1;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞12.3;"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, imaginary1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*i;");
    ASSERT_TRUE(res.ToStdString(10) == "3.*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imaginary2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3-34.5*i;");
    ASSERT_TRUE(res.ToStdString(10) == "3.-34.5*i") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imaginary_variable1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2+3*i)*x;");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(res.ToStdString(10) == "(2.+3.*i)*x") << res.ToStdString(10);
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

TEST_F(CalcTestSymbolicComplex, inf1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"inf;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞;");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,-∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf6)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(ln(x),x,0);");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, ln_zero)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(0);");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf7)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(exp(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf8)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x),x,∞);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf9)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf10)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((pow(x,2)-1)/(x-1),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, inf11)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-∞;");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan2)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"nan+1;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan3)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x*nan/3;");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan4)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(0)/(0);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan5)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(∞-∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan6)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(0*∞,x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan7)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((∞)/(∞),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "nan") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan8)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(∞,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, nan9)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(0,0),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, scientific1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e-5;");
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

TEST_F(CalcTestSymbolicComplex, imag_unit1)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i;");
    ASSERT_TRUE(res.ToStdString(10) == "i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, imag_unit2)
{
    parser.SetLocale(Language::English);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i*i;");
    ASSERT_TRUE(res.ToJson(10) == R"({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"}]}]})" ) << res.ToJson(10);
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"j*j;");
    ASSERT_TRUE(res.ToStdString(10) == "pow(j,2.)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"I*I;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imag_unit3)
{
    parser.SetLocale(Language::Russian);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i*i;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == R"({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"}]}]})" ) << res.ToJson(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"j*j;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == R"({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"}]}]})" ) << res.ToJson(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"I*I;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) == R"({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"1"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, imag_unit4)
{
    parser.SetLocale(Language::Spanish);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i*i;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"j*j;");
    ASSERT_TRUE(res.ToStdString(10) == "pow(j,2.)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"I*I;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imag_unit5)
{
    parser.SetLocale(Language::BrazilianPortuguese);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i*i;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"j*j;");
    ASSERT_TRUE(res.ToStdString(10) == "pow(j,2.)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"I*I;");
    ASSERT_TRUE(res.ToStdString(10) == "-1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, imag_unit_minus_i)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-i;");
    ASSERT_TRUE(res.ToStdString(10) == "-i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":12,"symbol":"-"},{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, complex_1_plus_i)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1+i;");
    ASSERT_TRUE(res.ToStdString(10) == "1.+i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, complex_1_minus_i)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1-i;");
    ASSERT_TRUE(res.ToStdString(10) == "1.-i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":12,"symbol":"-"},{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, complex_2_plus_i)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2+i;");
    ASSERT_TRUE(res.ToStdString(10) == "2.+i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":11,"symbol":"+"},{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, complex_2_minus_i)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2-i;");
    ASSERT_TRUE(res.ToStdString(10) == "2.-i") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":47,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":12,"symbol":"-"},{"type":8,"elements":"i"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicComplex, implicit_mul_div)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=x;");
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2a;");
    ASSERT_TRUE(r.ToStdString(10) == "2.*x") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"(1/2)a;");
    ASSERT_TRUE(r.ToStdString(10) == "0.5*x") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"2(a+1);");
    ASSERT_TRUE(r.ToStdString(10) == "2.*(1.+x)") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"2sin(x);");
    ASSERT_TRUE(r.ToStdString(10) == "2.*sin(x)") << r.ToStdString(10);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"sin(x)a;");
    ASSERT_TRUE(r.ToStdString(10) == "sin(x)*x") << r.ToStdString(10);
}

}
