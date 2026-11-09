/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestRational, units1)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"1m;")).ToStdString();
    ASSERT_TRUE(s == "1(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/3*m;")).ToStdString();
    ASSERT_TRUE(s == "2/3(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2)/(3)m*2m;")).ToStdString();
    ASSERT_TRUE(s == "4/3(m^2)") << s;
}

TEST_F(CalcTestRational, units2)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2/m;")).ToStdString();
    ASSERT_TRUE(s == "2(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2*(1/m);")).ToStdString();
    ASSERT_TRUE(s == "2(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"2m*3*(1/m);")).ToStdString();
    ASSERT_TRUE(s == "6") << s;
}

TEST_F(CalcTestRational, units3)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*(2)/(m);")).ToStdString();
    ASSERT_TRUE(s == "6(1/(m))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"3*(6m)/(3s);")).ToStdString();
    ASSERT_TRUE(s == "6((m)/(s))") << s;
}

TEST_F(CalcTestRational, units4)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(2)/(3)m+3m;")).ToStdString();
    ASSERT_TRUE(s == "11/3(m)") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"3m+4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"4/5+3m+4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, units5)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(5)/(3)m-3m;")).ToStdString();
    ASSERT_TRUE(s == "-4/3(m)") << s;
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"3m-4;"), yutovo_calculator::MathException);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 1}, U"4/5-3m-4;"), yutovo_calculator::MathException);
}

TEST_F(CalcTestRational, units6)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(3m,2);")).ToStdString();
    ASSERT_TRUE(s == "9(m^2)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"pow(3m/s,2);")).ToStdString();
    ASSERT_TRUE(s == "9(Gy)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"5m/pow(s,2);")).ToStdString();
    ASSERT_TRUE(s == "5((m)/(s^2))") << s;
}

TEST_F(CalcTestRational, units7)
{
    parser.Parse(LogicalId{0, 0, 0, 0, 0, 1}, U"km~1000m;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1km;")).ToStdString();
    ASSERT_TRUE(s == "1(km)") << s;
}

TEST_F(CalcTestRational, units8)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2m;")).ToStdString();
    ASSERT_TRUE(s == "2(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2/100*m;")).ToStdString();
    ASSERT_TRUE(s == "2(cm)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(2)/(1000)m;")).ToStdString();
    ASSERT_TRUE(s == "2(mm)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"200m;")).ToStdString();
    ASSERT_TRUE(s == "200(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(3000)/(2)m;")).ToStdString();
    ASSERT_TRUE(s == "3/2(km)") << s;
}

TEST_F(CalcTestRational, units9)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(1)/(5)m/s;")).ToStdString();
    ASSERT_TRUE(s == "2((dm)/(s))") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"5000m/s;")).ToStdString();
    ASSERT_TRUE(s == "5((km)/(s))") << s;
}

TEST_F(CalcTestRational, units10)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1Hz;")).ToStdString();
    ASSERT_TRUE(s == "1(Hz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2000Hz;")).ToStdString();
    ASSERT_TRUE(s == "2(kHz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"50*(1/s);")).ToStdString();
    ASSERT_TRUE(s == "50(Hz)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"50*(1)/(s);")).ToStdString();
    ASSERT_TRUE(s == "50(Hz)") << s;
}

TEST_F(CalcTestRational, units11)
{
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"10kg*10m/(2*pow(s,2));")).ToStdString();
    ASSERT_TRUE(s == "50(N)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"1000kg*10m/(2*pow(s,2));")).ToStdString();
    ASSERT_TRUE(s == "5(kN)") << s;
}

TEST_F(CalcTestRational, units12)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"liniya{rus};")).ToStdString();
    ASSERT_TRUE(t == "1(liniya){rus}") << t;
}

TEST_F(CalcTestRational, units13)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"2*km;")).ToStdString();
    ASSERT_TRUE(t == "2(km)") << t;
}

TEST_F(CalcTestRational, units14)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"7fut{rus};")).ToStdString();
    ASSERT_TRUE(t == "1(sazhen){rus}") << t;
}

TEST_F(CalcTestRational, units15)
{
    LogicalId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
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
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(10)/(4s);")).ToStdString();
    ASSERT_TRUE(t == "5/2(Hz)") << t;
    t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(50)/(4s);")).ToStdString();
    ASSERT_TRUE(t == "25/2(Hz)") << t;
}

TEST_F(CalcTestRational, units17)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(1m)/(3s);")).ToStdString();
    ASSERT_TRUE(t == "20((m)/(min))") << t;
}

