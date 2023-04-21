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
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=3/2;п;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3/2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=3/2;п;").ToStdString();
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"пр=3/2;пр+5;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"13/2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"пр=3/2;пр+5;").ToStdString();
}

TEST_F(CalcTestRational, symbols1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•;").ToStdString();
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣;").ToStdString();
}

TEST_F(CalcTestRational, error1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"123+(2/3)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 3}, U"123+3;45"), yutovo_calculator::SyntaxException);
}

}
