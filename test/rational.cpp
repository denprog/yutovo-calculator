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
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п=3/2;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"п;") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"3/2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"п;").ToStdString();
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"пр=3/2;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 1, 0, 1}, U"пр+5;") == parser.Parse(ElementId{0, 0, 1, 0, 1}, U"13/2;")) << 
        parser.Parse(ElementId{0, 0, 1, 0, 1}, U"пр+5;").ToStdString();
}

TEST_F(CalcTestRational, variables2)
{
    parser.Parse(ElementId{0, 0, 1}, U"a=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"(3)/(4)a;") == parser.Parse(ElementId{0, 0, 2}, U"(15)/(4);")) << 
        parser.Parse(ElementId{0, 0, 2}, U"(3)/(4)a;");
}

TEST_F(CalcTestRational, variables3)
{
    parser.Parse(ElementId{0, 0, 1}, U"d=4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 2}, U"5d;") == parser.Parse(ElementId{0, 0, 2}, U"20;")) << 
        parser.Parse(ElementId{0, 0, 2}, U"5d;");
    parser.Parse(ElementId{0, 0, 3}, U"d2=5;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 4}, U"5d+3d2;") == parser.Parse(ElementId{0, 0, 4}, U"35;")) << 
        parser.Parse(ElementId{0, 0, 4}, U"5d+3d2;");
}

TEST_F(CalcTestRational, variables4)
{
    parser.Parse(ElementId{0, 0, 1}, U"d=1/4;");
    parser.Parse(ElementId{0, 0, 2}, U"d{2}=3/4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 3}, U"d+d{2};") == parser.Parse(ElementId{0, 0, 3}, U"1;")) << 
        parser.Parse(ElementId{0, 0, 3}, U"d+d{2};");
    parser.Parse(ElementId{0, 0, 4}, U"d{2}=5/2;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 5}, U"5d{2}+3d;") == parser.Parse(ElementId{0, 0, 5}, U"13(1/4);")) << 
        parser.Parse(ElementId{0, 0, 5}, U"5d{2}+3d;");
}

TEST_F(CalcTestRational, functions1)
{
    parser.Parse(ElementId{0, 0, 0, 0, 2}, U"f(x)=2*x/3;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 3}, U"f(2);") == parser.Parse(ElementId{0, 0, 0, 0, 2, 0}, U"4/3;"));
    parser.Parse(ElementId{0, 0, 0, 0, 1}, U"f(x)=x/4;");
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"f(2);") == parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"4/3;")) << 
        parser.Parse(ElementId{0, 0, 0, 2, 3, 1}, U"f(2);").ToStdString();
}

TEST_F(CalcTestRational, symbols1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"•;").ToStdString();
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣;"), yutovo_calculator::SyntaxException) << parser.Parse(ElementId{0, 0, 0, 0, 1}, U"‣;").ToStdString();
}

TEST_F(CalcTestRational, errors1)
{
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"123"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 2}, U"123+(2/3)"), yutovo_calculator::SyntaxException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 3}, U"123+3;45"), yutovo_calculator::SyntaxException);
}

TEST_F(CalcTestRational, rationals1)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/4;") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1/2;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/4;").ToStdString();
}

TEST_F(CalcTestRational, rationals2)
{
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(1/4);") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"9/4;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(1/4);").ToStdString();
    ASSERT_TRUE(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3*22(3/5);") == parser.Parse(ElementId{0, 0, 0, 0, 2}, U"339/5;")) << 
        parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3*22(3/5);").ToStdString();
}

TEST_F(CalcTestRational, proper1)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(5/4);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "1");
    ASSERT_TRUE(n.ToStdString() == "1");
    ASSERT_TRUE(d.ToStdString() == "4");
}

TEST_F(CalcTestRational, proper2)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(3/4);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0");
    ASSERT_TRUE(n.ToStdString() == "3");
    ASSERT_TRUE(d.ToStdString() == "4");
}

TEST_F(CalcTestRational, proper3)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(2/3);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "-2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper4)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"-(2/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper5)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"((-2)/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper6)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"((-7)/3);");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "-2") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "1") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper7)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(7/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "-2") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "1") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, proper8)
{
    auto res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(2/(-3));");
    Integer i, n, d;
    res.ToProper(i, n, d);
    ASSERT_TRUE(i.ToStdString() == "0") << i.ToStdString();
    ASSERT_TRUE(n.ToStdString() == "-2") << n.ToStdString();
    ASSERT_TRUE(d.ToStdString() == "3") << d.ToStdString();
}

