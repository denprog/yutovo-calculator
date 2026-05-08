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

TEST_F(CalcTestSymbolic, numbers1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;");
    ASSERT_TRUE(res.ToString() == U"1") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 1}, U"0.2;");
    ASSERT_TRUE(res.ToString() == U"0.2") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, unary_plus1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+x;");
    ASSERT_TRUE(res.ToString() == U"x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, unary_minus1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-x;");
    ASSERT_TRUE(res.ToString() == U"-x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, addition1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 1;");
    ASSERT_TRUE(res.ToString() == U"1+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, subtraction1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x - 1;");
    ASSERT_TRUE(res.ToString() == U"-1+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, multiplication1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x * 2;");
    ASSERT_TRUE(res.ToString() == U"2*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, division1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x / 3;");
    ASSERT_TRUE(res.ToString() == U"(1/3)*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, mixed_arithmetic1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x + 2*x;");
    ASSERT_TRUE(res.ToString() == U"3*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, power1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x, 2);");
    ASSERT_TRUE(res.ToString() == U"pow(x,2)") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x + 1, 2);");
    ASSERT_TRUE(res.ToString() == U"pow(1+x,2)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, sin1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x);");
    ASSERT_TRUE(res.ToString() == U"sin(x)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, cos1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"cos(x);");
    ASSERT_TRUE(res.ToString() == U"cos(x)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, log1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2, 8);");
    ASSERT_TRUE(res.ToStdString().find("log(8)") != std::string::npos) << res.ToStdString();
}

TEST_F(CalcTestSymbolic, root1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToString() == U"2") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, expand1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow(x + 1, 2));");
    ASSERT_TRUE(res.ToString() == U"1+2*x+pow(x,2)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, simplify1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x + x);");
    ASSERT_TRUE(res.ToString() == U"2*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, simplify2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"simplify(f(x));");
    ASSERT_TRUE(res.ToString() == U"2*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, diff1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(pow(x, 2), x);");
    ASSERT_TRUE(res.ToString() == U"2*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, diff2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"diff(f(x), x);");
    ASSERT_TRUE(res.ToString() == U"2") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, diff3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=pow(x,2);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"diff(f(x), x);");
    ASSERT_TRUE(res.ToString() == U"2*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, diff4)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"diff(sin(x), x);");
    ASSERT_TRUE(res.ToString() == U"cos(x)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, subs1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(x, 2), x, 5);");
    ASSERT_TRUE(res.ToString() == U"25") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, evalf1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);");
    Real r = Symbolic::to_real(res);
    std::string s = r.ToStdString(3, 3);
    ASSERT_TRUE(s == "3.142E+0") << s;
}

TEST_F(CalcTestSymbolic, evalf_precision1)
{
    Parser<Symbolic> hp_parser(50, Language::English);
    Symbolic res = hp_parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(pi);");
    std::string s = res.ToStdString();
    ASSERT_TRUE(s.length() > 15) << s;
    ASSERT_TRUE(s.find("3.14159265358979") != std::string::npos) << s;
}

TEST_F(CalcTestSymbolic, to_real1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3.5;");
    Real r = Symbolic::to_real(res);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.5E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestSymbolic, to_complex1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"4;");
    Complex c = Symbolic::to_complex(res);
    ASSERT_TRUE(c.ToStdString(3, 3) == "4.E+0") << c.ToStdString(3, 3);
}

TEST_F(CalcTestSymbolic, long_numbers1)
{
    Symbolic res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"123456789012345678901234567890;");
    std::string s = res.ToStdString();
    ASSERT_TRUE(s.length() > 20) << s;
    ASSERT_TRUE(s.find("123456789012345678901234567890") != std::string::npos) << s;
}

TEST_F(CalcTestSymbolic, member_checks1)
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

TEST_F(CalcTestSymbolic, logical_not)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"!x;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, logical_and)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x && y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, logical_or)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x || y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_eq)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x == y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_ne)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x != y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_lt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x < y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_le)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x <= y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_gt)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x > y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, relational_ge)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x >= y;"), SyntaxException);
}

TEST_F(CalcTestSymbolic, variables1)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v;") == parser.Parse(LogicalId{0, 0, 2}, U"555;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"v;").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"vp=x;", &dependencies);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;", &dependencies) == parser.Parse(LogicalId{0, 0, 3, 0}, U"x+5;")) <<
        parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;").ToStdString();
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestSymbolic, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString();
}

TEST_F(CalcTestSymbolic, variables3)
{
    parser.Parse(LogicalId{0, 0, 2}, U"a=77;");
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"a;") == parser.Parse(LogicalId{0, 0, 3}, U"77;")) << parser.Parse(LogicalId{0, 0, 3}, U"a;").ToStdString();
}

TEST_F(CalcTestSymbolic, variables4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d+5;") == parser.Parse(LogicalId{0, 0, 2}, U"9;")) << parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"d=45+d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;") == parser.Parse(LogicalId{0, 0, 4}, U"54;")) << parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString();
}

TEST_F(CalcTestSymbolic, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a*x;") == parser.Parse(LogicalId{0, 0, 2}, U"5*x;")) << parser.Parse(LogicalId{0, 0, 2}, U"a*x;").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"b=x+1;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a*b;") == parser.Parse(LogicalId{0, 0, 4}, U"5*(1+x);")) << parser.Parse(LogicalId{0, 0, 4}, U"a*b;").ToStdString();
}

TEST_F(CalcTestSymbolic, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a{1};") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;") == parser.Parse(LogicalId{0, 0, 4}, U"12;")) << parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString();
}

