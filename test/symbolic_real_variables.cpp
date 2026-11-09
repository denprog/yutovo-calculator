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

TEST_F(CalcTestSymbolicReal, underscore_variables)
{
    //giac does not accept a lone underscore or a double underscore as a free identifier: the value degrades to nan
    Symbolic<Real> power1 = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(_,2);", 10);
    ASSERT_TRUE(power1.ToStdString(10) == "nan") << power1.ToStdString(10);
    Symbolic<Real> power4 = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"pow(__,2);", 10);
    ASSERT_TRUE(power4.ToStdString(10) == "nan") << power4.ToStdString(10);
    //giac evaluates identifiers starting with an underscore instead of keeping them symbolic
    Symbolic<Real> power2 = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"pow(_a,2);", 10);
    ASSERT_TRUE(power2.ToStdString(10) == "1.") << power2.ToStdString(10);
    Symbolic<Real> power3 = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"pow(_a_,2);", 10);
    ASSERT_TRUE(power3.ToStdString(10) == "1.") << power3.ToStdString(10);
    //a trailing underscore stays in the name: the power base must not lose it (no empty brackets)
    Symbolic<Real> power5 = parser.Parse(LogicalId{0, 0, 0, 0, 5}, U"pow(a_,2);", 10);
    ASSERT_TRUE(power5.ToStdString(10) == "pow(a_,2.)") << power5.ToStdString(10);
    ASSERT_TRUE(power5.ToJson(10) ==
        R"r({"type":45,"elements":[{"type":15,"elements":[{"type":7,"elements":[{"type":8,"elements":"a_"}]},{"type":10,"elements":[]},{"type":7,"elements":[{"type":8,"elements":"2"}]}]}]})r") << power5.ToJson(10);

    //all underscore names work as defined variables: values are stored by the calculator itself
    parser.Parse(LogicalId{0, 0, 0, 0, 6}, U"_=1;");
    parser.Parse(LogicalId{0, 0, 0, 0, 7}, U"_a=2;");
    parser.Parse(LogicalId{0, 0, 0, 0, 8}, U"_a_=3;");
    parser.Parse(LogicalId{0, 0, 0, 0, 9}, U"__=4;");
    parser.Parse(LogicalId{0, 0, 0, 0, 10}, U"a_=5;");
    Symbolic<Real> sum = parser.Parse(LogicalId{0, 0, 0, 0, 11}, U"_+_a+_a_+__+a_;", 10);
    ASSERT_TRUE(sum.ToStdString(10) == "15.") << sum.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, subscript1)
{
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript2)
{
    //text subscript
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{us};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript3)
{
    //multi-character subscript
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{ab};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript4)
{
    //expression with multiple variables, one subscripted
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"x{2}+y;"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript5)
{
    //subscript in implicit multiplication
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2x{2};"), SyntaxException);
}

TEST_F(CalcTestSymbolicReal, subscript6)
{
    //subscript as function argument
    ASSERT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"sin(x{2});"), SyntaxException);
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

