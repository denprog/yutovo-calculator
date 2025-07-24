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
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"0.2E+0;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers2)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.75;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3/4);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers3)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1-2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-1;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1*2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2;").ToStdString(3, 3));

    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1/2);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None);
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U".5;").ToStdString(3, 3));
}

TEST_F(CalcTestReal, numbers4)
{
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3) == "21.2E+0");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"21.2E+0;").ToStdString(3, 3) == "21.2E+0");
}

TEST_F(CalcTestReal, numbers5)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"+5.46;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.46E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"-5.55;");
    ASSERT_TRUE(res.ToString(3, 3) == U"-5.55E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, numbers6)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(4+5);").ToStdString(3, 3);
    ASSERT_TRUE(s == "18.E+0") << s;
}

TEST_F(CalcTestReal, numbers7)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E;"), yutovo_calculator::SyntaxException);
    auto s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1E+0;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E+0;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E+1;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"E-1;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, numbers8)
{
    parser.SetLocale(Language::Russian);
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
    res = parser.Parse(LogicalId{0, 0, 2}, U"21.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "21.2E+0") << res;
}

TEST_F(CalcTestReal, arithmetic1)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"10%75;");
    ASSERT_TRUE(res.ToString(3, 3) == U"7.5E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, arithmetic2)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2(3+4);");
    ASSERT_TRUE(res.ToString(3, 3) == U"14.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions1)
{
    Real r = parser.Parse(LogicalId{0, 0, 1}, U"sin(1);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.841E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"ln(4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.386E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"lg(4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.602E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"log(2, 4);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"2.0E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log(2, 3);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log$2,3;");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1.585E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log$2,1+3;");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"3.E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"log$2,(1+3);");
    ASSERT_TRUE(r.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"2.E+0;").ToStdString(3, 3)) << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions2)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"rad(pi/2);", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"deg(180);", AngleMeasure::Degree, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"pi;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"grad(1);", AngleMeasure::Grad, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0.9;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions3)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"1.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5.!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"120.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(5+2)!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"5.04E+3") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions4)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"root(4,2);");
    ASSERT_TRUE(res.ToString(3, 3) == U"2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions5)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"exp(1);", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, functions6)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2exp(1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.437E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions7)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "24.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions8)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"a=2;");
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"a!;");
    ASSERT_TRUE(res.ToString(3, 3) == U"2.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions9)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"abs(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.3E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"abs(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.3E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions10)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"integer(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(-2.7);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"integer(2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions11)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"fract(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-0.3E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"fract(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.3E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions12)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(-2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"ceil(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions13)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"floor(-2.6);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"floor(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions14)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-3.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"round(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions15)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.3);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(-2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "-2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(2.5);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"trunc(2.1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions16)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,0);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, functions17)
{
    Real res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(0, 2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, functions18)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"pow(0,-2);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::Overflow && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, user_functions1)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=5;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(x)=x+5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"f(2);") == parser.Parse(LogicalId{0, 0, 3}, U"7;")) << 
        parser.Parse(LogicalId{0, 0, 3}, U"f(2);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(2);") == parser.Parse(LogicalId{0, 0, 2}, U"2;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(2);").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"f(x)=x+7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(2);") == parser.Parse(LogicalId{0, 0, 4}, U"9;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(2);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"f(3);") == parser.Parse(LogicalId{0, 0, 2}, U"3;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"f(3);").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"f(3);") == parser.Parse(LogicalId{0, 0, 4}, U"10;")) << 
        parser.Parse(LogicalId{0, 0, 4}, U"f(10);").ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions3)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x*2;");
    parser.Parse(LogicalId{0, 0, 2}, U"f(5);", &dependencies);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"f") != dependencies.end());
}

