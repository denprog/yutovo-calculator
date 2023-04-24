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
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"a=5;");
    parser.Parse(ElementId{0, 0, 0, 0, 2}, U"a=5+6;");
    parser.Parse(ElementId{0, 0, 0, 1, 0}, U"b=a+7;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1, 1, 0}, U"b+3;") == parser.Parse(ElementId{0, 0, 1, 1, 0}, U"21;"));
}

TEST_F(CalcTestInteger, variables2)
{
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=55;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"п;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"55;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п;").ToStdString();
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"пр=55;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"пр+5;") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"60;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"п+5;").ToStdString();
    parser.Parse(ElementId{0, 0, 0, 0, 3}, U"fп=55;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 4}, U"fп+5;") == parser.Parse(ElementId{0, 0, 0, 0, 3}, U"60;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 4}, U"fп+5;").ToStdString();
}

TEST_F(CalcTestInteger, functions1)
{
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2, 0}, U"f(2);") == parser.Parse(ElementId{0, 0, 0, 0, 2, 0}, U"7;"));
    parser.Parse(ElementId{0, 0, 0, 0, 2}, U"a=5+6;");
    parser.Parse(ElementId{0, 0, 0, 2, 3, 0}, U"f(t)=a*t;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"f(2);") == parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"22;")) << 
        parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"f(2);").ToStdString();
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
