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

TEST_F(CalcTestSymbolicRational, expand1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow((x+1),2));");
    ASSERT_TRUE(res.ToStdString(0) == "1+2*x+pow(x,2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subs1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(x,2), x, 5);");
    ASSERT_TRUE(res.ToStdString(0) == "25") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subs2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(yy,3), yy, 5);");
    ASSERT_TRUE(res.ToStdString(0) == "125") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, subs3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x+y,y,2);");
    ASSERT_TRUE(res.ToStdString(0) == "2+x") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, subs4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(pow(x,2)+pow(y,2), x, 3);");
    ASSERT_TRUE(res.ToStdString(0) == "9+pow(y,2)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
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

TEST_F(CalcTestSymbolicRational, subs5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(x,2),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "∞") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, subs6)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(x+y,y,2);");
    ASSERT_TRUE(res.ToStdString(10) == "2+x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, subs7)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(2,x),x,1);");
    ASSERT_TRUE(res.ToStdString(10) == "0") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, subs8)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"subs(log(x,2),x,2);");
    ASSERT_TRUE(res.ToStdString(10) == "1") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+1;");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 2}, U"f(x);");
    ASSERT_TRUE(res.ToStdString(0) == "1+x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, log1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2, 8);");
    ASSERT_TRUE(res.ToStdString(0) == "log(8)/log(2)") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, log2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(x,y);");
    ASSERT_TRUE(res.ToStdString(0, Language::Russian) == "ln(y)/ln(x)") << res.ToStdString(0, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":46,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"y"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicRational, root1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(8, 3);");
    ASSERT_TRUE(res.ToStdString(0) == "2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, simplify1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(x+x);");
    ASSERT_TRUE(res.ToStdString(0) == "2*x") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, sqrt1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(4);");
    ASSERT_TRUE(res.ToStdString(0) == "2") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":8,"elements":"2"})"
            R"(]})"
        R"(]})" ) << json;

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(x);");
    ASSERT_TRUE(res.ToStdString(0) == "sqrt(x)") << res.ToStdString(0);
    json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":16,"elements":[)"
                R"({"type":10,"elements":[]},)"
                R"({"type":7,"elements":[)"
                    R"({"type":8,"elements":"x"})"
                R"(]})"
            R"(]})"
        R"(]})" ) << json;
}

TEST_F(CalcTestSymbolicRational, expand2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand((x+1)*(x-1));");
    ASSERT_TRUE(res.ToStdString(0) == "-1+pow(x,2)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[)"
            R"({"type":7,"elements":[)"
                R"({"type":12,"symbol":"-"},{"type":8,"elements":"1"},)"
                R"({"type":11,"symbol":"+"},)"
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

TEST_F(CalcTestSymbolicRational, log3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(8, 2);");
    std::string s = res.ToStdString(0);
    ASSERT_TRUE(s == "log(2)/log(8)") << s;
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[)r"
            R"r({"type":14,"elements":[)r"
                R"r({"type":7,"elements":[)r"
                    R"r({"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"})r"
                R"r(]},)r"
                R"r({"type":10,"elements":[]},)r"
                R"r({"type":7,"elements":[)r"
                    R"r({"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"8"},{"type":20,"symbol":")"})r"
                R"r(]})r"
            R"r(]})r"
        R"r(]})r" ) << json;
}

TEST_F(CalcTestSymbolicRational, expand3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"expand(pow((x+1),3));");
    ASSERT_TRUE(res.ToStdString(0) == "1+3*x+3*pow(x,2)+pow(x,3)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"3"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]},{"type":11,"symbol":"+"},{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"x"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"3"}]}]}]}]})" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, simplify2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x+x;");
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 2}, U"simplify(f(x));");
    ASSERT_TRUE(res.ToString(10) == U"2*x") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, simplify4)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(x+y,2));");
    ASSERT_TRUE(res.ToStdString(10) == "pow(x+y,2)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":46,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicRational, simplify5)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"simplify(pow(x+y,2)+pow(x+y,2));");
    ASSERT_TRUE(res.ToStdString(10) == "2*pow(x+y,2)") << res.ToStdString(10);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"json({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"2"},{"type":13,"symbol":"·"},{"type":15,"elements":[{"type":7,"elements":[{"type":7,"elements":[{"type":19,"symbol":"("},{"type":7,"elements":[{"type":8,"elements":"x"},{"type":11,"symbol":"+"},{"type":8,"elements":"y"}]},{"type":20,"symbol":")"}]}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]}]})json" ) << json;
}

TEST_F(CalcTestSymbolicRational, sqrt2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sqrt(x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "sqrt(x)") << s;
}