TEST_F(CalcTestReal, user_functions4)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x)=x;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 3}, U"f(x,y)=x+y;");
    res = parser.Parse(LogicalId{0, 0, 4}, U"f(5,4);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "9.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions5)
{
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    Real res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions6)
{
    parser.Parse(LogicalId{0, 0, 8}, U"p(a)=a+15;");
    Real res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 5}, U"p(a)=a+1;");
    parser.Parse(LogicalId{0, 0, 1}, U"p(a)=a+5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 6}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "10.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 12}, U"p(a)=a+25;");
    res = parser.Parse(LogicalId{0, 0, 9}, U"p(5);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "20.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=pow(x,y);");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(5,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "25.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, user_functions8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"f(x,y)=(x)/(y);");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"f(8,2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, str1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"в;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, str2)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"d33;"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"33t;"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, variables1)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v;") == parser.Parse(LogicalId{0, 0, 2}, U"555;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"v;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;") == parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;")) << 
        parser.Parse(LogicalId{0, 0, 1}, U"v=55.5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"v=55.5;v;") == parser.Parse(LogicalId{0, 0, 2}, U"55.5;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"v=55.5;v;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"vp=55.5;", &dependencies);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;", &dependencies) == parser.Parse(LogicalId{0, 0, 3, 0}, U"60.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 0}, U"vp+5;").ToStdString(3, 3);
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"vp") != dependencies.end());
}

TEST_F(CalcTestReal, variables2)
{
    parser.Parse(LogicalId{0, 0, 1}, U"п=555;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"п;") == parser.Parse(LogicalId{0, 0, 1}, U"555;")) << 
        parser.Parse(LogicalId{0, 0, 2}, U"п;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"п=55.5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 3}, U"п;") == parser.Parse(LogicalId{0, 0, 3, 3}, U"55.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 3}, U"п;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3, 3, 0}, U"пр=55.5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3, 3, 1}, U"пр+5;") == parser.Parse(LogicalId{0, 0, 1}, U"60.5;")) << 
        parser.Parse(LogicalId{0, 0, 3, 3, 1}, U"пр+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables3)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a;") == parser.Parse(LogicalId{0, 0, 2}, U"5;")) << parser.Parse(LogicalId{0, 0, 2}, U"a;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a;") == parser.Parse(LogicalId{0, 0, 4}, U"7;")) << parser.Parse(LogicalId{0, 0, 4}, U"a;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a+1;") == parser.Parse(LogicalId{0, 0, 2}, U"6;")) << parser.Parse(LogicalId{0, 0, 2}, U"a+1;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables4)
{
    parser.Parse(LogicalId{0, 0, 2}, U"a=77;");
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 3}, U"a;") == parser.Parse(LogicalId{0, 0, 3}, U"77;")) << parser.Parse(LogicalId{0, 0, 3}, U"a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables5)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d+5;") == parser.Parse(LogicalId{0, 0, 2}, U"9;")) << parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"d=45+d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;") == parser.Parse(LogicalId{0, 0, 4}, U"54;")) << parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables6)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1d;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables7)
{
    parser.Parse(LogicalId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"d*5;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"d+5;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"20.4;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 1}, U"d=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"5.1*d;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"d=4.1*d;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 4}, U"25.5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 4}, U"d+5;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables8)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a2_=3.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a2_;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables9)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2*a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2*a2;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables10)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a2=2.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"4.6;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2a2;").ToStdString(3, 3);
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5.29;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"2.3a2;").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a22=3.3;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 1, 4}, U"8.56;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 1, 4}, U"2a2+1.2a22;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables11)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"3.75;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"(3)/(4)a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables12)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a{1}=5;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 2}, U"5;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 2}, U"a{1};").ToStdString(3, 3);
    parser.Parse(LogicalId{0, 0, 3}, U"a=7;");
    ASSERT_TRUE(parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 4}, U"12;").ToStdString(3, 3)) << 
        parser.Parse(LogicalId{0, 0, 4}, U"a{1}+a;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables13)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pi;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "3.142E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"pi;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
    r = parser.Parse(LogicalId{0, 0, 1}, U"π;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "3.14159E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables14)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"e;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.718E+0") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"e;", 5);
    ASSERT_TRUE(r.ToStdString(3, 5) == "2.71828E+0") << r.ToStdString(3, 5);
}