//Remove variable after error of its declaring, a removed variable becomes a plain symbol again
TEST_F(CalcTestSymbolicReal, variables12)
{
    parser.Parse(LogicalId{0, 0, 1}, U"b=55;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"b;") == parser.Parse(LogicalId{0, 0, 2}, U"55;")) <<
        parser.Parse(LogicalId{0, 0, 2}, U"b;").ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=sin();"), yutovo_calculator::SyntaxException);
    Symbolic res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=(3)/();"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 3}, U"b=;"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(10) == "b") << res.ToStdString(10);
}

TEST_F(CalcTestSymbolicReal, threads_variables)
{
    constexpr int threads_count = 50;
    constexpr int iterations = 20;

    auto format_number =
        [](double v)
        {
            std::ostringstream s;
            s << std::setprecision(10) << std::fixed << v;
            std::string str = s.str();
            if (str.find('.') != std::string::npos)
            {
                str.erase(str.find_last_not_of('0') + 1);
                if (str.back() == '.')
                    str.pop_back();
            }
            return str;
        };

    std::vector<std::string> expected1(threads_count);
    std::vector<std::string> expected2(threads_count);
    for (int i = 0; i < threads_count; ++i)
    {
        double a = 2.3 + i;
        double b = 3.4 + i;
        double c = 6.7 + i;
        double d = 0.5 + i;
        double e = 1.5 + i;
        double f = 2.5 + i;
        expected1[i] = format_number(a + b * c + d + e * f) + "+sin(pow(x,2.))";

        double g = 0.25 + i;
        double h = 1.25 + i;
        double j = 2.25 + i;
        expected2[i] = format_number(g + h * j) + "+cos(pow(y,2.))";
    }

    std::vector<std::vector<std::string>> actual1(iterations, std::vector<std::string>(threads_count));
    std::vector<std::vector<std::string>> actual2(iterations, std::vector<std::string>(threads_count));
    std::vector<std::thread> threads;
    threads.reserve(threads_count);

    std::mutex m;
    std::condition_variable cv_start;
    std::condition_variable cv_done;
    int current_iter = -1;
    int done_count = 0;

    for (int i = 0; i < threads_count; ++i)
    {
        threads.emplace_back(
            [&, i]()
            {
                Parser<Symbolic<Real>> p(3, Language::English);
                int iter = -1;
                while (true)
                {
                    std::unique_lock<std::mutex> lock(m);
                    cv_start.wait(lock,
                        [&]()
                        {
                            return current_iter != iter;
                        });
                    iter = current_iter;
                    if (iter >= iterations)
                        break;
                    lock.unlock();

                    p.Parse(LogicalId{0, 0, 1}, U"a=" + ToUtfString(std::to_string(2.3 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 2}, U"b=" + ToUtfString(std::to_string(3.4 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 3}, U"c=" + ToUtfString(std::to_string(6.7 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 4}, U"d=" + ToUtfString(std::to_string(0.5 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 5}, U"e=" + ToUtfString(std::to_string(1.5 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 6}, U"f=" + ToUtfString(std::to_string(2.5 + i)) + U";", 3);
                    Symbolic<Real> res1 = p.Parse(LogicalId{0, 0, 7}, U"a+b*c+d+e*f+sin(pow(x,2));", 10);
                    actual1[iter][i] = res1.ToStdString(10);

                    p.Parse(LogicalId{0, 0, 8}, U"g=" + ToUtfString(std::to_string(0.25 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 9}, U"h=" + ToUtfString(std::to_string(1.25 + i)) + U";", 3);
                    p.Parse(LogicalId{0, 0, 10}, U"j=" + ToUtfString(std::to_string(2.25 + i)) + U";", 3);
                    Symbolic<Real> res2 = p.Parse(LogicalId{0, 0, 11}, U"g+h*j+cos(pow(y,2));", 10);
                    actual2[iter][i] = res2.ToStdString(10);

                    lock.lock();
                    ++done_count;
                    if (done_count == threads_count)
                        cv_done.notify_one();
                }
            });
    }

    for (int iter = 0; iter < iterations; ++iter)
    {
        {
            std::lock_guard<std::mutex> lock(m);
            current_iter = iter;
            done_count = 0;
        }
        cv_start.notify_all();

        std::unique_lock<std::mutex> lock(m);
        cv_done.wait(lock,
            [&]()
            {
                return done_count == threads_count;
            });
        lock.unlock();

        for (int i = 0; i < threads_count; ++i)
        {
            ASSERT_TRUE(!expected1[i].empty());
            ASSERT_TRUE(actual1[iter][i] == expected1[i]) << "iter=" << iter << " i=" << i << " actual=" << actual1[iter][i] << " expected=" << expected1[i];
            ASSERT_TRUE(!expected2[i].empty());
            ASSERT_TRUE(actual2[iter][i] == expected2[i]) << "iter=" << iter << " i=" << i << " actual=" << actual2[iter][i] << " expected=" << expected2[i];
        }
        actual1[iter].clear();
        actual2[iter].clear();
    }

    {
        std::lock_guard<std::mutex> lock(m);
        current_iter = iterations;
    }
    cv_start.notify_all();

    for (auto& t : threads)
        t.join();
}

}
