#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestReal, numbers1)
{
    ASSERT_TRUE(parser.Parse(U"1;").ToStdString(3, 3) == parser.Parse(U"1;").ToStdString(3, 3)) << parser.Parse(U"1;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"0.2;").ToStdString(3, 3) == parser.Parse(U"0.2;").ToStdString(3, 3)) << parser.Parse(U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"0.2E+0;").ToStdString(3, 3) == parser.Parse(U"0.2;").ToStdString(3, 3)) << parser.Parse(U"0.2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions1)
{
    ASSERT_TRUE(parser.Parse(U"sin(1);").ToStdString(3, 3) == parser.Parse(U"0.841E+0;").ToStdString(3, 3)) << parser.Parse(U"sin(1);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(U"ln(4);").ToStdString(3, 3) == parser.Parse(U"1.386E+0;").ToStdString(3, 3)) << parser.Parse(U"ln(4);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(U"lg(4);").ToStdString(3, 3) == parser.Parse(U"0.602E+0;").ToStdString(3, 3)) << parser.Parse(U"lg(4);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(U"log(2, 4);").ToStdString(3, 3) == parser.Parse(U"2.0E+0;").ToStdString(3, 3)) << parser.Parse(U"log(2, 4);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"log(2, 3);").ToStdString(3, 3) == parser.Parse(U"1.585E+0;").ToStdString(3, 3)) << parser.Parse(U"log(2, 3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"log%2,3;").ToStdString(3, 3) == parser.Parse(U"1.585E+0;").ToStdString(3, 3)) << parser.Parse(U"log%2,3;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"log%2,1+3;").ToStdString(3, 3) == parser.Parse(U"3.E+0;").ToStdString(3, 3)) << parser.Parse(U"log%2,1+3;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"log%2,(1+3);").ToStdString(3, 3) == parser.Parse(U"2.E+0;").ToStdString(3, 3)) << parser.Parse(U"log%2,(1+3);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions1)
{
    ASSERT_TRUE(parser.Parse(U"f(x)=5;") == Real());
    ASSERT_TRUE(parser.Parse(U"f(x)=x+5;f(2);") == parser.Parse(U"7;"));
}

TEST_F(CalcTestReal, str1)
{
    EXPECT_THROW(parser.Parse(U"в;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, str2)
{
    EXPECT_THROW(parser.Parse(U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(U"33t;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, variables1)
{
    std::vector<std::u32string> dependencies;
    ASSERT_TRUE(parser.Parse(U"v=555;v;") == parser.Parse(U"555;")) << parser.Parse(U"v=555;v;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"v=55.5;") == parser.Parse(U"v=55.5;")) << parser.Parse(U"v=55.5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"v=55.5;v;") == parser.Parse(U"55.5;")) << parser.Parse(U"v=55.5;v;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"vp=55.5;vp+5;", dependencies) == parser.Parse(U"60.5;")) << parser.Parse(U"vp=55.5;vp+5;").ToStdString(3, 3);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestReal, variables2)
{
    ASSERT_TRUE(parser.Parse(U"п=555;п;") == parser.Parse(U"555;")) << parser.Parse(U"п=555;п;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"п=55.5;п;") == parser.Parse(U"55.5;")) << parser.Parse(U"п=55.5;п;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(U"пр=55.5;пр+5;") == parser.Parse(U"60.5;")) << parser.Parse(U"пр=55.5;п+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, symbols1)
{
    EXPECT_THROW(parser.Parse(U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(U"•;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(U"‣;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, error1)
{
    EXPECT_THROW(parser.Parse(U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(U"123+sin(2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(U"123+3;45"), yutovo_calculator::SyntaxException);
}

}