TEST_F(CalcTestReal, variables15)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "6.674E-11((m^3)/(kg*s^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables16)
{
    parser.Parse(LogicalId{0, 0, 1}, U"a=5.5;");
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);

    parser.SetLocale(Language::Russian);
    r = parser.Parse(LogicalId{0, 0, 2}, U"a;", 5);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.5E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables17)
{
    parser.SetLocale(Language::Russian);
    parser.Parse(LogicalId{0, 0, 1}, U"α=1;");
    parser.Parse(LogicalId{0, 0, 2}, U"a=1см;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=2см;");
    parser.Parse(LogicalId{0, 0, 4}, U"S=a*b*sin(α);");
    auto r = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"S;"));
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"1.683E+0(см^2)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables18)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"6.674E-11((м^3)/(кг*с^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables19)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"g_a;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"9.807E+0((м)/(с^2))") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables20)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"A=5;");
    r = parser.Parse(LogicalId{0, 0, 2}, U"B=45;");
    r = parser.Parse(LogicalId{0, 0, 3}, U"A%B;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.25E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables21)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"sin;");
        ASSERT_FALSE(true);
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
    }

    parser.Parse(LogicalId{0, 0, 1}, U"sin=2;");
    Real res = parser.Parse(LogicalId{0, 0, 2}, U"sin;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 3}, U"sin(2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == "0.909E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables22)
{
    parser.Parse(LogicalId{0, 0, 5}, U"b=4;");
    parser.Parse(LogicalId{0, 0, 3}, U"b=3;");
    parser.Parse(LogicalId{0, 0, 1}, U"b=2;");
    Real res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "2.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 1}, U"b=5;");
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "4.E+0") << res.ToStdString(3, 3);

    parser.Parse(LogicalId{0, 0, 5}, U"b=6;");
    res = parser.Parse(LogicalId{0, 0, 6}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "6.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 2}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "5.E+0") << res.ToStdString(3, 3);
    res = parser.Parse(LogicalId{0, 0, 4}, U"b;");
    ASSERT_TRUE(res.ToStdString(3, 3) == "3.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, variables23)
{
    std::vector<std::u32string> dependencies;
    parser.Parse(LogicalId{0, 0, 1}, U"v{12}=555;");
    auto res = parser.Parse(LogicalId{0, 0, 2}, U"v{12};", &dependencies).ToStdString(3, 3);
    ASSERT_TRUE(res == "555.E+0") << res;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"v{12}") != dependencies.end());
}

TEST_F(CalcTestReal, variables24)
{
    yutovo_calculator::ParserContext parser_context;
    parser.Parse(LogicalId{0, 0, 1}, U"v=555;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    parser.Parse(LogicalId{0, 0, 2}, U"v;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestReal, variables25)
{
    parser.SetLocale(Language::Russian);
    yutovo_calculator::ParserContext parser_context;
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k;", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"k=1мk;"), yutovo_calculator::SyntaxException);
    res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 1}, U"k=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k;", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);
}

TEST_F(CalcTestReal, variables26)
{
    parser.SetLocale(Language::Russian);
    yutovo_calculator::ParserContext parser_context;
    auto res = parser.Parse(LogicalId{0, 0, 1}, U"k{e}=1м;", &parser_context);
    ASSERT_TRUE(parser_context.no_result == true);
    res = parser.Parse(LogicalId{0, 0, 2}, U"k{e};", &parser_context);
    ASSERT_TRUE(res == "1.E+0(м)") << res;
    ASSERT_TRUE(parser_context.no_result == false);

    parser.RemoveIdentifier(LogicalId{0, 0, 1}, U"k{e}");
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 2}, U"k{e};"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, symbols1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 1}, U"•;").ToStdString(3, 3);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(LogicalId{0, 0, 1}, U"‣;").ToStdString(3, 3);
}

