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

TEST_F(CalcTestReal, numbers5)
{
    Real res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"+5.46;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.46E+0") << res.ToStdString(3, 3);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-5.55;");
    ASSERT_TRUE(res.ToString(3, 3) == U"-5.55E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers6)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(4+5);").ToStdString(3, 3);
    ASSERT_TRUE(s == "18.E+0") << s;
}

TEST_F(CalcTestReal, numbers7)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"E;"), yutovo_calculator::SyntaxException);
    auto s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1E+0;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"E+0;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"E+1;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"E-1;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, numbers8)
{
    parser.SetLocale(Language::Russian);
    auto res = parser.Parse(ElementId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
    res = parser.Parse(ElementId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
}

TEST_F(CalcTestReal, arithmetic1)
{
    Real res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"10%75;");
    ASSERT_TRUE(res.ToString(3, 3) == U"7.5E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arithmetic2)
{
    Real res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(3+4);");
    ASSERT_TRUE(res.ToString(3, 3) == U"14.E+0") << res.ToStdString(3, 3);
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

TEST_F(CalcTestReal, functions3)
{
    Real res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5.!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"120.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(5+2)!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.04E+3") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions4)
{
    Real res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"root(4,2);");
    ASSERT_TRUE(res.ToString(3, 3) == U"2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions5)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"exp(1);", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, functions6)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"2exp(1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.437E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions7)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "24.E+0") << r.ToStdString(3, 3);
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

TEST_F(CalcTestReal, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(ElementId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(ElementId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
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
    parser.Parse(ElementId{0, 0, 3}, U"vp=55.5;", &dependencies);
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3, 0}, U"vp+5;", &dependencies) == parser.Parse(ElementId{0, 0, 3, 0}, U"60.5;")) << 
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

TEST_F(CalcTestReal, variables13)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"pi;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.142E+0") << r.ToStdString(3, 3);
    r = parser.Parse(ElementId{0, 0, 1}, U"pi;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
    r = parser.Parse(ElementId{0, 0, 1}, U"π;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables14)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"e;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.718E+0") << r.ToStdString(3, 3);
    r = parser.Parse(ElementId{0, 0, 1}, U"e;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables15)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "6.674E-11((m^3)/(kg*s^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables16)
{
    parser.Parse(ElementId{0, 0, 1}, U"a=5.5;");
    auto r = parser.Parse(ElementId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);

    parser.SetLocale(Language::Russian);
    r = parser.Parse(ElementId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables17)
{
    parser.SetLocale(Language::Russian);
    parser.Parse(ElementId{0, 0, 1}, U"α=1;");
    parser.Parse(ElementId{0, 0, 2}, U"a=1см;");
    parser.Parse(ElementId{0, 0, 3}, U"b=2см;");
    parser.Parse(ElementId{0, 0, 4}, U"S=a*b*sin(α);");
    auto r = parser.GetSuitableUnit(ElementId{0, 0, 5}, parser.Parse(ElementId{0, 0, 5}, U"S;"));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.683E+0(см^2)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables18)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "6.674E-11((м^3)/(кг*с^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables19)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 1}, U"g_a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "9.807E+0((м)/(с^2))") << r.ToStdString(3, 3);
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

TEST_F(CalcTestReal, trigonometric4)
{
    Real res = parser.Parse(ElementId{0, 0, 1}, U"sin(90°);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sin((pi)/(2)rad);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units1)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3*m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.3E+0(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m*2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "4.6E+0(m^2)") << s;
}

TEST_F(CalcTestReal, units2)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2*(1/m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2m*3*(1/m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
}

TEST_F(CalcTestReal, units3)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(2)/(m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3(6m)/(3s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0((m)/(s))") << s;
}

TEST_F(CalcTestReal, units4)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2.3m+3m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.3E+0(m)") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m+4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4.5+3m+4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units5)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5.3m-3m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.3E+0(m)") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m-4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4.5-3m-4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units6)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0(m^2)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m/s,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0(Gy)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5m/pow(s,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0((m)/(s^2))") << s;
}

TEST_F(CalcTestReal, units7)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0, 1}, U"km~1000m;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;
}

TEST_F(CalcTestReal, units8)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(dm)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.02m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(cm)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.002m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(mm)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"200m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.2E+0(km)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2200m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.2E+0(km)") << s;
}

TEST_F(CalcTestReal, units9)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"0.2m/s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((dm)/(s))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"5000m/s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0((km)/(s))") << s;
}

TEST_F(CalcTestReal, units10)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1Hz;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(Hz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2000Hz;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(kHz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50*(1/s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(Hz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50(1)/(s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(Hz)") << s;
}

TEST_F(CalcTestReal, units11)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(N)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(kN)") << s;
}

TEST_F(CalcTestReal, units12)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"liniya{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(liniya){rus}") << t;
}

TEST_F(CalcTestReal, units13)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2*km;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "2.E+0(km)") << t;
}

TEST_F(CalcTestReal, units14)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"400sazhen{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.8E+0(versta){rus}") << t;
}