TEST_F(CalcTestRational, units18)
{
    std::string t = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 2}, U"(1/3)m;")).ToStdString();
    ASSERT_TRUE(t == "1/3(m)") << t;
}

TEST_F(CalcTestRational, units19)
{
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"-(1m-2m);");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString();
    ASSERT_TRUE(s == "1(m)") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"+(2m-1m);");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString();
    ASSERT_TRUE(s == "1(m)") << s;
}

TEST_F(CalcTestRational, units20)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"pow(2,3)m;", 3);
    ASSERT_TRUE(r.ToStdString() == "8(m)") << r.ToStdString();
}

TEST_F(CalcTestRational, units21)
{
    parser.SetLocale(Language::Spanish);
    LogicalId id{0, 0, 0, 0, 0, 0, 0, 2, 0};
    auto val = parser.Parse(id, U"5*(km/hora);");
    std::vector<Unit> cast_units;
    parser.GetCastUnits(id, val, cast_units);
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"m", U"s")));
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"km", U"hora")));
    std::string t = parser.GetSuitableUnit(id, val).ToStdString();
    ASSERT_TRUE(t == "5((km)/(hora))") << t;
}

TEST_F(CalcTestRational, units22)
{
    parser.SetLocale(Language::Russian);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(1*Гц*1*мкФ);")).ToStdString();
    ASSERT_TRUE(s == "1(МОм)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(Гц*мкФ);")).ToStdString();
    ASSERT_TRUE(s == "1(МОм)") << s;
}

TEST_F(CalcTestRational, units23)
{
    auto exports = std::make_shared<yutovo_calculator::Export>();

    Unit meter_unit(U"meter");
    Unit foot_unit(U"foot");
    Rational meter_value(U"328084/100000");
    meter_value.unit = foot_unit;
    Rational foot_value(U"3048/10000");
    foot_value.unit = meter_unit;

    exports->AddUnit<Rational>(CustomUnit<Rational>(LogicalId{0, 0, 0, 0, 1}, U"meter", U"SI", meter_value, false));
    exports->AddUnit<Rational>(CustomUnit<Rational>(LogicalId{0, 0, 0, 0, 2}, U"foot", U"SI", foot_value, false));

    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(1000);
    parser_context.exports = exports;
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"5meter;", &parser_context);
    EXPECT_NO_THROW(parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 3}, r).ToStdString());
}

TEST_F(CalcTestRational, money1)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₽)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1kop;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(kop)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1$;", 3);
    ASSERT_TRUE(r.ToStdString() == "1($)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1cent;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(¢)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"33¢;", 3));
    ASSERT_TRUE(r.ToStdString() == "33(¢)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(€)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1eurocent;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(ct)") << r.ToStdString();
}

TEST_F(CalcTestRational, money2)
{
    parser.SetLocale(Language::Russian);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₽)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1коп;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(коп)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1$;", 3);
    ASSERT_TRUE(r.ToStdString() == "1($)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1цент;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(цент)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(€)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1евроцент;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(евроцент)") << r.ToStdString();
}

TEST_F(CalcTestRational, money3)
{
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1¥;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(¥)") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 1}, U"1yuan;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(¥)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1₹;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₹)") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 1}, U"1rupee;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₹)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1R$;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(R$)") << r.ToStdString();
    r = parser.Parse(LogicalId{0, 0, 1}, U"1real;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(R$)") << r.ToStdString();
}

TEST_F(CalcTestRational, money4)
{
    parser.SetLocale(Language::German);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₽)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1kop;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(kop)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(€)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1eurocent;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(ct)") << r.ToStdString();

    //SI units in the rational parser under the German locale
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1000m;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(km)") << r.ToStdString();
}

TEST_F(CalcTestRational, money5)
{
    parser.SetLocale(Language::French);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₽)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1kop;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(kop)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(€)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1eurocent;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(ct)") << r.ToStdString();

    //SI units in the rational parser under the French locale
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1000m;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(km)") << r.ToStdString();
}

TEST_F(CalcTestRational, money6)
{
    parser.SetLocale(Language::Italian);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1₽;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(₽)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1kop;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(kop)") << r.ToStdString();

    r = parser.Parse(LogicalId{0, 0, 1}, U"1€;", 3);
    ASSERT_TRUE(r.ToStdString() == "1(€)") << r.ToStdString();
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1eurocent;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(ct)") << r.ToStdString();

    //SI units in the rational parser under the Italian locale
    r = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1000m;", 3));
    ASSERT_TRUE(r.ToStdString() == "1(km)") << r.ToStdString();
}

}