TEST_F(CalcTestReal, errors1)
{
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123+sin(2)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"123+3;45"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, errors2)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"d=4+f;");
        parser.Parse(LogicalId{0, 0, 2}, U"d+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors3)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"5+d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors4)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"d=5+d;");
        parser.Parse(LogicalId{0, 0, 2}, U"33455-d;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 4) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors5)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"sqrt();");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::WrongArgumentsCount && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors6)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U";");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::ExpressionExpected && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, errors7)
{
    try
    {
        parser.Parse(LogicalId{0, 0, 1}, U"2+d+5;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 2 && ex.size == 1) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, trigonometric1)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
    
    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(0);");
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Radian && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"0;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric2)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"arcsin(1);", AngleMeasure::Degree, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric3)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(deg(90));", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"deg(arcsin(1));", AngleMeasure::Radian, AngleMeasure::Degree);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::Degree && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"90;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3) << "\n" << (int)res.angle_measure;
}

TEST_F(CalcTestReal, trigonometric4)
{
    Real res = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);

    res = parser.Parse(LogicalId{0, 0, 1}, U"sin((pi)/(2)rad);", AngleMeasure::Radian, AngleMeasure::Radian);
    ASSERT_TRUE(res.angle_measure == AngleMeasure::None && res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 1}, U"1;").ToStdString(3, 3)) << 
        res.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units1)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3*m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.3E+0(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3m*2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "4.6E+0(m^2)") << s;
}

TEST_F(CalcTestReal, units2)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*(1/m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2m*3*(1/m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0") << s;
}

TEST_F(CalcTestReal, units3)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3(2)/(m);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3(6m)/(3s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "6.E+0((m)/(s))") << s;
}

TEST_F(CalcTestReal, units4)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2.3m+3m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.3E+0(m)") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"3m+4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"4.5+3m+4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units5)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5.3m-3m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.3E+0(m)") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"3m-4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"4.5-3m-4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units6)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(3m,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0(m^2)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(3m/s,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "9.E+0(Gy)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5m/pow(s,2);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0((m)/(s^2))") << s;
}

TEST_F(CalcTestReal, units7)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0, 1}, U"km~1000m;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;
}

TEST_F(CalcTestReal, units8)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(dm)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.02m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(cm)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.002m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(mm)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"200m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.2E+0(km)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2200m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.2E+0(km)") << s;
}

TEST_F(CalcTestReal, units9)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"0.2m/s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((dm)/(s))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"5000m/s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0((km)/(s))") << s;
}

TEST_F(CalcTestReal, units10)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1Hz;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(Hz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2000Hz;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(kHz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"50*(1/s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(Hz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"50(1)/(s);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(Hz)") << s;
}

TEST_F(CalcTestReal, units11)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(N)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));")).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(kN)") << s;
}

TEST_F(CalcTestReal, units12)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"liniya{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(liniya){rus}") << t;
}

TEST_F(CalcTestReal, units13)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*km;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "2.E+0(km)") << t;
}

TEST_F(CalcTestReal, units14)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"400sazhen{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.8E+0(versta){rus}") << t;
}

TEST_F(CalcTestReal, units15)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"7fut{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(sazhen){rus}") << t;
}

TEST_F(CalcTestReal, units16)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1fut{rus}*1fut{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(fut^2){rus}") << t;
}

TEST_F(CalcTestReal, units17)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2m*2m;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "4.E+0(m^2)") << t;
}

TEST_F(CalcTestReal, units18)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2km*2km;")).ToStdString(3, 3);
    ASSERT_TRUE(t == "4.E+0(km^2)") << t;
}

TEST_F(CalcTestReal, units19)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2cm*3cm;");
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "6.E+0(cm^2)") << t;
}

TEST_F(CalcTestReal, units20)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"700fut{rus};");
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.2E+0(versta){rus}") << t;
}

TEST_F(CalcTestReal, units21)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1m;");
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"mm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+3(mm)") << t;
}