TEST_F(CalcTestSymbolicRational, abs1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"abs(-(1)/(2));");
    ASSERT_TRUE(res.ToStdString(10) == "1/2") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicRational, abs2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"abs(x);");
    std::string s = res.ToStdString(10);
    ASSERT_TRUE(s == "abs(x)") << s;
}

TEST_F(CalcTestSymbolicRational, log_x_base)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(x,2);");
    ASSERT_TRUE(res.ToStdString(0) == "log(2)/log(x)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, nested_functions_brackets)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"csc(coth(x));");
    ASSERT_TRUE(res.ToStdString(0) == "csc(coth(x))") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"csc"},{"type":19,"symbol":"("},{"type":8,"elements":"coth"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, ln_russian)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(0, Language::Russian) == "ln(x)") << res.ToStdString(0, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicRational, ln_in_expression_russian)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x)+1;");
    ASSERT_TRUE(res.ToStdString(0, Language::Russian) == "1+ln(x)") << res.ToStdString(0, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicRational, ln_english)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(0) == "log(x)") << res.ToStdString(0);
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, log_change_of_base_russian)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"log(2,4);");
    ASSERT_TRUE(res.ToStdString(0, Language::Russian) == "ln(4)/ln(2)") << res.ToStdString(0, Language::Russian);
    ASSERT_TRUE(res.ToJson(10, Language::Russian) ==
        R"r({"type":46,"elements":[{"type":14,"elements":[{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"4"},{"type":20,"symbol":")"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"ln"},{"type":19,"symbol":"("},{"type":8,"elements":"2"},{"type":20,"symbol":")"}]}]}]})r" ) << res.ToJson(10, Language::Russian);
}

TEST_F(CalcTestSymbolicRational, exp1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"exp(0);");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"1"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, exp2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"exp(x);");
    ASSERT_TRUE(res.ToStdString(0) == "exp(x)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"e"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"x"}]}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, ln2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(1);");
    ASSERT_TRUE(res.ToStdString(0) == "0") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"0"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, ln3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"ln(x);");
    ASSERT_TRUE(res.ToStdString(0) == "log(x)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"log"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, fact1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToStdString(0) == "120") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"120"}]}]})"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, fact2)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x!;");
    ASSERT_TRUE(res.ToStdString(0) == "gamma(1+x)") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"gamma"},{"type":19,"symbol":"("},{"type":8,"elements":"1"},{"type":11,"symbol":"+"},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, fact3)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0!;");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToStdString(0) == "1") << res.ToStdString(0);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10!;");
    ASSERT_TRUE(res.ToStdString(0) == "3628800") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, fact4)
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

TEST_F(CalcTestSymbolicRational, gamma1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"gamma(5);");
    ASSERT_TRUE(res.ToStdString(0) == "24") << res.ToStdString(0);
    std::string json = res.ToJson(10);
    ASSERT_TRUE(json ==
        "{\"type\":46,\"elements\":[{\"type\":7,\"elements\":[{\"type\":8,\"elements\":\"24\"}]}]}"
        ) << json;
}

TEST_F(CalcTestSymbolicRational, gamma_tojson)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"gamma(x);");
    ASSERT_TRUE(res.ToJson(10) ==
        R"r({"type":46,"elements":[{"type":7,"elements":[{"type":8,"elements":"gamma"},{"type":19,"symbol":"("},{"type":8,"elements":"x"},{"type":20,"symbol":")"}]}]})r" ) << res.ToJson(10);
}

TEST_F(CalcTestSymbolicRational, min_max1)
{
    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(x,y);");
    ASSERT_TRUE(res.ToStdString(0) == "min(x,y)") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(x,y);");
    ASSERT_TRUE(res.ToStdString(0) == "max(x,y)") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"min(1/2,1/3);");
    ASSERT_TRUE(res.ToStdString(0) == "1/3") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"max(1/2,1/3);");
    ASSERT_TRUE(res.ToStdString(0) == "1/2") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, min_max2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"b=3;");

    Symbolic<Rational> res = parser.Parse(LogicalId{0, 0, 3}, U"min(a,b);");
    ASSERT_TRUE(res.ToStdString(0) == "3") << res.ToStdString(0);

    res = parser.Parse(LogicalId{0, 0, 4}, U"max(a,b);");
    ASSERT_TRUE(res.ToStdString(0) == "5") << res.ToStdString(0);
}

TEST_F(CalcTestSymbolicRational, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sin(x;"), yutovo_calculator::SyntaxException);
}


TEST_F(CalcTestSymbolicRational, errors2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"sinx);"), yutovo_calculator::SyntaxException);
}


}
