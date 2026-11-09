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
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"y"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10, Language::Russian);
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


TEST_F(CalcTestSymbolicReal, expand2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow((pow(x,3)+x+3),2));");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(!s.empty());
}


TEST_F(CalcTestSymbolicReal, expand3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow((x+1),3));");
    ASSERT_TRUE(res.ToStdString(10) == "1.+3.*x+3.*pow(x,2.)+pow(x,3.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]}]})r" ) << res.ToJson(10);
}


TEST_F(CalcTestSymbolicReal, expand4)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+y)*(x-y));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x,2.)-pow(y,2.)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":12,"symbol":"-"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"y"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})r" ) << res.ToJson(10);
}


TEST_F(CalcTestSymbolicReal, simplify1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x + x);");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 2}, U"simplify(f(x));");
    ASSERT_TRUE(res.ToString(10) == U"2.*x") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(sin(x),2)+pow(cos(x),2));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(cos(x),2.)+pow(sin(x),2.)") << res.ToStdString(10);
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


TEST_F(CalcTestSymbolicReal, simplify6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(x-1,2)/(x-1));");
    ASSERT_TRUE(res.ToString(10) == U"-1.+x") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(x,2)*pow(x,4);");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,6.)") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify8)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(pow(x,5))/(pow(x,2));");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,3.)") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify((pow(x,5))/(pow(x,2)));");
    ASSERT_TRUE(res.ToString(10) == U"pow(x,3.)") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify9)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(1-pow(cos(x),2));");
    ASSERT_TRUE(res.ToStdString(10) == "(-(-1.+cos(x)))*(1.+cos(x))") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify10)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow((x+y),2)-pow(x,2)-2*x*y);");
    ASSERT_TRUE(res.ToStdString(10) == "pow(y,2.)") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify11)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow((x+y),2)+pow((x+y),2));");
    ASSERT_TRUE(res.ToStdString(10) == "2.*pow(x+y,2.)") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, simplify_wrong_args)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"simplify();", 10);
    }
    catch (SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::WrongArgumentsCount &&
                    ex.pos == 0 && ex.size == 10) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
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
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(x,2)+pow(y,2), x, 3);");
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


TEST_F(CalcTestSymbolicReal, evalf4)
{
    Parser<Symbolic<Real>> parser10(10, Language::English);
    Symbolic<Real> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(100));");
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"2.6881171418"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"10"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"43"}]}]}]}]})r" ) << json;
}


TEST_F(CalcTestSymbolicReal, evalf5)
{
    Parser<Symbolic<Real>> parser10(10, Language::English);
    Symbolic<Real> res = parser10.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(exp(1000));");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"∞"}]}]})r" ) << res.ToJson(10);
}


TEST_F(CalcTestSymbolicReal, evalf6)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(x*sqrt(pow(10,9)+1)-x*sqrt(pow(10,9)));", 10);
    ASSERT_TRUE(res.ToStdString(10) == "0.0000158114*x") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, evalf7)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"evalf(sqrt((y)/(2))+y+3.4*y);", 10);
    ASSERT_TRUE(res.ToStdString(10) == "4.4*y+sqrt(0.5*y)") << res.ToStdString(10);
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
        "{\"type\":45,\"elements\":[{\"type\":16,\"elements\":[{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, abs1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"abs(-5.2);");
    ASSERT_TRUE(res.ToStdString(10) == "5.2") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, abs2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"abs(x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "abs(x)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"abs\"},{\"type\":19,\"symbol\":\"(\"},{\"type\":8,\"elements\":\"x\"},{\"type\":20,\"symbol\":\")\"}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, abs3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(abs(y),y,-3);");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, root2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(x, 3);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "pow(x,0.333)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":17,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"3\"}]},{\"type\":10,\"elements\":[]},{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"x\"}]}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, log_x_base)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(x,2);");
    ASSERT_TRUE(res.ToStdString(10) == "log2./log(x)") << res.ToStdString(10);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
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
        R"r({"type":45,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"4"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10, Language::Russian);
}


TEST_F(CalcTestSymbolicReal, exp1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"exp(0);");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"1\"}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, exp2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"exp(x);");
    ASSERT_TRUE(res.ToStdString(10) == "exp(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"e"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"x"}]}]}]})r"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, ln2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(1);");
    ASSERT_TRUE(res.ToStdString(10) == "0.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"0\"}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, ln3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(10) == "log(x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, fact1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToStdString(10) == "120.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"120\"}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, fact2)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x!;");
    ASSERT_TRUE(res.ToStdString(10) == "gamma(1.+x)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"gamma"},{"type":19,"symbol":"("},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, fact3)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0!;");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToStdString(10) == "1.") << res.ToStdString(10);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10!;");
    ASSERT_TRUE(res.ToStdString(10) == "3628800") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, fact4)
{
    yutovo_calculator::ParserContext parser_context;
#ifdef NDEBUG
    parser_context.Init(1);
#else
    parser_context.Init(100);
#endif
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"100000!;", &parser_context),
        yutovo_calculator::TimeExceedException);
}


TEST_F(CalcTestSymbolicReal, gamma1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"gamma(5);");
    ASSERT_TRUE(res.ToStdString(10) == "24.") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":45,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"24\"}]}]}"
        ) << json;
}