TEST_F(CalcTestReal, units22)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"20*pow(mm, 2);");
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"cm", 2)).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.2E+0(cm^2)") << t;
}

TEST_F(CalcTestReal, units23)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(m/min);");
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"km", U"min")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.005E+0((km)/(min))") << t;
    t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"m", U"min")).ToStdString(3, 3);
    ASSERT_TRUE(t == "5.E+0((m)/(min))") << t;
    t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, Unit(U"m", U"s")).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.0833E+0((m)/(s))") << t;
}

TEST_F(CalcTestReal, units24)
{
    LogicalId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
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
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"0.001s;");
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(ms)") << t;
}

TEST_F(CalcTestReal, units26)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1/(1000m);");
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(1/(km))") << t;
}

TEST_F(CalcTestReal, units27)
{
    LogicalId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
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
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2*(m/s);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0((m)/(s))") << s;
}

TEST_F(CalcTestReal, units29)
{
    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"2*(м/с);").ToStdString(3, 3);
    ASSERT_TRUE(s == u8"2.E+0((м)/(с))") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"2*(m/s);"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestReal, units30)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(1/2)m;").ToStdString(3, 3);
    ASSERT_TRUE(s == u8"0.5E+0(m)") << s;
}

TEST_F(CalcTestReal, units31)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(2.4/2)m;").ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.2E+0(m)") << s;
}

TEST_F(CalcTestReal, units32)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"5.E+0(mm)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"50mm;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"5.E+0(cm)") << s;
}

TEST_F(CalcTestReal, units33)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"500сажень{rus};");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(верста){rus}") << s;
}

TEST_F(CalcTestReal, units34)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1rad;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(rad)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1°;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(°)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1';");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(')") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1'';");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0('')") << s;
}

TEST_F(CalcTestReal, units35)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1°C;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(K)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1K;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(K)") << s;
}

TEST_F(CalcTestReal, units36)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dm~1mm;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dm;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(mm)") << s;

    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 2}, r, Unit(U"dm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.E+0(dm)") << t;
}

TEST_F(CalcTestReal, units37)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"dm~1.2mm;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"dm;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(dm)") << s;

    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 2}, r, Unit(U"mm")).ToStdString(3, 3);
    ASSERT_TRUE(t == "1.2E+0(mm)") << t;
}

TEST_F(CalcTestReal, units38)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(mm)") << s;

    parser.SetLocale(Language::Russian);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5mm;"), yutovo_calculator::SyntaxException);
    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5мм;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"5.E+0(мм)") << s;
}

TEST_F(CalcTestReal, units39)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"d_m`dmeter`~1.5mm;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"d_m;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(d_m)") << s;
}

TEST_F(CalcTestReal, units40)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5N;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(N)") << s;
}

TEST_F(CalcTestReal, units41)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5V;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(V)") << s;
}

TEST_F(CalcTestReal, units42)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"pow(5m,2);");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "25.E+0(m^2)") << s;
}

TEST_F(CalcTestReal, units43)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,2));");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,2)/pow(3s,2));");
    s = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r, Unit(U"m", U"s")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.667E+0((m)/(s))") << s;

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(5m);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"sqrt(pow(5m,3));"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units44)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,2),2);");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,3),3);");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0(m)") << s;

    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,2),3);"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"root(pow(5m,3),2);"), yutovo_calculator::MathException);
}

TEST_F(CalcTestReal, units45)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"-(1m-2m);");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"+(2m-1m);");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;
}

TEST_F(CalcTestReal, units46)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(m/min);");
    Unit u;
    ASSERT_TRUE(u.FromString(U"km/hour"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "0.3E+0((km)/(hour))") << t;
}

TEST_F(CalcTestReal, units47)
{
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(N);");
    Unit u;
    ASSERT_TRUE(u.FromString(U"(kg*m)/(s^2)"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == "5.E+0((kg*m)/(s^2))") << t;
}

TEST_F(CalcTestReal, units48)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(м/с{SI});");
    Unit u;
    ASSERT_TRUE(u.FromString(U"(км)/(час)"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"18.E+0((км)/(час))") << t;
}

TEST_F(CalcTestReal, units49)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"5*(м{SI});");
    Unit u;
    ASSERT_TRUE(u.FromString(U"сотка{rus}"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"234.346E+0(сотка){rus}") << t;
}

