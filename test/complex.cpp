#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestComplex, numbers1)
{
    std::string res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1;").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 1}, U"0.2;").ToStdString(3, 3);
    ASSERT_TRUE(res == "0.2E+0") << res;
    res = parser.Parse(ElementId{0, 0, 1}, U"0.2E+0;").ToStdString(3, 3);
    ASSERT_TRUE(res == "0.2E+0") << res;
}

TEST_F(CalcTestComplex, numbers2)
{
    std::string res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"1i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*1.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2+1i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "2.E+0+i*1.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*5.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"5.678i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*5.678E+0") << res;
}

TEST_F(CalcTestComplex, numbers3)
{
    std::string res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"i*(1+2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2/i;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*-2.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*2.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"2(2+3i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "4.E+0+i*6.E+0") << res;
}

TEST_F(CalcTestComplex, numbers4)
{
    parser.SetLocale(Language::Russian);
    std::string res;
    EXPECT_THROW(parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2)*i;").ToStdString(3, 3), yutovo_calculator::SyntaxException);
    res = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(1+2)*j;").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*3.E+0") << res;
}

TEST_F(CalcTestComplex, trigonometric1)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"sin(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.175E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sin(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.841E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sin(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "9.154E+0+i*-4.169E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric2)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"cos(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.543E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cos(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.54E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cos(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-4.19E+0+i*-9.109E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric3)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"tg(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.762E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"tg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.557E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"tg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.00376E+0+i*1.003E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric4)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"ctg(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.313E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"ctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.642E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"ctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.00374E+0+i*-0.997E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric5)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"sec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.648E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.0417E+0+i*0.0906E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric6)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"cosec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cosec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.188E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cosec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.0905E+0+i*0.0412E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric7)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"sh(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.841E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sh(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.175E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sh(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-3.591E+0+i*0.531E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric8)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"ch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.54E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"ch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.543E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"ch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-3.725E+0+i*0.512E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric9)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"th(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.557E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"th(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.762E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"th(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.965E+0+i*-0.00988E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric10)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"cth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.642E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cth(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.313E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"cth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.036E+0+i*0.0106E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric11)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"sch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.648E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"sch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.264E+0+i*-0.0362E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric12)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"csch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.188E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"csch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.851E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"csch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "-0.273E+0+i*-0.0403E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric13)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arcsin(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "i*0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsin(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsin(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.571E+0+i*1.983E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric14)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arccos(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0+i*-0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arccos(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arccos(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.E+0+i*-1.983E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric15)
{
    Complex res;
    EXPECT_THROW(res = parser.Parse(ElementId{0, 0, 1}, U"arctg(i);"), yutovo_calculator::MathException);

    res = parser.Parse(ElementId{0, 0, 1}, U"arctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.785E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.41E+0+i*0.229E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric16)
{
    Complex res;
    EXPECT_THROW(res = parser.Parse(ElementId{0, 0, 1}, U"arcctg(i);"), yutovo_calculator::MathException);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcctg(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.785E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcctg(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.161E+0+i*-0.229E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric17)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arcsec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0+i*0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.42E+0+i*0.231E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric18)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arccosec(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "i*-0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arccosec(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arccosec(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::Radian && res.ToStdString(3, 3) == "0.15E+0+i*-0.231E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric19)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arsh(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arsh(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arsh(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.969E+0+i*0.965E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric20)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0+i*1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "1.983E+0+i*1.E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric21)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*0.785E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(res = parser.Parse(ElementId{0, 0, 1}, U"arth(1);"), yutovo_calculator::MathException);

    res = parser.Parse(ElementId{0, 0, 1}, U"arth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.147E+0+i*1.339E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric22)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arcth(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-0.785E+0") << res.ToStdString(3, 3);

    EXPECT_THROW(res = parser.Parse(ElementId{0, 0, 1}, U"arcth(1);"), yutovo_calculator::MathException);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcth(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.147E+0+i*-0.232E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric23)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arsch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0+i*-1.571E+0") << res.ToStdString(3, 3) << (int)res.GetAngleMeasure();

    res = parser.Parse(ElementId{0, 0, 1}, U"arsch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arsch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.231E+0+i*-1.42E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, trigonometric24)
{
    Complex res = parser.Parse(ElementId{0, 0, 1}, U"arcsch(i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "i*-1.571E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsch(1);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.881E+0") << res.ToStdString(3, 3);

    res = parser.Parse(ElementId{0, 0, 1}, U"arcsch(2+3i);");
    ASSERT_TRUE(res.GetAngleMeasure() == AngleMeasure::None && res.ToStdString(3, 3) == "0.157E+0+i*-0.23E+0") << res.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions1)
{
    std::string res = parser.Parse(ElementId{0, 0, 1}, U"ln(i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*1.571E+0") << res;
    res = parser.Parse(ElementId{0, 0, 1}, U"ln(5+i);").ToStdString(3, 3);
    ASSERT_TRUE(res == "1.629E+0+i*0.197E+0") << res;
}

TEST_F(CalcTestComplex, functions2)
{
    std::string res = parser.Parse(ElementId{0, 0, 1}, U"pow(i,2);").ToStdString(3, 3);
    ASSERT_TRUE(res == "-1.E+0") << res;
    res = parser.Parse(ElementId{0, 0, 1}, U"pow(2i,3);").ToStdString(3, 3);
    ASSERT_TRUE(res == "i*8.E+0") << res;
}

TEST_F(CalcTestComplex, functions3)
{
    auto r = parser.Parse(ElementId{0, 0, 1}, U"2exp(1);", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "5.437E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, functions4)
{
    parser.Parse(ElementId{0, 0, 0, 0, 2}, U"a=5;");
    auto r = parser.Parse(ElementId{0, 0, 1}, U"pow(2,3)a;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "40.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestComplex, sqrt1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(ElementId{0, 0, 1}, U"sqrt(i);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "0.707E+0+i*0.707E+0,-0.707E+0+i*-0.707E+0") << res;

    results.clear();
    parser.Parse(ElementId{0, 0, 1}, U"sqrt(3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    res = ToString(results);
    ASSERT_TRUE(res == "1.732E+0,-1.732E+0") << res;

    results.clear();
    parser.Parse(ElementId{0, 0, 1}, U"sqrt(2i+3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    res = ToString(results);
    ASSERT_TRUE(res == "1.817E+0+i*0.55E+0,-1.817E+0+i*-0.55E+0") << res;
}

TEST_F(CalcTestComplex, sqrt2)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(ElementId{0, 0, 1}, U"sqrt(sqrt(1+i));", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "1.07E+0+i*0.213E+0,0.213E+0+i*-1.07E+0,-1.07E+0+i*-0.213E+0,-0.213E+0+i*1.07E+0") << res;
}

TEST_F(CalcTestComplex, root1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(ElementId{0, 0, 1}, U"root(1+i,3);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "1.084E+0+i*0.291E+0,-0.794E+0+i*0.794E+0,-0.291E+0+i*-1.084E+0") << res;
}

TEST_F(CalcTestComplex, exp1)
{
    Dependencies dependencies;
    std::vector<Complex> results;
    parser.Parse(ElementId{0, 0, 1}, U"exp(i);", &dependencies, AngleMeasure::Radian, AngleMeasure::Radian, 3, 10, results);
    std::string res = ToString(results);
    ASSERT_TRUE(res == "0.54E+0+i*0.841E+0") << res;
}

TEST_F(CalcTestComplex, variables1)
{
    parser.Parse(ElementId{0, 0, 1}, U"v=5;");
    std::string s = parser.Parse(ElementId{0, 0, 2}, U"v;").ToStdString(3, 3);
    ASSERT_TRUE(s == "5.E+0") << s;
}

TEST_F(CalcTestComplex, variables2)
{
    parser.Parse(ElementId{0, 0, 1}, U"v=5;");
    std::string s = parser.Parse(ElementId{0, 0, 2}, U"(1/2)v;").ToStdString(3, 3);
    ASSERT_TRUE(s == "2.5E+0") << s;
}

TEST_F(CalcTestComplex, compare1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"(0<3);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((3+5*i)==(3+5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"((3+5*i)<>(3+5*i));").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
}

TEST_F(CalcTestComplex, compare2)
{
    parser.Parse(ElementId{0, 0, 0, 0, 0}, U"a=3;");
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a>10);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a==3+5*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"(a<>3+5*i);").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0") << s;
}

TEST_F(CalcTestComplex, sum1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=10),k=k+1,t=0,t=t+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(k=1,(k<=10),k=k+1,t=0,t=t+k);").ToStdString(3, 3);
    ASSERT_TRUE(s == "55.E+0") << s;
}

TEST_F(CalcTestComplex, sum2)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=10),k=k+1,t1=0,t1=t1+k*i+1);").ToStdString(3, 3);
    ASSERT_TRUE(s == "11.E+0+i*55.E+0") << s;
}

TEST_F(CalcTestComplex, prod1)
{
    std::string s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(k=0,(k<=5),k=k+1,t=1,t=t*(2));").ToStdString(3, 3);
    ASSERT_TRUE(s == "64.E+0") << s;
    s = parser.Parse(ElementId{0, 0, 0, 0, 1}, U"loop(k=1,(k<=3),k=k+1,t=1,t=t*(i+5));").ToStdString(3, 3);
    ASSERT_TRUE(s == "110.E+0+i*74.E+0") << s;
}

TEST_F(CalcTestComplex, errors1)
{
    try
    {
        parser.Parse(ElementId{0, 0, 1}, U";");
    }
    catch (yutovo_calculator::SyntaxException& ex)
    {
        ASSERT_TRUE(ex.id == MakeElementId(ElementId{0, 0, 1}) && ex.ex_id == ParserExceptionCode::ExpressionExpected && ex.pos == 0) << ex.ex_id;
        return;
    }
    ASSERT_FALSE(true);
}

}
