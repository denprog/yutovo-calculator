#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, integers1)
{
    ASSERT_TRUE(parser.Parse("1+1") == parser.Parse("2"));
}

TEST_F(CalcTestInteger, variables1)
{
    ASSERT_TRUE(parser.Parse("a=5;a") == parser.Parse("5"));
    ASSERT_TRUE(parser.Parse("a=5+6;b=a+7;b+3") == parser.Parse("21"));
}

TEST_F(CalcTestInteger, functions1)
{
    ASSERT_TRUE(parser.Parse("f(x)=x+5;f(2)") == parser.Parse("7"));
    ASSERT_TRUE(parser.Parse("a=5+6;f(t)=a*t;f(2)") == parser.Parse("22"));
}

}
