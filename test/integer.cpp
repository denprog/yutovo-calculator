#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, integers1)
{
    ASSERT_TRUE(parser.Parse(U"1+1") == parser.Parse(U"2"));
}

TEST_F(CalcTestInteger, variables1)
{
    ASSERT_TRUE(parser.Parse(U"a=5;a") == parser.Parse(U"5"));
    ASSERT_TRUE(parser.Parse(U"a=5+6;b=a+7;b+3") == parser.Parse(U"21"));
}

TEST_F(CalcTestInteger, variables2)
{
    ASSERT_TRUE(parser.Parse(U"п=55;п") == parser.Parse(U"55")) << parser.Parse(U"п=55;п").ToStdString();
    ASSERT_TRUE(parser.Parse(U"пр=55;пр+5") == parser.Parse(U"60")) << parser.Parse(U"пр=55;п+5").ToStdString();
    ASSERT_TRUE(parser.Parse(U"fп=55;fп+5") == parser.Parse(U"60")) << parser.Parse(U"fп=55;fп+5").ToStdString();
}

TEST_F(CalcTestInteger, functions1)
{
    ASSERT_TRUE(parser.Parse(U"f(x)=x+5;f(2)") == parser.Parse(U"7"));
    ASSERT_TRUE(parser.Parse(U"a=5+6;f(t)=a*t;f(2)") == parser.Parse(U"22"));
}

TEST_F(CalcTestInteger, symbols1)
{
    EXPECT_THROW(parser.Parse(U"•"), yutovo_calculator::SyntaxException) << parser.Parse(U"•").ToStdString();
    EXPECT_THROW(parser.Parse(U"‣"), yutovo_calculator::SyntaxException) << parser.Parse(U"‣").ToStdString();
}

}