TEST_F(CalcTestReal, units50)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(2,3)m;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "8.E+0(m)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units51)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"G;");
    Unit u;
    ASSERT_TRUE(u.FromString(U"((м^3)/(кг*с^2))"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"6.674E-11((м^3)/(кг*с^2))") << t;
}

TEST_F(CalcTestReal, units52)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((м/pow(с,2))*pow(м,2))/(pow(м,3)/(кг*pow(с,2)));");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"1.E+0(кг)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units53)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(м/с)/(м/с);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, units54)
{
    parser.SetLocale(Language::Russian);
    auto val = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1Ом;");
    Unit u;
    ASSERT_TRUE(u.FromString(U"((Дж)/(нс*мкА^2))"));
    std::string t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"1.E-21((Дж)/(нс*мкА^2))") << t;
    ASSERT_TRUE(u.FromString(U"((Дж)/(сутки*мА^2))"));
    t = parser.CastToUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, val, u).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"0.0864E+0((Дж)/(сутки*мА^2))") << t;
}

TEST_F(CalcTestReal, units55)
{
    parser.SetLocale(Language::Russian);
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"кг*м*А/(pow(с,2));");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(Н*А)") << s;
}

TEST_F(CalcTestReal, units56)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"b=1kg;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"h=1b;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"E=b*g_a*h;");
    r = parser.Parse(LogicalId{0, 0, 0, 0, 4}, U"E;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "9.807E+0(N*kg)") << s;
}

TEST_F(CalcTestReal, units57)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(kg,2)*pow(m,2)/pow(s,4);");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(N^2)") << s;
}

TEST_F(CalcTestReal, units58)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1/(pow(kg,2)*pow(m,2)/pow(s,4));");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(1/(N^2))") << s;
}

TEST_F(CalcTestReal, units59)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
    auto r = parser.Parse(id, U"с;");
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, r, cast_units);
    ASSERT_TRUE(std::adjacent_find(cast_units.begin(), cast_units.end()) == cast_units.end());
    std::string s = parser.GetSuitableUnit(id, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(с)") << s;
}

TEST_F(CalcTestReal, units60)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 0, 0, 1};
    auto r = parser.Parse(id, U"1км;");
    std::string s = parser.GetSuitableUnit(id, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(км)") << s;
    auto t = parser.CastToUnit(id, r, Unit(U"см")).ToStdString(3, 3);
    ASSERT_TRUE(t == u8"1.E+5(см)") << t;
    r = parser.Parse(id, U"1с;");
    s = parser.GetSuitableUnit(id, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0(с)") << s;
}

TEST_F(CalcTestReal, units61)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 0, 0, 1};
    auto r = parser.Parse(id, U"кг*pow(м,2)/(pow(с,2)*моль*К);");
    std::string s = parser.GetSuitableUnit(id, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0((Дж)/(моль*К))") << s;
}

TEST_F(CalcTestReal, units62)
{
    parser.SetLocale(Language::Russian);
    std::vector<std::u32string> dependencies;
    LogicalId id{0, 0, 1};
    parser.Parse(id, U"d_m~10м;");
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"10м;", &dependencies);
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"10.E+0(м)") << r.ToStdString(3, 3);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(d_m)") << s;
    ASSERT_TRUE(std::find(dependencies.begin(), dependencies.end(), U"м") != dependencies.end());
}

TEST_F(CalcTestReal, units63)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 1};
    parser.Parse(id, U"d_m~10м;");
    auto r = parser.Parse(LogicalId{0, 0, 2}, U"10м;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"10.E+0(м)") << r.ToStdString(3, 3);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(d_m)") << s;

    parser.RemoveIdentifier(id, U"d_m");
    r = parser.Parse(LogicalId{0, 0, 2}, U"10м;");
    ASSERT_TRUE(r.ToStdString(3, 3) == u8"10.E+0(м)") << r.ToStdString(3, 3);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"10.E+0(м)") << s;
}

