#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, integers1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1+1;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2;"));
}

TEST_F(CalcTestInteger, variables1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"a=5;a;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5;"));
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"a=5+6;b=a+7;b+3;") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"21;"));
}

TEST_F(CalcTestInteger, variables2)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=55;п;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"55;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=55;п;").ToStdString();
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"пр=55;пр+5;") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"60;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"пр=55;п+5;").ToStdString();
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 3}, U"fп=55;fп+5;") == parser.Parse(ElementId{0, 0, 0, 0, 3}, U"60;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 3}, U"fп=55;fп+5;").ToStdString();
}

TEST_F(CalcTestInteger, functions1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"f(x)=x+5;f(2);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"7;"));
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"a=5+6;f(t)=a*t;f(2);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"22;"));
}

TEST_F(CalcTestInteger, symbols1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•").ToStdString();
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣").ToStdString();
}

TEST_F(CalcTestInteger, str1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"33ttt;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, error1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"123+(24/2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 3}, U"123+3;45"), yutovo_calculator::SyntaxException);
}

}
