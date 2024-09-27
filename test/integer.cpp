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

TEST_F(CalcTestInteger, integers2)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(2/3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(2/3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-1)/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-1)/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-1)/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-1)/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-2)/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-2)/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-2)/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"0;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-2)/(-3);");
}

TEST_F(CalcTestInteger, integers3)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"4/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"4/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(4/3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(4/3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-4/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-4/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-4)/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-4)/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-4)/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-4)/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"4/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-1;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"4/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-7/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-7/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-7/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-7/(-3);");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-7)/3;") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-7)/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-7)/(-3);") == parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(-7)/(-3);");
}

TEST_F(CalcTestInteger, integers4)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"+5;");
    ASSERT_TRUE(res.ToString(10) == U"5") << res.ToStdString(10);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-5;");
    ASSERT_TRUE(res.ToString(10) == U"-5") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, integers5)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5(4*2);");
    ASSERT_TRUE(res.ToString(10) == U"40") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical1)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"!5;");
    ASSERT_TRUE(res.ToString(10) == U"2") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical2)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"10&12;");
    ASSERT_TRUE(res.ToString(10) == U"8") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical3)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"10|12;");
    ASSERT_TRUE(res.ToString(10) == U"14") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, logical4)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"10^12;");
    ASSERT_TRUE(res.ToString(10) == U"6") << res.ToStdString(10);
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
    Integer r = parser.Parse(ElementId{0, 0, 0, 0, 2}, U"пр+5;");
    ASSERT_TRUE(r == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"60;")) << r.ToStdString();
    parser.Parse(ElementId{0, 0, 0, 0, 3}, U"fп=55;");
    r = parser.Parse(ElementId{0, 0, 0, 0, 4}, U"fп+5;");
    ASSERT_TRUE(r == parser.Parse(ElementId{0, 0, 0, 0, 3}, U"60;")) << r.ToStdString();
}

TEST_F(CalcTestInteger, variables3)
{
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1, 1, 0}, U"3a;") == parser.Parse(ElementId{0, 0, 1, 1, 0}, U"15;")) << 
        parser.Parse(ElementId{0, 0, 1, 1, 0}, U"3a;").ToStdString();
}

TEST_F(CalcTestInteger, variables4)
{
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"a=5;");
    parser.Parse(ElementId{0, 0, 0, 1, 1}, U"b=6;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2, 1, 0}, U"3a+4b+3;") == parser.Parse(ElementId{0, 0, 2, 1, 0}, U"42;")) << 
        parser.Parse(ElementId{0, 0, 2, 1, 0}, U"3a+4b+3;").ToStdString();
}

TEST_F(CalcTestInteger, variables5)
{
    parser.Parse(ElementId{0, 0, 1}, U"a{1}=5;");
    parser.Parse(ElementId{0, 0, 1}, U"a{1}=6;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"a{1};").ToStdString() == parser.Parse(ElementId{0, 0, 2}, U"6;").ToStdString()) << 
        parser.Parse(ElementId{0, 0, 2}, U"a{1};").ToStdString();
    parser.Parse(ElementId{0, 0, 3}, U"a{2}=7;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"a{1}+a{2};").ToStdString() == parser.Parse(ElementId{0, 0, 4}, U"13;").ToStdString()) << 
        parser.Parse(ElementId{0, 0, 4}, U"a{1}+a{2};").ToStdString();
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

TEST_F(CalcTestInteger, functions2)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"3pow(2,3);", 3);
    ASSERT_TRUE(r.ToStdString() == "24") << r.ToStdString();
}

TEST_F(CalcTestInteger, functions3)
{
    parser.Parse(ElementId{0, 0, 0, 0, 2}, U"a=5;");
    auto r = parser.Parse(ElementId{0, 0, 1}, U"pow(2,3)a;", 3);
    ASSERT_TRUE(r.ToStdString() == "40") << r.ToStdString();
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

TEST_F(CalcTestInteger, notation1)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123;");
    ASSERT_TRUE(res.ToString(2) == U"1111011") << res.ToStdString(2);
    ASSERT_TRUE(res.ToString(8) == U"173") << res.ToStdString(8);
    ASSERT_TRUE(res.ToString(10) == U"123") << res.ToStdString(10);
    ASSERT_TRUE(res.ToString(16) == U"7b") << res.ToStdString(16);
}

TEST_F(CalcTestInteger, notation2)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"bin[1010];");
    ASSERT_TRUE(res.ToString(2) == U"1010") << res.ToStdString(2);
    ASSERT_TRUE(res.ToString(10) == U"10") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"bin[123];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"bin[101+111];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation3)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"oct[2345];");
    ASSERT_TRUE(res.ToString(10) == U"1253") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"oct[985];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"oct[10/345];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation4)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"dec[4569];");
    ASSERT_TRUE(res.ToString(10) == U"4569") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"dec[3ef];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"dec[101+35];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation5)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"hex[56fe];");
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"hex[dt45];"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"hex[101+f1];"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation6)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1010;", Notation::Binary);
    ASSERT_TRUE(res.ToString(10) == U"10") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123;", Notation::Binary), yutovo_calculator::MathException);
}

TEST_F(CalcTestInteger, notation7)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2345;", Notation::Octal);
    ASSERT_TRUE(res.ToString(10) == U"1253") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"128;", Notation::Octal), yutovo_calculator::MathException);
}

TEST_F(CalcTestInteger, notation8)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1010;", Notation::Decimal);
    ASSERT_TRUE(res.ToString(10) == U"1010") << res.ToStdString(10);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123f;", Notation::Decimal), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestInteger, notation9)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"56fe;", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"56FE;", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22270") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, notation10)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"56fe+dec[10];", Notation::Hexadecimal);
    ASSERT_TRUE(res.ToString(10) == U"22280") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, builtin_functions1)
{
    Integer res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1!;");
    ASSERT_TRUE(res.ToString(10) == U"1") << res.ToStdString(10);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5!;");
    ASSERT_TRUE(res.ToString(10) == U"120") << res.ToStdString(10);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(5+2)!;");
    ASSERT_TRUE(res.ToString(10) == U"5040") << res.ToStdString(10);
}

TEST_F(CalcTestInteger, compare1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10==10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(10<>10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
}

TEST_F(CalcTestInteger, compare2)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0}, U"a=5;");
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a==10);").ToStdString();
    ASSERT_TRUE(s == "0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<>10);").ToStdString();
    ASSERT_TRUE(s == "1") << s;
}

TEST_F(CalcTestInteger, sum1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=0,t=t+1);").ToStdString();
    ASSERT_TRUE(s == "11") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=0,t=t+i);").ToStdString();
    ASSERT_TRUE(s == "55") << s;
}

TEST_F(CalcTestInteger, sum2)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t1=0,t1=t1+loop(j=0,(j<=20),j=j+1,t2=0,t2=t2+i+j));").ToStdString();
    ASSERT_TRUE(s == "3465") << s;
}

TEST_F(CalcTestInteger, prod1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=0,(i<=10),i=i+1,t=1,t=t*2);").ToStdString();
    ASSERT_TRUE(s == "2048") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(i=1,(i<=10),i=i+1,t=1,t=t*i);").ToStdString();
    ASSERT_TRUE(s == "3628800") << s;
}

}
