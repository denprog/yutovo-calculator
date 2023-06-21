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

TEST_F(CalcTestReal, numbers2)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.75;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers3)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1-2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1;").ToStdString(3, 3));

    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1*2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2;").ToStdString(3, 3));

    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1/2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U".5;").ToStdString(3, 3));
}

TEST_F(CalcTestReal, numbers4)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"21.2;").ToStdString(3, 3) == "21.2E+0");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"21.2E+0;").ToStdString(3, 3) == "21.2E+0");
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

TEST_F(CalcTestReal, functions2)
{
    Real res = parser.Parse(ElementId{0, 0, 1}, U"rad(pi/2);", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"deg(180);", AngleMeasure::Degree, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"pi;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"grad(1);", AngleMeasure::Grad, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0.9;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
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

TEST_F(CalcTestReal, variables6)
{
    parser.Parse(ElementId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"5d;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"5d;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"5.1d;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"5.1d;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables7)
{
    parser.Parse(ElementId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"d*5;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"20.4;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"d=4.1*d;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"d+5;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 4}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables8)
{
    parser.Parse(ElementId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"a2_=3.3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3) == parser.Parse(ElementId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables9)
{
    parser.Parse(ElementId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"2*a2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"2*a2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables10)
{
    parser.Parse(ElementId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"a22=3.3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3) == parser.Parse(ElementId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables11)
{
    parser.Parse(ElementId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"3.75;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables12)
{
    parser.Parse(ElementId{0, 0, 1}, U"a{1}=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"a{1};").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 2}, U"5;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 2}, U"a{1};").ToStdString(3, 3);
    parser.Parse(ElementId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 4}, U"12;").ToStdString(3, 3)) << 
        parser.Parse(ElementId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3);
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

TEST_F(CalcTestReal, trigonometric1)
{
    Real res = parser.Parse(ElementId{0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
    
    res = parser.Parse(ElementId{0, 0, 1}, U"arcsin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric2)
{
    Real res = parser.Parse(ElementId{0, 0, 1}, U"sin(90);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsin(1);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric3)
{
    Real res = parser.Parse(ElementId{0, 0, 1}, U"sin(deg(90));", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"deg(arcsin(1));", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, units1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1m;").ToStdString(3, 3) == "1.E+0(m)") << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3*m;").ToStdString(3, 3) == "2.3E+0(m)") << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3*m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m*2m;").ToStdString(3, 3) == "4.6E+0(m^2)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m*2m;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units2)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/m;").ToStdString(3, 3) == "2.E+0(1/(m))") << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2*(1/m);").ToStdString(3, 3) == "2.E+0(1/(m))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2*(1/m);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2m*3*(1/m);").ToStdString(3, 3) == "6.E+0") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2m*3*(1/m);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units3)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(2)/(m);").ToStdString(3, 3) == "6.E+0(1/(m))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(2)/(m);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(6m)/(3s);").ToStdString(3, 3) == "6.E+0((m)/(s))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(6m)/(3s);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units4)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m+3m;").ToStdString(3, 3) == "5.3E+0(m)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m+3m;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m+4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4.5+3m+4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units5)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5.3m-3m;").ToStdString(3, 3) == "2.3E+0(m)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5.3m-3m;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m-4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4.5-3m-4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units6)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m,2);").ToStdString(3, 3) == "9.E+0(m^2)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m,2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m/s,2);").ToStdString(3, 3) == "9.E+0((m^2)/(s^2))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m/s,2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5m/pow(s,2);").ToStdString(3, 3) == "5.E+0((m)/(s^2))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5m/pow(s,2);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units7)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0, 1}, U"km~1000m;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1km;").ToStdString(3, 3) == "1.E+0(km)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1km;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units8)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m;").ToStdString(3, 3) == "0.2E+0(m)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.02m;").ToStdString(3, 3) == "2.E+0(cm)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.02m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.002m;").ToStdString(3, 3) == "2.E+0(mm)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.002m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"200m;").ToStdString(3, 3) == "0.2E+0(km)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"200m;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2200m;").ToStdString(3, 3) == "2.2E+0(km)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2200m;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units9)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m/s;").ToStdString(3, 3) == "0.2E+0((m)/(s))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m/s;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"5000m/s;").ToStdString(3, 3) == "5.E+0((km)/(s))") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"5000m/s;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units10)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1Hz;").ToStdString(3, 3) == "1.E+0(Hz)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1Hz;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2000Hz;").ToStdString(3, 3) == "2.E+0(kHz)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2000Hz;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50*(1/s);").ToStdString(3, 3) == "50.E+0(Hz)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50*(1/s);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50(1)/(s);").ToStdString(3, 3) == "50.E+0(Hz)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50(1)/(s);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, units11)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));").ToStdString(3, 3) == "50.E+0(N)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));").ToStdString(3, 3) == "5.E+0(kN)") << 
        parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));").ToStdString(3, 3);
}

}
