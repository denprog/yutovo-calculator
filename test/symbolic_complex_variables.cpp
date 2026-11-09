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

TEST_F(CalcTestSymbolicComplex, subscript1)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, subscript2)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{us};"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, subscript3)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{ab};"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, subscript4)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2}+y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, subscript5)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, subscript6)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x{2});"), SyntaxException);
}

TEST_F(CalcTestSymbolicComplex, underscore_variables)
{
    //giac does not accept a lone underscore or a double underscore as a free identifier: the value degrades to nan
    Symbolic<Complex> power1 = parser.Parse(LogicalId{0, 0, 1}, U"pow(_,2);", 10);
    ASSERT_TRUE(power1.ToStdString(10) == "nan") << power1.ToStdString(10);
    Symbolic<Complex> power4 = parser.Parse(LogicalId{0, 0, 4}, U"pow(__,2);", 10);
    ASSERT_TRUE(power4.ToStdString(10) == "nan") << power4.ToStdString(10);
    //giac evaluates identifiers starting with an underscore instead of keeping them symbolic
    Symbolic<Complex> power2 = parser.Parse(LogicalId{0, 0, 2}, U"pow(_a,2);", 10);
    ASSERT_TRUE(power2.ToStdString(10) == "1") << power2.ToStdString(10);
    Symbolic<Complex> power3 = parser.Parse(LogicalId{0, 0, 3}, U"pow(_a_,2);", 10);
    ASSERT_TRUE(power3.ToStdString(10) == "1") << power3.ToStdString(10);
    //a trailing underscore stays in the name: the power base must not lose it (no empty brackets)
    Symbolic<Complex> power5 = parser.Parse(LogicalId{0, 0, 5}, U"pow(a_,2);", 10);
    ASSERT_TRUE(power5.ToStdString(10) == "pow(a_,2.)") << power5.ToStdString(10);
    ASSERT_TRUE(power5.ToJson(10) ==
        R"r({"type":47,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"a_"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]})r") << power5.ToJson(10);

    //all underscore names work as defined variables: values are stored by the calculator itself
    parser.Parse(LogicalId{0, 0, 6}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 7}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 8}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 9}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 10}, U"a_=5;");
    Symbolic<Complex> sum = parser.Parse(LogicalId{0, 0, 11}, U"_+_a+_a_+__+a_;");
    ASSERT_TRUE(sum.ToStdString(10) == "15") << sum.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, variables1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=2;");
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 2}, U"a*x;");
    ASSERT_TRUE(res.ToStdString(10) == "2.*x") << res.ToStdString(10);
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

//Remove variable after error of its declaring, a removed variable becomes a plain symbol again
TEST_F(CalcTestSymbolicComplex, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"b;") == parser.Parse(LogicalId{0, 0, 2}, U"55;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"b;").ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=sin();"), yutovo_calculator::SyntaxException);
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=(3)/();"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=;"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicComplex, long_number_with_symbol)
{
    Symbolic<Complex> res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"12345678901234567890.123*x;");
    ASSERT_TRUE(res.ToStdString(10) == "1.235E+19*x") << res.ToStdString(10);
}

}
