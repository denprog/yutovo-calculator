#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, variables1)
{
    ASSERT_TRUE(parser.Parse(U"п=3/2;п") == parser.Parse(U"3/2")) << parser.Parse(U"п=3/2;п").ToStdString();
    ASSERT_TRUE(parser.Parse(U"пр=3/2;пр+5") == parser.Parse(U"13/2")) << parser.Parse(U"пр=3/2;пр+5").ToStdString();
}

TEST_F(CalcTestRational, symbols1)
{
    EXPECT_THROW(parser.Parse(U"•"), yutovo_calculator::SyntaxException) << parser.Parse(U"•").ToStdString();
    EXPECT_THROW(parser.Parse(U"‣"), yutovo_calculator::SyntaxException) << parser.Parse(U"‣").ToStdString();
}

}