TEST_F(CalcTestReal, units64)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"кг*м/с;");
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, r, cast_units);
    Unit u(U"кг", U"с");
    u.system = U"rus";
    u.unit.push_back(std::make_pair(U"м", 1));
    ASSERT_FALSE(FindUnit(cast_units, u));
}

TEST_F(CalcTestReal, units65)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"10Вт;");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"10.E+0(Вт)") << s;
}

TEST_F(CalcTestReal, units66)
{
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse({0, 0, 0, 1}, U"x=50*(pow(m,3));");
    r = parser.Parse({0, 0, 0, 2}, U"cube_m~50*(pow(m,3));");
    r = parser.Parse({0, 0, 0, 3}, U"4*x;");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"4.E+0(cube_m)") << s;
}

TEST_F(CalcTestReal, units67)
{
    parser.SetLocale(Language::Russian);
    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"pow(1.008,с);");
    }
    catch (yutovo_calculator::MathException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 0, 0, 2}) && ex.ex_id == ParserExceptionCode::UnitsAreIncompatible && ex.pos == 0) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, compare1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10<>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestReal, compare2)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0}, U"a=5;");
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a==10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<>10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(a<=10.);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

TEST_F(CalcTestReal, sum1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;

    try
    {
        parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"i;");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE((ex.id == LogicalId{0, 0, 0, 0, 2}) && ex.ex_id == ParserExceptionCode::UnknownIdentifier && ex.pos == 0) << LogicalIdToString(ex.id);
        return;
    }
    ASSERT_FALSE(true);
}

TEST_F(CalcTestReal, sum2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=0,(j<=20),j=j+1,t2=0,t2=t2+i+j));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.465E+3") << s;
}

TEST_F(CalcTestReal, prod1)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2);").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.048E+3") << s;
    s = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, prod2)
{
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"loop(i=1,(i<=10),i=i+1,i_=1,i_=i_*(i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "3.629E+6") << s;
}

TEST_F(CalcTestReal, max_time1)
{
    yutovo_calculator::ParserContext parser_context;
#ifdef _WIN32
    parser_context.Init(200);
#else
    parser_context.Init(1000);
#endif
    parser.SetMaxCastUnitSize(3);
    EXPECT_THROW(parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));", 
        &parser_context)).ToStdString(3, 3), yutovo_calculator::TimeExceedException);
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1+2);");
    ASSERT_TRUE(res.ToStdString(3, 3) == parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3;").ToStdString(3, 3));

    parser_context.Init(20000);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));", 
        &parser_context)).ToStdString(3, 3);
    ASSERT_TRUE(s == "50.E+0(N)") << s;
}

TEST_F(CalcTestReal, max_time2)
{
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(5000);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(234)/((3)/((4)/((6)/((7)/((6)/((7)/((2)/(4))))))));", &parser_context), 
        yutovo_calculator::TimeExceedException);
    parser_context.Init(0);
    std::string s = parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"123;", &parser_context).ToStdString(3, 3);
    ASSERT_TRUE(s == "123.E+0") << s;
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

TEST_F(CalcTestReal, money1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(₽)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"0.1₽;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.1E+0(₽)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 2}, U"1kop;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(kop)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1$;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0($)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1cent;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(¢)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"33¢;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "33.E+0(¢)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(€)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1euro;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(€)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1eurocent;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(ct)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, money2)
{
    parser.SetLocale(Language::Russian);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(₽)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"0.1₽;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "0.1E+0(₽)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, parser.Parse(LogicalId{0, 0, 0, 2}, U"1коп;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(коп)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1$;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0($)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1цент;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(цент)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(€)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1евро;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(€)") << r.ToStdString(3, 3);
    r = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1евроцент;", 3));
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(евроцент)") << r.ToStdString(3, 3);
}

}
