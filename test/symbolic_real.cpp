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

TEST_F(CalcTestSymbolicReal, numbers3)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123456789012345678901234567890.123;");
    std::string s = res.ToStdString(20);
    ASSERT_TRUE(s == "1.235E+29") << s;
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

TEST_F(CalcTestSymbolicReal, eval1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x;");
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, eval2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x+x;");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, eval3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"y+2*y+2;");
    ASSERT_TRUE(res.ToString(10) == U"2+3*y") << res.ToStdString(10);
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

TEST_F(CalcTestSymbolicReal, unary_plus1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;", 3);
    ASSERT_TRUE(res.ToString(10) == U"x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, unary_minus1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-x;");
    ASSERT_TRUE(res.ToString(10) == U"-x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, addition1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 1;");
    ASSERT_TRUE(res.ToString(10) == U"1+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subtraction1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x - 1;");
    ASSERT_TRUE(res.ToString(10) == U"-1+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, multiplication1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x * 2;");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, division1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x / 3;");
    ASSERT_TRUE(res.ToString(10) == U"0.333*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, mixed_arithmetic1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 2*x;");
    ASSERT_TRUE(res.ToString(10) == U"3*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, power1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x, 2);");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,2)") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x + 1, 2);");
    ASSERT_TRUE(res.ToString(10) == U"pow(1+x,2)") << res.ToStdString(10);
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

TEST_F(CalcTestSymbolicReal, root1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToString(10) == U"2.") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, expand1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow(x + 1, 2));");
    ASSERT_TRUE(res.ToString(10) == U"1+2*x+pow(x,2)") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, simplify1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x + x);");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, simplify2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"simplify(f(x));");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, diff1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(pow(x, 2), x);");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
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
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
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

TEST_F(CalcTestSymbolicReal, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a{1};") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(10);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;") == parser.Parse(LogicalId{0, 0, 4}, U"12;")) << parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(10);
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

TEST_F(CalcTestSymbolicReal, variables9)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v{12}=555;");
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"v{12};", &dependencies).ToStdString(10);
    ASSERT_TRUE(res == "555.") << res;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"v{12}") != dependencies.end());
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
    ASSERT_TRUE(res.ToStdString(10) == "pow(1+x,2)") << res.ToStdString(10);
    parser.Parse(LogicalId{0, 0, 4}, U"f(t)=t+r;");
    res = parser.Parse(LogicalId{0, 0, 5}, U"f(3);");
    ASSERT_TRUE(res.ToStdString(10) == "3+pow(1+x,2)") << res.ToStdString(10);
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

}
