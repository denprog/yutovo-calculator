/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestComplex, numbers1)
{
    std::string res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "0.2E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"0.2E+0;").ToStdString(3, 3);
    ASSERT_TRUE(res == "0.2E+0") << res;
}

TEST_F(CalcTestComplex, numbers_invalid_dots)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"."), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1..2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2.3"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3.4"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".1.2"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.2."), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestComplex, numbers2)
{
    std::string res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*1.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2+1i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "2.E+0+i*1.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*5.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5.678i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*5.678E+0") << res;
}

TEST_F(CalcTestComplex, numbers3)
{
    std::string res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"i*(1+2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-2.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*2.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(2+3i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "4.E+0+i*6.E+0") << res;
}

TEST_F(CalcTestComplex, percent1)
{
    std::string res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2+3i)%10;").ToStdString(3, 15);
    ASSERT_TRUE(res == "0.2E+0+i*0.3E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"200%50;").ToStdString(3, 15);
    ASSERT_TRUE(res == "100.E+0") << res;
}

TEST_F(CalcTestComplex, numbers4)
{
    parser.SetLocale(Language::Russian);
    std::string res;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*j;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
}

TEST_F(CalcTestComplex, numbers5)
{
    parser.SetLocale(Language::German);
    std::string res;
    //the German locale uses i as the imaginary unit, j is not an identifier
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*j;").ToStdString(3, 3), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2i+1;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0+i*2.E+0") << res;
}

TEST_F(CalcTestComplex, numbers6)
{
    parser.SetLocale(Language::French);
    std::string res;
    //the French locale uses i as the imaginary unit, j is not an identifier
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*j;").ToStdString(3, 3), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2i+1;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0+i*2.E+0") << res;
}

TEST_F(CalcTestComplex, numbers7)
{
    parser.SetLocale(Language::Italian);
    std::string res;
    //the Italian locale uses i as the imaginary unit, j is not an identifier
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*j;").ToStdString(3, 3), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2i+1;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0+i*2.E+0") << res;
}

TEST_F(CalcTestComplex, scientific1)
{
    std::string res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e-5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.23E-5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e-5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.23E-5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1.23e+5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.23E+5") << res;
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1.23e+5;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.23E+5") << res;
}

TEST_F(CalcTestComplex, pow1)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,0);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestComplex, pow2)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(0, 2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, pow3)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,-2);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestComplex, pow4)
{
    std::string res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,0);").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,1);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*2.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "-4.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,3);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-8.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,4);").ToStdString(3, 3);
    ASSERT_TRUE(res == "16.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2*i,-1);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-0.5E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(0.5*i,3);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-0.125E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(-2*i,3);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*8.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(-2*i,2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "-4.E+0") << res;
}

TEST_F(CalcTestComplex, sqrt1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(LogicalId{0, 0, 1}, U"sqrt(i);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "0.707E+0+i*0.707E+0,-0.707E+0+i*-0.707E+0") << res;

    results.clear();
    parser.Parse(LogicalId{0, 0, 1}, U"sqrt(3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    res = ToString(results);
    ASSERT_TRUE(res == "1.732E+0,-1.732E+0") << res;

    results.clear();
    parser.Parse(LogicalId{0, 0, 1}, U"sqrt(2i+3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    res = ToString(results);
    ASSERT_TRUE(res == "1.817E+0+i*0.55E+0,-1.817E+0+i*-0.55E+0") << res;
}

TEST_F(CalcTestComplex, sqrt2)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(LogicalId{0, 0, 1}, U"sqrt(sqrt(1+i));", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "1.07E+0+i*0.213E+0,0.213E+0+i*-1.07E+0,-1.07E+0+i*-0.213E+0,-0.213E+0+i*1.07E+0") << res;
}

TEST_F(CalcTestComplex, root1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(LogicalId{0, 0, 1}, U"root(1+i,3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "1.084E+0+i*0.291E+0,-0.794E+0+i*0.794E+0,-0.291E+0+i*-1.084E+0") << res;
}

TEST_F(CalcTestComplex, root2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"root(1+i,0);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, exp1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(LogicalId{0, 0, 1}, U"exp(i);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "0.54E+0+i*0.841E+0") << res;
}

TEST_F(CalcTestComplex, compare1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<3);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((3+5*i)==(3+5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((3+5*i)<>(3+5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestComplex, compare2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0}, U"a=3;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a==3+5*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<>3+5*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

TEST_F(CalcTestComplex, pow_int)
{
    int res_pos = 0;
    Complex base(64, 2);
    Complex res = pow(base, 0, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
    res = pow(base, 1, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);
    res = pow(base, 2, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
    res = pow(base, 3, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "8.E+0") << res.ToStdString(3, 3);
    res = pow(base, 10, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.024E+3") << res.ToStdString(3, 3);
    res = pow(base, -1, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.5E+0") << res.ToStdString(3, 3);
    res = pow(base, -2, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.25E+0") << res.ToStdString(3, 3);
    res = pow(base, -3, res_pos);
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.125E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, sqrt_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sqrt(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+10") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, root_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"root(pow(10,20),3);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.642E+6") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, pow_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"pow(10,20);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+20") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, implicit_mul_div2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=1+1i;");
    parser.Parse(LogicalId{0, 0, 2}, U"x=3;");
    parser.Parse(LogicalId{0, 0, 3}, U"y=6;");
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"2a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0+i*2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 5}, U"(x)/(y)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.5E+0+i*0.5E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 6}, U"(1/2)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.5E+0+i*0.5E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 7}, U"2(a+1);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "4.E+0+i*2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 8}, U"2abs(3+4i);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "10.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 9}, U"abs(3+4i)a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.E+0+i*5.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, errors1)
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


TEST_F(CalcTestComplex, errors3)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"(1)/(0);"), yutovo_calculator::MathException);
}


TEST_F(CalcTestComplex, errors4)
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


}