TEST_F(CalcTestSymbolicReal, gamma_tojson)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"gamma(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":7,"elements":[{"type":8,"elements":"gamma"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}


TEST_F(CalcTestSymbolicReal, min_max1)
{
    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(x,y);");
    ASSERT_TRUE(res.ToStdString(10) == "min(x,y)") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(x,y);");
    ASSERT_TRUE(res.ToStdString(10) == "max(x,y)") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(2,3);");
    ASSERT_TRUE(res.ToStdString(10) == "2.") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(2,3);");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, min_max2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"b=3;");

    Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 3}, U"min(a,b);");
    ASSERT_TRUE(res.ToStdString(10) == "3.") << res.ToStdString(10);

    res = parser.Parse(LogicalId{0, 0, 4}, U"max(a,b);");
    ASSERT_TRUE(res.ToStdString(10) == "5.") << res.ToStdString(10);
}


TEST_F(CalcTestSymbolicReal, all_symbolic_functions)
{
    struct Expect
    {
        const char* name;
        const char* alias;
        const char* numeric_expr;
        const char* numeric_result;
        const char* sym_result;
        const char* derivative_result;
    };
    const Expect funcs[] = 
        {
            //direct numeric, alias, symbolic form, derivative
            {"sin", nullptr, "sin(0)", "0.", "sin(x)", "cos(x)"},
            {"cos", nullptr, "cos(0)", "1.", "cos(x)", "-sin(x)"},
            {"tg", "tan", "tg(0)", "0.", "tan(x)", "1.+pow(tan(x),2.)"},
            {"cot", "ctg", "subs(cot(x),x,0)", "∞", "cot(x)", "diff(cot(x),x)"},
            {"sec", nullptr, "subs(sec(x),x,0)", "1.", "sec(x)", "diff(sec(x),x)"},
            {"csc", "cosec", "subs(csc(x),x,1)", "1.1883951058", "csc(x)", "diff(csc(x),x)"},
            {"arcsin", nullptr, "arcsin(0)", "0.", "asin(x)", "pow(sqrt(1.-pow(x,2.)),-1.)"},
            {"arccos", nullptr, "arccos(1)", "0.", "acos(x)", "-(pow(sqrt(1.-pow(x,2.)),-1.))"},
            {"arctg", "arctan", "arctg(0)", "0.", "atan(x)", "pow(1.+pow(x,2.),-1.)"},
            {"arcctg", "arccot", "subs(arcctg(x),x,1)", "0.7853981634", "atan(pow(x,-1.))", "-(pow(pow(x,2.),-1.))/(pow(pow(x,-1.),2.)+1.)"},
            {"arcsec", nullptr, "arcsec(1)", "0.", "acos(pow(x,-1.))", "(pow(pow(x,2.),-1.)/sqrt(-1.+pow(x,2.))/abs(x))*pow(x,2.)"},
            {"arccsc", "arccosec", "arccsc(1)", "1.5707963268", "asin(pow(x,-1.))", "(-(pow(pow(x,2.),-1.))/sqrt(-1.+pow(x,2.))/abs(x))*pow(x,2.)"},
            {"sinh", "sh", "sinh(0)", "0.", "sinh(x)", "cosh(x)"},
            {"cosh", "ch", "cosh(0)", "1.", "cosh(x)", "sinh(x)"},
            {"tanh", "th", "tanh(0)", "0.", "tanh(x)", "1.-pow(tanh(x),2.)"},
            {"coth", "cth", "subs(coth(x),x,1)", "1.3130352855", "coth(x)", "diff(coth(x),x)"},
            {"sech", "sch", "sech(0)", "1.", "sech(x)", "diff(sech(x),x)"},
            {"csch", "cosech", "subs(csch(x),x,1)", "0.8509181282", "csch(x)", "diff(csch(x),x)"},
            {"asinh", "arsinh", "asinh(0)", "0.", "asinh(x)", "diff(asinh(x),x)"},
            {"acosh", "arcosh", "acosh(1)", "0.", "acosh(x)", "diff(acosh(x),x)"},
            {"atanh", "artanh", "atanh(0)", "0.", "atanh(x)", "diff(atanh(x),x)"},
            {"acoth", "arcoth", "subs(acoth(x),x,2)", "0.5493061443", "acoth(x)", "diff(acoth(x),x)"},
            {"asech", "arsech", "asech(1)", "0.", "asech(x)", "diff(asech(x),x)"},
            {"acsch", "arcsch", "acsch(1)", "0.881373587", "acsch(x)", "diff(acsch(x),x)"},
        };

    for (const auto& f : funcs)
    {
        SCOPED_TRACE(f.name);
        std::u32string name = std::u32string(f.name, f.name + strlen(f.name));

        //numeric variant
        {
            Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1},
                std::u32string(f.numeric_expr, f.numeric_expr + strlen(f.numeric_expr)) + U";", 10);
            ASSERT_TRUE(res.ToStdString(10) == f.numeric_result) << res.ToStdString(10);
        }

        //symbolic variant
        {
            Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, name + U"(x);", 10);
            ASSERT_TRUE(res.ToStdString(10) == f.sym_result) << res.ToStdString(10);
        }

        //derivative variant
        {
            Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"derivative(" + name + U"(x),x);", 10);
            ASSERT_TRUE(res.ToStdString(10) == f.derivative_result) << res.ToStdString(10);
        }

        //alias variant
        if (f.alias)
        {
            std::u32string alias = std::u32string(f.alias, f.alias + strlen(f.alias));
            Symbolic<Real> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, alias + U"(x);", 10);
            ASSERT_TRUE(res.ToStdString(10) == f.sym_result) << res.ToStdString(10);
        }
    }
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
