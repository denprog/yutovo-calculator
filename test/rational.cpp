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

}
