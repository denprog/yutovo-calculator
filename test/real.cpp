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
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"0.2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"0.2E+0;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"sin(1);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0.841E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"sin(1);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"ln(4);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1.386E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"ln(4);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"lg(4);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0.602E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"lg(4);").ToStdString(3, 3);

    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"log(2, 4);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"2.0E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"log(2, 4);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"log(2, 3);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"log(2, 3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"log%2,3;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"log%2,3;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"log%2,1+3;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"3.E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"log%2,1+3;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"log%2,(1+3);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"2.E+0;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 1}, U"log%2,(1+3);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions1)
{
    parser.Parse(ElementId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(ElementId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3}, U"f(2);") == parser.Parse(ElementId{0, 0, 3}, U"7;")) << 
        parser.Parse(ElementId{0, 0, 3}, U"f(2);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions2)
{
    parser.Parse(ElementId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"f(2);") == parser.Parse(ElementId{0, 0, 2}, U"2;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"f(2);").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"f(2);") == parser.Parse(ElementId{0, 0, 4}, U"9;")) << 
        parser.Parse(ElementId{0, 0, 4}, U"f(2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"f(3);") == parser.Parse(ElementId{0, 0, 2}, U"3;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"f(3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"f(3);") == parser.Parse(ElementId{0, 0, 4}, U"10;")) << 
        parser.Parse(ElementId{0, 0, 4}, U"f(10);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, str1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"в;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, str2)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"33t;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, variables1)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(ElementId{0, 0, 1}, U"v=555;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"v;") == parser.Parse(ElementId{0, 0, 2}, U"555;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"v;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1}, U"v=55.5;") == parser.Parse(ElementId{0, 0, 1}, U"v=55.5;")) << 
        parser.Parse(ElementId{0, 0, 1}, U"v=55.5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"v=55.5;v;") == parser.Parse(ElementId{0, 0, 2}, U"55.5;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"v=55.5;v;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"vp=55.5;", dependencies);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3, 0}, U"vp+5;", dependencies) == parser.Parse(ElementId{0, 0, 3, 0}, U"60.5;")) << 
        parser.Parse(ElementId{0, 0, 3, 0}, U"vp+5;").ToStdString(3, 3);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestReal, variables2)
{
    parser.Parse(ElementId{0, 0, 1}, U"п=555;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"п;") == parser.Parse(ElementId{0, 0, 1}, U"555;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"п;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"п=55.5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3, 3}, U"п;") == parser.Parse(ElementId{0, 0, 3, 3}, U"55.5;")) << 
        parser.Parse(ElementId{0, 0, 3, 3}, U"п;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3, 3, 0}, U"пр=55.5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3, 3, 1}, U"пр+5;") == parser.Parse(ElementId{0, 0, 1}, U"60.5;")) << 
        parser.Parse(ElementId{0, 0, 3, 3, 1}, U"пр+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables3)
{
    parser.Parse(ElementId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"a;") == parser.Parse(ElementId{0, 0, 2}, U"5;")) << parser.Parse(ElementId{0, 0, 2}, U"a;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"a;") == parser.Parse(ElementId{0, 0, 4}, U"7;")) << parser.Parse(ElementId{0, 0, 4}, U"a;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"a+1;") == parser.Parse(ElementId{0, 0, 2}, U"6;")) << parser.Parse(ElementId{0, 0, 2}, U"a+1;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables4)
{
    parser.Parse(ElementId{0, 0, 2}, U"a=77;");
    parser.Parse(ElementId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3}, U"a;") == parser.Parse(ElementId{0, 0, 3}, U"77;")) << parser.Parse(ElementId{0, 0, 3}, U"a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables5)
{
    parser.Parse(ElementId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"d+5;") == parser.Parse(ElementId{0, 0, 2}, U"9;")) << parser.Parse(ElementId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"d=45+d;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"d+5;") == parser.Parse(ElementId{0, 0, 4}, U"54;")) << parser.Parse(ElementId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, symbols1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 1}, U"•;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 1}, U"‣;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, errors1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"123+sin(2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"123+3;45"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, errors2)
{
    try
    {
        parser.Parse(ElementId{0, 0, 1}, U"d=4+f;");
        parser.Parse(ElementId{0, 0, 2}, U"d+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.id == MakeElementId(ElementId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << ElementIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors3)
{
    try
    {
        parser.Parse(ElementId{0, 0, 1}, U"5+d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.id == MakeElementId(ElementId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2) << ElementIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors4)
{
    try
    {
        parser.Parse(ElementId{0, 0, 1}, U"d=5+d;");
        parser.Parse(ElementId{0, 0, 2}, U"33455-d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.id == MakeElementId(ElementId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << ElementIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors5)
{
    try
    {
        parser.Parse(ElementId{0, 0, 1}, U"sqrt();");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.id == MakeElementId(ElementId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::WrongArgumentsCount && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

}
