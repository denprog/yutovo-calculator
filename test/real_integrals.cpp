/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, definite_integral1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.5E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(-1,4,sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.1939459267E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_alias)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.5E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,pi,sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "2.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(1,2,1/x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.6931471806E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral5)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,100,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "5000.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral6)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(2,2,pow(x,3),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral7)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,3,2*pow(x,2)+3*x+1,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "34.5E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral8)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,pi/2,cos(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral9)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,exp(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.7182818285E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral10)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(1,exp(1),ln(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral11)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,10,pow(x,4),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "20000.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral12)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,pow(x,2)*sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.2232442755E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral13)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,1,(1)/(x+i),x);", 10);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnknownIdentifier) << ex.ex_id;
        ASSERT_TRUE(ex.pos == 29) << ex.pos;
        ASSERT_TRUE(ex.size == 1) << ex.size;
    }
}

TEST_F(CalcTestReal, definite_integral14)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(-2,-1,sqrt(x),x);", 10), yutovo_calculator::ParserException);
}

TEST_F(CalcTestReal, definite_integral_unit_limits1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0mm,1mm,5,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.005E+0(m)") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_unit_limits2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0s,2s,x,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "2.E+0(s^2)") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_unit_limits3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0mm,1cm,5,x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "0.05E+0(m)") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_unit_limits_incompatible)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0m,1kg,5,x);", 10);
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE(ex.ex_id == ParserExceptionCode::UnitsAreIncompatible) << ex.ex_id;
    }
}

TEST_F(CalcTestReal, definite_integral_inf1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,∞,exp(-x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_inf2)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,∞,1/(1+pow(x,2)),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.5707963268E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_inf3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,∞,1/pow((x+1),2),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_inf4)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(1,∞,1/pow(x,2),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_inf5)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(0,∞,x*exp(-x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "1.E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, definite_integral_wide_interval)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"definite_integral(-3,33,pow(x,2)*sin(x),x);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "72.649339474E+0") << res.ToStdString(10, 10);
}

TEST_F(CalcTestReal, threads_integrals)
{
    constexpr int threads_count = 50;
    constexpr int iterations = 20;

    Parser<Real> expected_parser(3, Language::English);
    std::vector<std::string> expected1(threads_count);
    std::vector<std::string> expected2(threads_count);
    std::vector<std::string> expected3(threads_count);
    std::vector<std::string> expected4(threads_count);
    for (int i = 0; i < threads_count; ++i)
    {
        std::u32string coeff = ToUtfString(std::to_string(i));
        expected1[i] = expected_parser.Parse(LogicalId{0, 0, 0, 0, 1}, coeff + U"/2;", 10).ToStdString(10, 10);
        expected2[i] = expected_parser.Parse(LogicalId{0, 0, 0, 0, 1}, coeff + U"/4;", 10).ToStdString(10, 10);
        expected3[i] = expected_parser.Parse(LogicalId{0, 0, 0, 0, 1}, coeff + U"/8;", 10).ToStdString(10, 10);
        expected4[i] = expected_parser.Parse(LogicalId{0, 0, 0, 0, 1}, coeff + U"/16;", 10).ToStdString(10, 10);
    }

    std::vector<std::vector<std::string>> actual1(iterations, std::vector<std::string>(threads_count));
    std::vector<std::vector<std::string>> actual2(iterations, std::vector<std::string>(threads_count));
    std::vector<std::vector<std::string>> actual3(iterations, std::vector<std::string>(threads_count));
    std::vector<std::vector<std::string>> actual4(iterations, std::vector<std::string>(threads_count));
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
                Parser<Real> p(3, Language::English);
                std::u32string coeff = ToUtfString(std::to_string(i));
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

                    auto res1 = p.Parse(LogicalId{0, 0, 1}, U"definite_integral(0,1," + coeff + U"*x,x);", 10);
                    actual1[iter][i] = res1.ToStdString(10, 10);

                    auto res2 = p.Parse(LogicalId{0, 0, 2}, U"definite_integral(0,1," + coeff + U"*pow(x,3),x);", 10);
                    actual2[iter][i] = res2.ToStdString(10, 10);

                    auto res3 = p.Parse(LogicalId{0, 0, 3}, U"definite_integral(0,1," + coeff + U"*pow(x,7),x);", 10);
                    actual3[iter][i] = res3.ToStdString(10, 10);

                    auto res4 = p.Parse(LogicalId{0, 0, 4}, U"definite_integral(0,1," + coeff + U"*pow(x,15),x);", 10);
                    actual4[iter][i] = res4.ToStdString(10, 10);

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
            ASSERT_TRUE(!expected3[i].empty());
            ASSERT_TRUE(actual3[iter][i] == expected3[i]) << "iter=" << iter << " i=" << i << " actual=" << actual3[iter][i] << " expected=" << expected3[i];
            ASSERT_TRUE(!expected4[i].empty());
            ASSERT_TRUE(actual4[iter][i] == expected4[i]) << "iter=" << iter << " i=" << i << " actual=" << actual4[iter][i] << " expected=" << expected4[i];
        }
        actual1[iter].clear();
        actual2[iter].clear();
        actual3[iter].clear();
        actual4[iter].clear();
    }

    {
        std::lock_guard<std::mutex> lock(m);
        current_iter = iterations;
    }
    cv_start.notify_all();

    for (auto& t : threads)
        t.join();
}

TEST_F(CalcTestReal, definite_integral_user_function)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"iff(t)=5*pow(e,-2t);");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"definite_integral(0,1,iff(t),t);", 10);
    ASSERT_TRUE(res.ToStdString(10, 10) == "2.1616617919E+0") << res.ToStdString(10, 10);
}

}
