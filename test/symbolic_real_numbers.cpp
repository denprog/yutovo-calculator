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

TEST_F(CalcTestSymbolicReal, numbers10)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
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
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, ln_zero)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(0);");
    ASSERT_TRUE(res.ToStdString(10) == "-∞") << res.ToStdString(10);
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
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs((pow(x,2)-1)/(x-1),x,1);");
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

TEST_F(CalcTestSymbolicReal, pi_symbol_output)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123(ee)/(π);");
    ASSERT_TRUE(res.ToStdString(10) == "123.*ee/π") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, infinity_syntax_error)
{
    for (size_t n = 2; n <= 5; ++n)
    {
        std::u32string expr(n, U'∞');
        expr += U';';
        ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, expr), SyntaxException) << "n=" << n;
    }
}

TEST_F(CalcTestSymbolicReal, infinity_in_identifier)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞x;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ab∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞ab;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, infinity_in_number)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12.3∞;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞1;"), SyntaxException);
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"∞12.3;"), SyntaxException);
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

TEST_F(CalcTestSymbolicReal, locale_comma_decimal)
{
    //numbers must survive a comma decimal locale: on Windows CLocaleGuard forces the C locale around the CRT parsing (giac's own "POSIX" shielding fails on 
    //MSVC because that locale name does not exist); on other platforms giac shields its strtod itself
    const char* saved_locale = std::setlocale(LC_NUMERIC, nullptr);

    struct LocaleRestore
    {
        std::string saved;

        ~LocaleRestore()
        {
            std::setlocale(LC_NUMERIC, saved.c_str());
        }
    };

    LocaleRestore restore{ saved_locale ? saved_locale : "" };

    const char* comma_locale = nullptr;
    for (const char* name : {"ru-RU.UTF-8", "ru_RU.UTF-8", "ru-RU", "ru_RU", "Russian_Russia.1251", "de-DE.UTF-8", "de-DE"})
    {
        if (std::setlocale(LC_NUMERIC, name) != nullptr)
        {
            comma_locale = name;
            break;
        }
    }
    if (!comma_locale)
        GTEST_SKIP() << "no comma decimal locale is installed";

    char* end = nullptr;
    double parsed = std::strtod("1.5", &end);
    if (parsed == 1.5 && *end == 0)
        GTEST_SKIP() << "locale " << comma_locale << " did not install a comma decimal separator";

    Symbolic<Real> res = parser.Parse(LogicalId{ 0, 0, 0, 0, 1 }, U"1.5+2.25;", 3);
    ASSERT_TRUE(res.ToString(10) == U"3.75") << res.ToStdString(10);
    res = parser.Parse(LogicalId{ 0, 0, 1 }, U"1.5*x+2.25*x;", 3);
    ASSERT_TRUE(res.ToString(10) == U"3.75*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, implicit_mul_div)
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