TEST_F(CalcTestSymbolic, variables7)
{
    //in Symbolic mode, unknown identifiers become symbols
    Symbolic res = parser.Parse(LogicalId{0, 0, 1}, U"sin;");
    ASSERT_TRUE(res.ToStdString() == "sin") << res.ToStdString();

    parser.Parse(LogicalId{0, 0, 1}, U"sin=2;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"sin;");
    ASSERT_TRUE(res.ToStdString() == "2") << res.ToStdString();

    res = parser.Parse(LogicalId{0, 0, 3}, U"sin(2);");
    ASSERT_TRUE(res.ToStdString() == "sin(2)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, variables8)
{
    parser.Parse(LogicalId{0, 0, 5}, U"b=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 1}, U"b=2;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "3") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "2") << res.ToStdString();

    parser.Parse(LogicalId{0, 0, 1}, U"b=5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "5") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "3") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, variables9)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v{12}=555;");
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"v{12};", &dependencies).ToStdString();
    ASSERT_TRUE(res == "555") << res;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"v{12}") != dependencies.end());
}

TEST_F(CalcTestSymbolic, variables10)
{
    yutovo_calculator::ParserContext parser_context;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    parser.Parse(LogicalId{0, 0, 2}, U"v;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestSymbolic, variables_rational)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=1/2;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"a+x;");
    ASSERT_TRUE(res.ToStdString() == "1/2+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, variables_complex)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=1+2*i;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"a+x;");
    ASSERT_TRUE(res.ToStdString() == "1+2*i+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) <<
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString();
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) <<
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString();
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) <<
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestSymbolic, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString() == "5") << res.ToStdString();

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString() == "9") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "6") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "6") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "10") << res.ToStdString();
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString() == "20") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString() == "25") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString() == "4") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"y=x+1;");
    parser.Parse(LogicalId{0, 0, 2}, U"r=pow(y,2);");
    Symbolic res = parser.Parse(LogicalId{0, 0, 3}, U"r;");
    ASSERT_TRUE(res.ToStdString() == "pow(1+x,2)") << res.ToStdString();
    parser.Parse(LogicalId{0, 0, 4}, U"f(t)=t+r;");
    res = parser.Parse(LogicalId{0, 0, 5}, U"f(3);");
    ASSERT_TRUE(res.ToStdString() == "3+pow(1+x,2)") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions_rational)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1/2;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(1/3);");
    ASSERT_TRUE(res.ToStdString() == "5/6") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, user_functions_complex)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1+i;");
    Symbolic res = parser.Parse(LogicalId{0, 0, 2}, U"f(2*i);");
    ASSERT_TRUE(res.ToStdString() == "1+3*i") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, construct_real)
{
    Real re(53, 3.5f);
    Symbolic s(53, re);
    ASSERT_TRUE(s.ToStdString() == "3.5") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, construct_complex)
{
    Complex c(Real(53, 1.0f), Real(53, 2.0f));
    Symbolic s(53, c);
    ASSERT_TRUE(s.ToStdString() == "1.0+2.0*I") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, construct_rational)
{
    Rational r(U"3/4");
    Symbolic s(53, r);
    ASSERT_TRUE(s.ToStdString() == "3/4") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, construct_real_with_unit_throws)
{
    Real re(53, 1.5f);
    Unit u(U"m");
    re.SetUnit(u);
    ASSERT_THROW(Symbolic(53, re), ParserException);
}

TEST_F(CalcTestSymbolic, construct_rational_with_unit_throws)
{
    Unit u(U"m");
    Rational r(u);
    ASSERT_THROW(Symbolic(53, r), ParserException);
}

TEST_F(CalcTestSymbolic, assign_real)
{
    Symbolic s(53);
    s = Real(53, 2.5f);
    ASSERT_TRUE(s.ToStdString() == "2.5") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, assign_complex)
{
    Symbolic s(53);
    s = Complex(Real(53, 1.0f), Real(53, -1.0f));
    ASSERT_TRUE(s.ToStdString() == "1.0-1.0*I") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, assign_rational)
{
    Symbolic s(53);
    s = Rational(U"5/2");
    ASSERT_TRUE(s.ToStdString() == "5/2") << s.ToStdString();
}

TEST_F(CalcTestSymbolic, add_real)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"x;");
    Symbolic res = s + Real(53, 2.0f);
    ASSERT_TRUE(res.ToStdString() == "2.0+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, add_rational)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"x;");
    Symbolic res = s + Rational(U"1/2");
    ASSERT_TRUE(res.ToStdString() == "1/2+x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, mul_complex)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"x;");
    Symbolic res = s * Complex(Real(53, 1.0f), Real(53, 1.0f));
    ASSERT_TRUE(res.ToStdString() == "(1.0+1.0*I)*x") << res.ToStdString();
}

TEST_F(CalcTestSymbolic, to_rational1)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"1/3+1/6;");
    Rational r = Symbolic::to_rational(s);
    ASSERT_TRUE(r.ToString() == U"1/2") << ToBasicString(r.ToString());
}

TEST_F(CalcTestSymbolic, to_rational2)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"2;");
    Rational r = Symbolic::to_rational(s);
    ASSERT_TRUE(r.ToString() == U"2") << ToBasicString(r.ToString());
}

TEST_F(CalcTestSymbolic, to_rational_variable_throws)
{
    Symbolic s = parser.Parse(LogicalId{0, 0, 1}, U"x;");
    ASSERT_THROW(Symbolic::to_rational(s), ParserException);
}

}
