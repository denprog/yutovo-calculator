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

TEST_F(CalcTestComplex, functions1)
{
    std::string res = parser.Parse(LogicalId{0, 0, 1}, U"ln(i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*1.571E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"ln(5+i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.629E+0+i*0.197E+0") << res;
}

TEST_F(CalcTestComplex, functions2)
{
    std::string res = parser.Parse(LogicalId{0, 0, 1}, U"pow(i,2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 1}, U"pow(2i,3);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-8.E+0") << res;
}

TEST_F(CalcTestComplex, functions3)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2exp(1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.437E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions4)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a=5;");
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(2,3)a;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "40.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions5)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"re(2);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"re(4+5*i);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "4.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"re(5*i);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions6)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"im(2);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"im(4+5*i);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"im(5*i);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions7)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"mod(2);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"mod(-6+3*i);", 3);
    auto r1 = parser.Parse(LogicalId{0, 0, 1}, U"sqrt(45);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == r1.ToStdString(3, 3)) << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions8)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arg(5+4i);", 3);
    auto r1 = parser.Parse(LogicalId{0, 0, 1}, U"arctg(4/5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == r1.ToStdString(3, 3)) << r.ToStdString(3, 3) << " " << r1.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions9)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(10);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.571E+0+i*-2.993E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions10)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"arg(10);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions11)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(2);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-6+3*i);", 3);
    auto r1 = parser.Parse(LogicalId{0, 0, 1}, U"sqrt(45);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == r1.ToStdString(3, 3)) << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestComplex, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Complex res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "9.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Complex res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Complex res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Complex res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "25.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Complex res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, user_functions9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"y=(sin((pi)/(2)))/(3.4);");
    parser.Parse(LogicalId{0, 0, 2}, U"r=sqrt(y);");
    Complex res = parser.Parse(LogicalId{0, 0, 3}, U"r;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.542E+0") << res.ToStdString(3, 3);
    ASSERT_TRUE(res.GetBitPrecision() < 1000) << res.GetBitPrecision();
    parser.Parse(LogicalId{0, 0, 4}, U"f(t)=t*cos(r*2);");
    res = parser.Parse(LogicalId{0, 0, 5}, U"f(3.4);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.589E+0") << res.ToStdString(3, 3);
    ASSERT_TRUE(res.GetBitPrecision() < 1000) << res.GetBitPrecision();
}

TEST_F(CalcTestComplex, sin_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sin(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-0.645E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, cos_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"cos(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 1)) << res.ToStdString(3, 10);
    ASSERT_FALSE(res.GetRe() == Real(3, -1)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.764E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, tg_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"tg(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-0.845E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, ctg_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"ctg(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-1.184E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, sec_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"sec(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 1)) << res.ToStdString(3, 10);
    ASSERT_FALSE(res.GetRe() == Real(3, -1)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.309E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, cosec_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"cosec(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_FALSE(res.GetRe() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.GetIm() == Real(3, 0)) << res.ToStdString(3, 10);
    ASSERT_TRUE(res.ToStdString(3, 3) == "-1.55E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, ln_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"ln(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "46.052E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, lg_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"lg(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, log_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"log(2,pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "66.439E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, exp_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"exp(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, arctg_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arctg(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arcctg_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcctg(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arcsec_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsec(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.571E+0+i*5.E-41") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arccosec_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arccosec(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E-20+i*-5.E-41") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, th_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"th(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, cth_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"cth(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, sch_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sch(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, csch_large_arg)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"csch(pow(10,20));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestComplex, arsh_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arsh(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "46.745E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arcth_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcth(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.942E-21") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arcsch_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsch(pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E-20") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arcsin_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.571E+0+i*-46.745E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arccos_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arccos(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.508E-20+i*46.745E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, arch_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"arch(pow(10,20));");
    ASSERT_TRUE(res.ToStdString(3, 3) == "46.745E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, abs_large_arg)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"abs(-pow(10,20));");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+20") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, fact1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "120.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10!;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.629E+6") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, fact2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(100);
#ifdef _WIN32
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5000000!;", &parser_context), yutovo_calculator::TimeExceedException);
#else
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1000000!;", &parser_context), yutovo_calculator::TimeExceedException);
#endif
}

TEST_F(CalcTestComplex, min_max1)
{
    Complex res = parser.Parse(LogicalId{0, 0, 1}, U"min(1,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(1,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"min(1+2i,3+4i);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "1.E+0+i*2.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"max(1+2i,3+4i);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0+i*4.E+0") << res.ToStdString(3, 3);
}

}