TEST_F(CalcTestRational, units1)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1m;")).ToStdString();
    ASSERT_TRUE(s == "1(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/3*m;")).ToStdString();
    ASSERT_TRUE(s == "2/3(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(2)/(3)m*2m;")).ToStdString();
    ASSERT_TRUE(s == "4/3(m^2)") << s;
}

TEST_F(CalcTestRational, units2)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/m;")).ToStdString();
    ASSERT_TRUE(s == "2(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2*(1/m);")).ToStdString();
    ASSERT_TRUE(s == "2(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2m*3*(1/m);")).ToStdString();
    ASSERT_TRUE(s == "6") << s;
}

TEST_F(CalcTestRational, units3)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3*(2)/(m);")).ToStdString();
    ASSERT_TRUE(s == "6(1/(m))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"3*(6m)/(3s);")).ToStdString();
    ASSERT_TRUE(s == "6((m)/(s))") << s;
}

TEST_F(CalcTestRational, units4)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(2)/(3)m+3m;")).ToStdString();
    ASSERT_TRUE(s == "11/3(m)") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m+4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4/5+3m+4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, units5)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(5)/(3)m-3m;")).ToStdString();
    ASSERT_TRUE(s == "-4/3(m)") << s;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"3m-4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 1}, U"4/5-3m-4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, units6)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m,2);")).ToStdString();
    ASSERT_TRUE(s == "9(m^2)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"pow(3m/s,2);")).ToStdString();
    ASSERT_TRUE(s == "9((m^2)/(s^2))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5m/pow(s,2);")).ToStdString();
    ASSERT_TRUE(s == "5((m)/(s^2))") << s;
}

TEST_F(CalcTestRational, units7)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0, 1}, U"km~1000m;");
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1km;")).ToStdString();
    ASSERT_TRUE(s == "1(km)") << s;
}

TEST_F(CalcTestRational, units8)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2m;")).ToStdString();
    ASSERT_TRUE(s == "2(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2/100*m;")).ToStdString();
    ASSERT_TRUE(s == "2(cm)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(2)/(1000)m;")).ToStdString();
    ASSERT_TRUE(s == "2(mm)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"200m;")).ToStdString();
    ASSERT_TRUE(s == "200(m)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(3000)/(2)m;")).ToStdString();
    ASSERT_TRUE(s == "3/2(km)") << s;
}

TEST_F(CalcTestRational, units9)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(1)/(5)m/s;")).ToStdString();
    ASSERT_TRUE(s == "12((m)/(min))") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"5000m/s;")).ToStdString();
    ASSERT_TRUE(s == "5((km)/(s))") << s;
}

TEST_F(CalcTestRational, units10)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1Hz;")).ToStdString();
    ASSERT_TRUE(s == "1(Hz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2000Hz;")).ToStdString();
    ASSERT_TRUE(s == "2(kHz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50*(1/s);")).ToStdString();
    ASSERT_TRUE(s == "50(Hz)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"50*(1)/(s);")).ToStdString();
    ASSERT_TRUE(s == "50(Hz)") << s;
}

TEST_F(CalcTestRational, units11)
{
    std::string s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString();
    ASSERT_TRUE(s == "50(N)") << s;
    s = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));")).ToStdString();
    ASSERT_TRUE(s == "5(kN)") << s;
}

TEST_F(CalcTestRational, units12)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"liniya{rus};")).ToStdString();
    ASSERT_TRUE(t == "1(liniya){rus}") << t;
}

TEST_F(CalcTestRational, units13)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"2*km;")).ToStdString();
    ASSERT_TRUE(t == "2(km)") << t;
}

TEST_F(CalcTestRational, units14)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"7fut{rus};")).ToStdString();
    ASSERT_TRUE(t == "1(sazhen){rus}") << t;
}

TEST_F(CalcTestRational, units15)
{
    ElementId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
    auto val = parser.Parse(id, U"5*(km/hour);");
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, val, cast_units);
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"m", U"s")));
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"km", U"hour")));
    std::string t = parser.GetSuitableUnit(id, val).ToStdString();
    ASSERT_TRUE(t == "5((km)/(hour))") << t;
}

TEST_F(CalcTestRational, units16)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(10)/(4s);")).ToStdString();
    ASSERT_TRUE(t == "5/2(Hz)") << t;
    t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(50)/(4s);")).ToStdString();
    ASSERT_TRUE(t == "25/2(Hz)") << t;
}

TEST_F(CalcTestRational, units17)
{
    std::string t = parser.GetSuitableUnit(ElementId{0, 0, 0, 0, 1}, parser.Parse(ElementId{0, 0, 0, 0, 2}, U"(1m)/(3s);")).ToStdString();
    ASSERT_TRUE(t == "20((m)/(min))") << t;
}

}