TEST_F(CalcTestReal, units15)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"7fut{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(sazhen){rus}") << t;
}

TEST_F(CalcTestReal, units16)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1fut{rus}*1fut{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(fut^2){rus}") << t;
}

TEST_F(CalcTestReal, units17)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2m*2m;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "4.E+0(m^2)") << t;
}

TEST_F(CalcTestReal, units18)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2km*2km;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "4.E+0(km^2)") << t;
}

TEST_F(CalcTestReal, units19)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2cm*3cm;");
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "6.E+0(cm^2)") << t;
}

TEST_F(CalcTestReal, units20)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"700fut{rus};");
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.2E+0(versta){rus}") << t;
}

TEST_F(CalcTestReal, units21)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1m;");
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"mm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+3(mm)") << t;
}

TEST_F(CalcTestReal, units22)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"20*pow(mm, 2);");
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"cm", 2)).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.2E+0(cm^2)") << t;
}

TEST_F(CalcTestReal, units23)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(m/min);");
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"km", U"min")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.005E+0((km)/(min))") << t;
    t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"m", U"min")).ToStdString(3, 3);
    ASSERT_TRUE(t == "5.E+0((m)/(min))") << t;
    t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"m", U"s")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.0833E+0((m)/(s))") << t;
}

TEST_F(CalcTestReal, units24)
{
    ElementId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
    auto val = parser.Parse(id, U"5*(km/hour);");
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, val, cast_units);
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"m", U"s")));
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"km", U"hour")));
    std::string t = parser.GetSuitableUnit(id, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "5.E+0((km)/(hour))") << t;
}

TEST_F(CalcTestReal, units25)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"0.001s;");
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(ms)") << t;
}

TEST_F(CalcTestReal, units26)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1/(1000m);");
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(1/(km))") << t;
}

TEST_F(CalcTestReal, units27)
{
    ElementId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
    auto val = parser.Parse(id, U"2(m/s);");
    std::string s = val.ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((m)/(s))") << s;
    s = parser.GetSuitableUnit(id, val).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((m)/(s))") << s;
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, val, cast_units);
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"km", U"hour")));
}

TEST_F(CalcTestReal, units28)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2*(m/s);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((m)/(s))") << s;
}

TEST_F(CalcTestReal, units29)
{
    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2*(м/с);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((м)/(с))") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"2*(m/s);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, units30)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(1/2)m;").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.5E+0(m)") << s;
}

TEST_F(CalcTestReal, units31)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(2.4/2)m;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.2E+0(m)") << s;
}

TEST_F(CalcTestReal, units32)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(mm)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"50mm;");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(cm)") << s;
}

TEST_F(CalcTestReal, units33)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"500сажень{rus};");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(верста){rus}") << s;
}

TEST_F(CalcTestReal, units34)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1rad;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(rad)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1°;");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(°)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1';");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(')") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1'';");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0('')") << s;
}

TEST_F(CalcTestReal, units35)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1°C;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(K)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1K;");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(K)") << s;
}

TEST_F(CalcTestReal, units36)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"dm~1mm;");
    r = parser.Parse(ElementId{0, 0, 0, 0, 2}, U"dm;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(mm)") << s;

    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 2}, r, Unit(U"dm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(dm)") << t;
}

TEST_F(CalcTestReal, units37)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"dm~1.2mm;");
    r = parser.Parse(ElementId{0, 0, 0, 0, 2}, U"dm;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(dm)") << s;

    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 2}, r, Unit(U"mm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.2E+0(mm)") << t;
}

TEST_F(CalcTestReal, units38)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(mm)") << s;

    parser.SetLocale(Language::Russian);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;"), yutovo_calculator::SyntaxException);
    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5мм;");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(мм)") << s;
}

TEST_F(CalcTestReal, units39)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"d_m`decimeter`~10mm;");
    r = parser.Parse(ElementId{0, 0, 0, 0, 2}, U"d_m;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(d_m)") << s;
}

TEST_F(CalcTestReal, units40)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5N;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(N)") << s;
}

TEST_F(CalcTestReal, units41)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5V;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(V)") << s;
}

TEST_F(CalcTestReal, units42)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"pow(5m,2);");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "25.E+0(m^2)") << s;
}

