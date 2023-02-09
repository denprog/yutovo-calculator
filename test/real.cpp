#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, numbers1)
{
    ASSERT_TRUE(parser.Parse("1").ToString(3, 3) == parser.Parse("1").ToString(3, 3)) << parser.Parse("1").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("0.2").ToString(3, 3) == parser.Parse("0.2").ToString(3, 3)) << parser.Parse("0.2").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("0.2E+0").ToString(3, 3) == parser.Parse("0.2").ToString(3, 3)) << parser.Parse("0.2").ToString(3, 3);
}

TEST_F(CalcTestReal, functions1)
{
    ASSERT_TRUE(parser.Parse("sin(1)").ToString(3, 3) == parser.Parse("0.841E+0").ToString(3, 3)) << parser.Parse("sin(1)").ToString(3, 3);

    ASSERT_TRUE(parser.Parse("ln(4)").ToString(3, 3) == parser.Parse("1.386E+0").ToString(3, 3)) << parser.Parse("ln(4)").ToString(3, 3);

    ASSERT_TRUE(parser.Parse("lg(4)").ToString(3, 3) == parser.Parse("0.602E+0").ToString(3, 3)) << parser.Parse("lg(4)").ToString(3, 3);

    ASSERT_TRUE(parser.Parse("log(2, 4)").ToString(3, 3) == parser.Parse("2.0E+0").ToString(3, 3)) << parser.Parse("log(2, 4)").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("log(2, 3)").ToString(3, 3) == parser.Parse("1.585E+0").ToString(3, 3)) << parser.Parse("log(2, 3)").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("log%2,3;").ToString(3, 3) == parser.Parse("1.585E+0;").ToString(3, 3)) << parser.Parse("log%2,3;").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("log%2,1+3").ToString(3, 3) == parser.Parse("3.E+0").ToString(3, 3)) << parser.Parse("log%2,1+3").ToString(3, 3);
    ASSERT_TRUE(parser.Parse("log%2,(1+3)").ToString(3, 3) == parser.Parse("2.E+0").ToString(3, 3)) << parser.Parse("log%2,(1+3)").ToString(3, 3);
}

TEST_F(CalcTestReal, user_functions1)
{
    ASSERT_TRUE(parser.Parse("f(x)=5;") == Real());
    ASSERT_TRUE(parser.Parse("f(x)=x+5;f(2)") == parser.Parse("7"));
}

}