TEST_F(CalcTestReal, units43)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,2));");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,2)/pow(3s,2));");
    s = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r, Unit(U"m", U"s")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.667E+0((m)/(s))") << s;

    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(5m);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,3));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units44)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,2),2);");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,3),3);");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,2),3);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,3),2);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units45)
{
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"-(1m-2m);");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;

    r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"+(2m-1m);");
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;
}

TEST_F(CalcTestReal, units46)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(m/min);");
    Unit u;
    ASSERT_TRUE(u.FromString(U"km/hour"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.3E+0((km)/(hour))") << t;
}

TEST_F(CalcTestReal, units47)
{
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(N);");
    Unit u;
    ASSERT_TRUE(u.FromString(U"(kg*m)/(s^2)"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "5.E+0((kg*m)/(s^2))") << t;
}

TEST_F(CalcTestReal, units48)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(м/с{SI});");
    Unit u;
    ASSERT_TRUE(u.FromString(U"(км)/(час)"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "18.E+0((км)/(час))") << t;
}

TEST_F(CalcTestReal, units49)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(м{SI});");
    Unit u;
    ASSERT_TRUE(u.FromString(U"сотка{rus}"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "234.346E+0(сотка){rus}") << t;
}

TEST_F(CalcTestReal, units50)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"pow(2,3)m;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "8.E+0(m)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units51)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"G;");
    Unit u;
    ASSERT_TRUE(u.FromString(U"((м^3)/(кг*с^2))"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "6.674E-11((м^3)/(кг*с^2))") << t;
}

TEST_F(CalcTestReal, units52)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((м/pow(с,2))*pow(м,2))/(pow(м,3)/(кг*pow(с,2)));");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(кг)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units53)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(м/с)/(м/с);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units54)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1Ом;");
    Unit u;
    ASSERT_TRUE(u.FromString(U"((Дж)/(нс*мкА^2))"));
    std::string t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E-21((Дж)/(нс*мкА^2))") << t;
    ASSERT_TRUE(u.FromString(U"((Дж)/(сутки*мА^2))"));
    t = parser.CastToUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.0864E+0((Дж)/(сутки*мА^2))") << t;
}

TEST_F(CalcTestReal, units55)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"кг*м*А/(pow(с,2));");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(Н*А)") << s;
}

TEST_F(CalcTestReal, compare1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10<>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestReal, compare2)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0}, U"a=5;");
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<>10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<=10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

TEST_F(CalcTestReal, sum1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
}

TEST_F(CalcTestReal, sum2)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=0,(j<=20),j=j+1,t2=0,t2=t2+i+j));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.465E+3") << s;
}

TEST_F(CalcTestReal, prod1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.048E+3") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, prod2)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"loop(i=1,(i<=10),i=i+1,i_=1,i_=i_*(i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, max_time1)
{
    parser.SetMaxTime(1000);
    parser.SetMaxCastUnitSize(3);
    EXPECT_THROW(parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString(3, 3), 
        yutovo_calculator::TimeExceedException);
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    parser.SetMaxTime(10000);
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 2}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(N)") << s;
}

TEST_F(CalcTestReal, list_identifiers1)
{
    std::vector<std::pair<std::u32string, std::u32string>> variables;
    parser.ListUserVariables(variables);
    ASSERT_TRUE(std::find_if(variables.begin(), variables.end(), 
        [](auto& p)
        {
            return p.first == U"c" && p.second == U"speed of light";
        }) != variables.end());
    
    variables.clear();
    parser.SetLocale(Language::Russian);
    parser.ListUserVariables(variables);
    ASSERT_TRUE(std::find_if(variables.begin(), variables.end(), 
        [](auto& p)
        {
            return p.first == U"ск_св" && p.second == U"скорость света";
        }) != variables.end());
}

TEST_F(CalcTestReal, list_identifiers2)
{
    std::vector<CustomUnit<Real>> units;
    parser.ListUserUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.name == U"km" && p.description == U"kilometre";
        }) != units.end());
    
    units.clear();
    parser.SetLocale(Language::Russian);
    parser.ListUserUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.name == U"км" && p.description == U"километр";
        }) != units.end());
}

TEST_F(CalcTestReal, list_identifiers3)
{
    std::vector<std::pair<std::u32string, std::u32string>> units;
    parser.ListBuiltinUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.first == U"m" && p.second == U"metre";
        }) != units.end());
    
    units.clear();
    parser.SetLocale(Language::Russian);
    parser.ListBuiltinUnits(units);
    ASSERT_TRUE(std::find_if(units.begin(), units.end(), 
        [](auto& p)
        {
            return p.first == U"м" && p.second == U"метр";
        }) != units.end());
}

}
