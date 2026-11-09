/*
 * Yutovo Calculator
 * Copyright (C) 2022-2026 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "parser_exception.h"
#include "export.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

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
    ASSERT_TRUE(s == "1.E+0") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1°;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"1.E+0") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1';");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.0167E+0") << s;

    r = parser.Parse(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, U"1'';");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 0, 0, 0, 2, 0}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.778E-4") << s;
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

TEST_F(CalcTestReal, units68)
{
    parser.SetLocale(Language::Spanish);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"10m;");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"10.E+0(m)") << s;
}

TEST_F(CalcTestReal, units69)
{
    parser.SetLocale(Language::Spanish);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"2día;");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == u8"2.E+0(día)") << s;
}

TEST_F(CalcTestReal, units70)
{
    parser.SetLocale(Language::English);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"2inch{us};");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(inch){us}") << s;
    r = parser.Parse(id, U"2in{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(inch){us}") << s;
}

TEST_F(CalcTestReal, units71)
{
    parser.SetLocale(Language::Russian);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"2дюйм{us};");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(дюйм){us}") << s;
}

TEST_F(CalcTestReal, units72)
{
    parser.SetLocale(Language::Spanish);
    LogicalId id{0, 0, 0, 1};
    auto r = parser.Parse(id, U"2pulgada{us};");
    auto s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(pulgada){us}") << s;
    r = parser.Parse(id, U"2in{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(pulgada){us}") << s;
}

TEST_F(CalcTestReal, units73)
{
    parser.SetLocale(Language::Russian);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(1*Гц*1*мкФ);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(МОм)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"(1)/(Гц*мкФ);")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(МОм)") << s;
}

TEST_F(CalcTestReal, units74)
{
    parser.SetLocale(Language::English);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.789*m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.789E+0(m)") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"0.871*m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.871E+0(m)") << s;
}

TEST_F(CalcTestReal, units75)
{
    parser.SetLocale(Language::Spanish);
    parser.Parse({0, 0, 0, 0, 1}, U"L~pow(m,3);");
    parser.Parse({0, 0, 0, 0, 2}, U"r~2*s;");
    parser.Parse({0, 0, 0, 0, 3}, U"rp~((r)/(s));");
    parser.Parse({0, 0, 0, 0, 5, 0, 0}, U"Dt=root(L,3);");
    Real r = parser.Parse({0, 0, 0, 5, 0, 1}, U"Dt;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 5, 0, 1}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;

    std::vector<Unit> cast_units;
    parser.ClearCastUnits();
    parser.GetCastUnits({0, 0, 0, 5, 0, 1}, r, cast_units);
    ASSERT_TRUE(FindUnit(cast_units, Unit(U"m")));

    Unit u;
    ASSERT_TRUE(u.FromString(U"arshin{rus}"));
    s = parser.CastToUnit(LogicalId{0, 0, 0, 5, 0, 1}, r, u).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.406E+0(arshin){rus}") << s;
}

TEST_F(CalcTestReal, units76)
{
    parser.SetLocale(Language::Russian);
    Real r = parser.Parse({0, 0, 0, 0, 1}, U"0.*Кл;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 1}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "0.E+0(Кл)") << s;
}

TEST_F(CalcTestReal, units77)
{
    auto exports = std::make_shared<yutovo_calculator::Export>();

    Unit meter_unit(U"meter");
    Unit foot_unit(U"foot");
    Real meter_value(3, "3.28084");
    meter_value.unit = foot_unit;
    Real foot_value(3, "0.3048");
    foot_value.unit = meter_unit;

    exports->AddUnit<Real>(CustomUnit<Real>(LogicalId{0, 0, 0, 0, 1}, U"meter", U"SI", meter_value, false));
    exports->AddUnit<Real>(CustomUnit<Real>(LogicalId{0, 0, 0, 0, 2}, U"foot", U"SI", foot_value, false));

    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(1000);
    parser_context.exports = exports;
    auto r = parser.Parse(LogicalId{0, 0, 0, 0, 3}, U"5meter;", &parser_context);
    EXPECT_NO_THROW(parser.GetSuitableUnit(LogicalId{0, 0, 0, 0, 3}, r).ToStdString(3, 3));
}

TEST_F(CalcTestReal, units78)
{
    yutovo_calculator::Solver<yutovo_calculator::Real> solver(3, yutovo_calculator::AngleMeasure::Radian);

    yutovo_calculator::Unit meter_unit(U"meter");
    yutovo_calculator::Unit foot_unit(U"foot");
    yutovo_calculator::Real meter_value(3, "3.28084");
    meter_value.unit = foot_unit;
    yutovo_calculator::Real foot_value(3, "0.3048");
    foot_value.unit = meter_unit;

    solver.symbols->units.push_back(yutovo_calculator::CustomUnit<yutovo_calculator::Real>(LogicalId{0, 0, 0, 0, 1}, U"meter", U"SI", meter_value, false));
    solver.symbols->units.push_back(yutovo_calculator::CustomUnit<yutovo_calculator::Real>(LogicalId{0, 0, 0, 0, 2}, U"foot", U"SI", foot_value, false));

    yutovo_calculator::Real val(3, "5");
    val.unit = meter_unit;
    yutovo_calculator::ParserContext parser_context;
    parser_context.Init(1000);
    solver.SetParserContext(&parser_context);

    std::vector<yutovo_calculator::Unit> cast_units;
    EXPECT_NO_THROW(solver.GetCastUnits(LogicalId{0, 0, 0, 0, 3}, val, U"SI", cast_units));
}

TEST_F(CalcTestReal, units79)
{
    // 1 angstrom = 1e-10 m in every locale
    LogicalId id{0, 0, 0, 1};
    for (Language language :
        {Language::English, Language::Spanish, Language::BrazilianPortuguese, Language::German, Language::French, Language::Italian})
    {
        parser.SetLocale(language);
        std::string s = parser.Parse(id, U"1ang/m;").ToStdString(3, 3);
        ASSERT_TRUE(s == "1.E-10") << s;
    }

    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(id, U"1анг/м;").ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E-10") << s;
}

TEST_F(CalcTestReal, units80)
{
    //1 vedro = 10 shtof = 12.3 l, 1 bochka = 40 vedro = 492 l in every locale
    LogicalId id{0, 0, 0, 1};
    for (Language language :
        {Language::English, Language::Spanish, Language::BrazilianPortuguese, Language::German, Language::French, Language::Italian})
    {
        parser.SetLocale(language);
        std::string s = parser.Parse(id, U"1vedro{rus}/l;").ToStdString(3, 3);
        ASSERT_TRUE(s == "12.3E+0") << s;
        s = parser.Parse(id, U"1bochka{rus}/l;").ToStdString(3, 3);
        ASSERT_TRUE(s == "492.E+0") << s;
    }

    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(id, U"1ведро{rus}/л;").ToStdString(3, 3);
    ASSERT_TRUE(s == "12.3E+0") << s;
    s = parser.Parse(id, U"1бочка{rus}/л;").ToStdString(3, 3);
    ASSERT_TRUE(s == "492.E+0") << s;
}

TEST_F(CalcTestReal, units81)
{
    LogicalId id{0, 0, 0, 1};
    for (Language language : 
        {Language::English, Language::Spanish, Language::BrazilianPortuguese, Language::German, Language::French, Language::Italian})
    {
        parser.SetLocale(language);
        std::string s = parser.Parse(id, U"1lot{rus}/g;").ToStdString(3, 3);
        ASSERT_TRUE(s == "12.798E+0") << s;
    }

    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(id, U"1лот{rus}/г;").ToStdString(3, 3);
    ASSERT_TRUE(s == "12.798E+0") << s;
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

TEST_F(CalcTestReal, money3)
{
    parser.SetLocale(Language::Russian);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1¥;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(¥)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"1юань;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(¥)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1₹;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(₹)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"1рупия;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(₹)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 1}, U"1R$;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(R$)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 1}, U"1реал;", 3);
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(R$)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, pt_BR_language_to_string)
{
    ASSERT_TRUE(LanguageToString(Language::BrazilianPortuguese) == "Portuguese (Brazil)");
}

TEST_F(CalcTestReal, german_language_to_string)
{
    ASSERT_TRUE(LanguageToString(Language::German) == "German");
}

TEST_F(CalcTestReal, german_units_and_currencies)
{
    parser.SetLocale(Language::German);

    //SI units are available under the German locale with the international identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"0.2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(dm)") << s;

    //angle identifiers
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 5}, U"2rad;");
    ASSERT_TRUE(r.GetAngleMeasure() == AngleMeasure::Radian) << r.ToStdString(3, 3);

    //currencies
    r = parser.Parse(LogicalId{0, 0, 6}, U"1₽*2;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0(₽)") << r.ToStdString(3, 3);

    //physical constants: speed of light is available under the German locale
    r = parser.Parse(LogicalId{0, 0, 7}, U"c;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.998E+8((m)/(s))") << r.ToStdString(3, 3);

    //Russian and American unit systems use German word identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"1werst{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(werst){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 9}, parser.Parse(LogicalId{0, 0, 9}, U"1zoll{us};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(zoll){us}") << s;
}

TEST_F(CalcTestReal, german_builtin_unit_names)
{
    parser.SetLocale(Language::German);

    //base units and currencies: ListBuiltinUnits
    std::vector<std::pair<std::u32string, std::u32string>> builtin_units;
    parser.ListBuiltinUnits(builtin_units);
    auto has = [&builtin_units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(builtin_units.begin(), builtin_units.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != builtin_units.end();
        };
    EXPECT_TRUE(has(U"m", U"Meter"));
    EXPECT_TRUE(has(U"kg", U"Kilogramm"));
    EXPECT_TRUE(has(U"s", U"Sekunde"));
    EXPECT_TRUE(has(U"A", U"Ampere"));
    EXPECT_TRUE(has(U"cd", U"Candela"));
    EXPECT_TRUE(has(U"K", U"Kelvin"));
    EXPECT_TRUE(has(U"mol", U"Mol"));
    EXPECT_TRUE(has(U"sr", U"Steradiant"));
    EXPECT_TRUE(has(U"₽", U"Rubel"));
    EXPECT_TRUE(has(U"$", U"Dollar"));
    EXPECT_TRUE(has(U"€", U"Euro"));
    EXPECT_TRUE(has(U"¥", U"Yuan"));
    EXPECT_TRUE(has(U"₹", U"Rupie"));
    EXPECT_TRUE(has(U"R$", U"Real"));

    //derived units (SI prefixes, time words, unit systems): ListUserUnits
    std::vector<CustomUnit<Real>> units;
    parser.ListUserUnits(units);
    auto has_unit = [&units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.description == name;
                }) != units.end();
        };
    EXPECT_TRUE(has_unit(U"nm", U"Nanometer"));
    EXPECT_TRUE(has_unit(U"mcm", U"Mikrometer"));
    EXPECT_TRUE(has_unit(U"km", U"Kilometer"));
    EXPECT_TRUE(has_unit(U"mg", U"Milligramm"));
    EXPECT_TRUE(has_unit(U"kV", U"Kilovolt"));
    EXPECT_TRUE(has_unit(U"mcWb", U"Mikroweber"));
    EXPECT_TRUE(has_unit(U"mT", U"Millitesla"));
    EXPECT_TRUE(has_unit(U"kOhm", U"Kiloohm"));
    EXPECT_TRUE(has_unit(U"nF", U"Nanofarad"));
    EXPECT_TRUE(has_unit(U"mcGy", U"Mikrogray"));
    EXPECT_TRUE(has_unit(U"l", U"Liter"));
    EXPECT_TRUE(has_unit(U"ml", U"Milliliter"));
    EXPECT_TRUE(has_unit(U"ha", U"Hektar"));
    EXPECT_TRUE(has_unit(U"ang", U"Angström"));

    //word identifiers carry no display description (like in the English tree)
    auto has_unit_id = [&units](const std::u32string& id)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id;
                }) != units.end();
        };

    //German time identifiers
    EXPECT_TRUE(has_unit_id(U"stunde"));
    EXPECT_TRUE(has_unit_id(U"tag"));
    EXPECT_TRUE(has_unit_id(U"woche"));
    EXPECT_TRUE(has_unit_id(U"jahr"));

    //German word identifiers of the American and Russian unit systems
    auto has_sys_unit = [&units](const std::u32string& id, const std::u32string& system)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.system == system;
                }) != units.end();
        };

    EXPECT_TRUE(has_sys_unit(U"zoll", U"us"));
    EXPECT_TRUE(has_sys_unit(U"fuß", U"us"));
    EXPECT_TRUE(has_sys_unit(U"meile", U"us"));
    EXPECT_TRUE(has_sys_unit(U"pfund", U"us"));
    EXPECT_TRUE(has_sys_unit(U"gallone", U"us"));
    EXPECT_TRUE(has_sys_unit(U"teelöffel", U"us"));
    EXPECT_TRUE(has_sys_unit(U"werst", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"arschin", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"pfund", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"solotnik", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"glas", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"stunde", U"rus"));

    //physical constants: ListUserVariables
    std::vector<std::pair<std::u32string, std::u32string>> variables;
    parser.ListUserVariables(variables);
    auto has_var = [&variables](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(variables.begin(), variables.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != variables.end();
        };

    EXPECT_TRUE(has_var(U"c", U"Lichtgeschwindigkeit"));
    EXPECT_TRUE(has_var(U"G", U"Gravitationskonstante"));
    EXPECT_TRUE(has_var(U"h", U"Plancksche Konstante"));
    EXPECT_TRUE(has_var(U"k", U"Boltzmannsche Konstante"));
    EXPECT_TRUE(has_var(U"e_c", U"Elementarladung"));
    EXPECT_TRUE(has_var(U"g_a", U"Fallbeschleunigung"));
}

TEST_F(CalcTestReal, german_word_units_parse)
{
    parser.SetLocale(Language::German);

    //German time identifiers
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2jahr;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.307E+7(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"1woche;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.048E+5(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 3}, U"1stunde;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "3.6E+3(s)") << r.ToStdString(3, 3);

    //American system with German identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1fuß{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(fuß){us}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1gallone{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(gallone){us}") << s;

    //Russian system with German identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 6}, parser.Parse(LogicalId{0, 0, 6}, U"1arschin{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(arschin){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 7}, parser.Parse(LogicalId{0, 0, 7}, U"1solotnik{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(solotnik){rus}") << s;
}

TEST_F(CalcTestReal, german_angle_identifiers)
{
    parser.SetLocale(Language::German);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"sin(100grad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 3}, U"sin(1.5707963rad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, german_constructor_locale)
{
    //the parser must also work when constructed directly with the German locale
    Parser<Real> german_parser(3, Language::German);
    std::string s = german_parser.GetSuitableUnit(LogicalId{0, 0, 1}, german_parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;
    auto r = german_parser.Parse(LogicalId{0, 0, 2}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, german_locale_switch)
{
    parser.SetLocale(Language::German);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    //switching away and back must re-register the German (aliased) unit lists
    parser.SetLocale(Language::Russian);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1км;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(км)") << s;

    parser.SetLocale(Language::German);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, french_language_to_string)
{
    ASSERT_TRUE(LanguageToString(Language::French) == "French");
}

TEST_F(CalcTestReal, french_units_and_currencies)
{
    parser.SetLocale(Language::French);

    //SI units are available under the French locale with the international identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"0.2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(dm)") << s;

    //angle identifiers
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 5}, U"2rad;");
    ASSERT_TRUE(r.GetAngleMeasure() == AngleMeasure::Radian) << r.ToStdString(3, 3);

    //currencies
    r = parser.Parse(LogicalId{0, 0, 6}, U"1₽*2;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0(₽)") << r.ToStdString(3, 3);

    //physical constants: speed of light is available under the French locale
    r = parser.Parse(LogicalId{0, 0, 7}, U"c;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.998E+8((m)/(s))") << r.ToStdString(3, 3);

    //Russian and American unit systems use French word identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"1verste{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(verste){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 9}, parser.Parse(LogicalId{0, 0, 9}, U"1pouce{us};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(pouce){us}") << s;
}

TEST_F(CalcTestReal, french_builtin_unit_names)
{
    parser.SetLocale(Language::French);

    //base units and currencies: ListBuiltinUnits
    std::vector<std::pair<std::u32string, std::u32string>> builtin_units;
    parser.ListBuiltinUnits(builtin_units);
    auto has = [&builtin_units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(builtin_units.begin(), builtin_units.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != builtin_units.end();
        };
    EXPECT_TRUE(has(U"m", U"mètre"));
    EXPECT_TRUE(has(U"kg", U"kilogramme"));
    EXPECT_TRUE(has(U"s", U"seconde"));
    EXPECT_TRUE(has(U"A", U"ampère"));
    EXPECT_TRUE(has(U"cd", U"candela"));
    EXPECT_TRUE(has(U"K", U"kelvin"));
    EXPECT_TRUE(has(U"mol", U"mole"));
    EXPECT_TRUE(has(U"sr", U"stéradian"));
    EXPECT_TRUE(has(U"₽", U"rouble"));
    EXPECT_TRUE(has(U"$", U"dollar"));
    EXPECT_TRUE(has(U"€", U"euro"));
    EXPECT_TRUE(has(U"¥", U"yuan"));
    EXPECT_TRUE(has(U"₹", U"roupie"));
    EXPECT_TRUE(has(U"R$", U"réal"));

    //derived units (SI prefixes, time words, unit systems): ListUserUnits
    std::vector<CustomUnit<Real>> units;
    parser.ListUserUnits(units);
    auto has_unit = [&units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.description == name;
                }) != units.end();
        };
    EXPECT_TRUE(has_unit(U"nm", U"nanomètre"));
    EXPECT_TRUE(has_unit(U"mcm", U"micromètre"));
    EXPECT_TRUE(has_unit(U"km", U"kilomètre"));
    EXPECT_TRUE(has_unit(U"mg", U"milligramme"));
    EXPECT_TRUE(has_unit(U"kV", U"kilovolt"));
    EXPECT_TRUE(has_unit(U"mcWb", U"microweber"));
    EXPECT_TRUE(has_unit(U"mT", U"millitesla"));
    EXPECT_TRUE(has_unit(U"kOhm", U"kiloohm"));
    EXPECT_TRUE(has_unit(U"nF", U"nanofarad"));
    EXPECT_TRUE(has_unit(U"mcGy", U"microgray"));
    EXPECT_TRUE(has_unit(U"l", U"litre"));
    EXPECT_TRUE(has_unit(U"ml", U"millilitre"));
    EXPECT_TRUE(has_unit(U"ha", U"hectare"));
    EXPECT_TRUE(has_unit(U"ang", U"angström"));

    //word identifiers carry no display description (like in the English tree)
    auto has_unit_id = [&units](const std::u32string& id)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id;
                }) != units.end();
        };

    //French time identifiers
    EXPECT_TRUE(has_unit_id(U"heure"));
    EXPECT_TRUE(has_unit_id(U"jour"));
    EXPECT_TRUE(has_unit_id(U"semaine"));
    EXPECT_TRUE(has_unit_id(U"an"));

    //French word identifiers of the American and Russian unit systems
    auto has_sys_unit = [&units](const std::u32string& id, const std::u32string& system)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.system == system;
                }) != units.end();
        };

    EXPECT_TRUE(has_sys_unit(U"pouce", U"us"));
    EXPECT_TRUE(has_sys_unit(U"pied", U"us"));
    EXPECT_TRUE(has_sys_unit(U"mille", U"us"));
    EXPECT_TRUE(has_sys_unit(U"livre", U"us"));
    EXPECT_TRUE(has_sys_unit(U"gallon", U"us"));
    EXPECT_TRUE(has_sys_unit(U"cuillere_cafe", U"us"));
    EXPECT_TRUE(has_sys_unit(U"verste", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"archine", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"livre", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"zolotnik", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"verre", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"heure", U"rus"));

    //physical constants: ListUserVariables
    std::vector<std::pair<std::u32string, std::u32string>> variables;
    parser.ListUserVariables(variables);
    auto has_var = [&variables](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(variables.begin(), variables.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != variables.end();
        };

    EXPECT_TRUE(has_var(U"c", U"vitesse de la lumière"));
    EXPECT_TRUE(has_var(U"G", U"constante gravitationnelle"));
    EXPECT_TRUE(has_var(U"h", U"constante de Planck"));
    EXPECT_TRUE(has_var(U"k", U"constante de Boltzmann"));
    EXPECT_TRUE(has_var(U"e_c", U"charge élémentaire"));
    EXPECT_TRUE(has_var(U"g_a", U"accélération de la pesanteur"));
}

TEST_F(CalcTestReal, french_word_units_parse)
{
    parser.SetLocale(Language::French);

    //French time identifiers
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2an;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.307E+7(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"1semaine;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.048E+5(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 3}, U"1heure;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "3.6E+3(s)") << r.ToStdString(3, 3);

    //American system with French identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1pied{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(pied){us}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1gallon{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(gallon){us}") << s;

    //Russian system with French identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 6}, parser.Parse(LogicalId{0, 0, 6}, U"1archine{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(archine){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 7}, parser.Parse(LogicalId{0, 0, 7}, U"1zolotnik{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(zolotnik){rus}") << s;
}

TEST_F(CalcTestReal, french_angle_identifiers)
{
    parser.SetLocale(Language::French);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"sin(100grad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 3}, U"sin(1.5707963rad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, french_constructor_locale)
{
    //the parser must also work when constructed directly with the French locale
    Parser<Real> french_parser(3, Language::French);
    std::string s = french_parser.GetSuitableUnit(LogicalId{0, 0, 1}, french_parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;
    auto r = french_parser.Parse(LogicalId{0, 0, 2}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, french_locale_switch)
{
    parser.SetLocale(Language::French);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    //switching away and back must re-register the French unit lists
    parser.SetLocale(Language::Russian);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1км;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(км)") << s;

    parser.SetLocale(Language::French);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, italian_language_to_string)
{
    ASSERT_TRUE(LanguageToString(Language::Italian) == "Italian");
}

TEST_F(CalcTestReal, italian_units_and_currencies)
{
    parser.SetLocale(Language::Italian);

    //SI units are available under the Italian locale with the international identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"0.2m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "2.E+0(dm)") << s;

    //angle identifiers
    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 5}, U"2rad;");
    ASSERT_TRUE(r.GetAngleMeasure() == AngleMeasure::Radian) << r.ToStdString(3, 3);

    //currencies
    r = parser.Parse(LogicalId{0, 0, 6}, U"1₽*2;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.E+0(₽)") << r.ToStdString(3, 3);

    //physical constants: speed of light is available under the Italian locale
    r = parser.Parse(LogicalId{0, 0, 7}, U"c;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "2.998E+8((m)/(s))") << r.ToStdString(3, 3);

    //Russian and American unit systems use Italian word identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"1versta{rus};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(versta){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 9}, parser.Parse(LogicalId{0, 0, 9}, U"1pollice{us};")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(pollice){us}") << s;
}

TEST_F(CalcTestReal, italian_builtin_unit_names)
{
    parser.SetLocale(Language::Italian);

    //base units and currencies: ListBuiltinUnits
    std::vector<std::pair<std::u32string, std::u32string>> builtin_units;
    parser.ListBuiltinUnits(builtin_units);
    auto has = [&builtin_units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(builtin_units.begin(), builtin_units.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != builtin_units.end();
        };
    EXPECT_TRUE(has(U"m", U"metro"));
    EXPECT_TRUE(has(U"kg", U"chilogrammo"));
    EXPECT_TRUE(has(U"s", U"secondo"));
    EXPECT_TRUE(has(U"A", U"ampere"));
    EXPECT_TRUE(has(U"cd", U"candela"));
    EXPECT_TRUE(has(U"K", U"kelvin"));
    EXPECT_TRUE(has(U"mol", U"mole"));
    EXPECT_TRUE(has(U"sr", U"steradiante"));
    EXPECT_TRUE(has(U"₽", U"rublo"));
    EXPECT_TRUE(has(U"$", U"dollaro"));
    EXPECT_TRUE(has(U"€", U"euro"));
    EXPECT_TRUE(has(U"¥", U"yuan"));
    EXPECT_TRUE(has(U"₹", U"rupia"));
    EXPECT_TRUE(has(U"R$", U"real"));

    //derived units (SI prefixes, time words, unit systems): ListUserUnits
    std::vector<CustomUnit<Real>> units;
    parser.ListUserUnits(units);
    auto has_unit = [&units](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.description == name;
                }) != units.end();
        };
    EXPECT_TRUE(has_unit(U"nm", U"nanometro"));
    EXPECT_TRUE(has_unit(U"mcm", U"micrometro"));
    EXPECT_TRUE(has_unit(U"km", U"chilometro"));
    EXPECT_TRUE(has_unit(U"mg", U"milligrammo"));
    EXPECT_TRUE(has_unit(U"kV", U"kilovolt"));
    EXPECT_TRUE(has_unit(U"mcWb", U"microweber"));
    EXPECT_TRUE(has_unit(U"mT", U"millitesla"));
    EXPECT_TRUE(has_unit(U"kOhm", U"kiloohm"));
    EXPECT_TRUE(has_unit(U"nF", U"nanofarad"));
    EXPECT_TRUE(has_unit(U"mcGy", U"microgray"));
    EXPECT_TRUE(has_unit(U"l", U"litro"));
    EXPECT_TRUE(has_unit(U"ml", U"millilitro"));
    EXPECT_TRUE(has_unit(U"ha", U"ettaro"));
    EXPECT_TRUE(has_unit(U"ang", U"angstrom"));

    //word identifiers carry no display description (like in the English tree)
    auto has_unit_id = [&units](const std::u32string& id)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id;
                }) != units.end();
        };

    //Italian time identifiers
    EXPECT_TRUE(has_unit_id(U"ora"));
    EXPECT_TRUE(has_unit_id(U"giorno"));
    EXPECT_TRUE(has_unit_id(U"settimana"));
    EXPECT_TRUE(has_unit_id(U"anno"));

    //Italian word identifiers of the American and Russian unit systems
    auto has_sys_unit = [&units](const std::u32string& id, const std::u32string& system)
        {
            return std::find_if(units.begin(), units.end(),
                [&](auto& p)
                {
                    return p.name == id && p.system == system;
                }) != units.end();
        };

    EXPECT_TRUE(has_sys_unit(U"pollice", U"us"));
    EXPECT_TRUE(has_sys_unit(U"piede", U"us"));
    EXPECT_TRUE(has_sys_unit(U"miglio", U"us"));
    EXPECT_TRUE(has_sys_unit(U"libbra", U"us"));
    EXPECT_TRUE(has_sys_unit(U"gallone", U"us"));
    EXPECT_TRUE(has_sys_unit(U"cucchiaino", U"us"));
    EXPECT_TRUE(has_sys_unit(U"versta", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"arscin", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"libbra", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"zolotnik", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"bicchiere", U"rus"));
    EXPECT_TRUE(has_sys_unit(U"ora", U"rus"));

    //physical constants: ListUserVariables
    std::vector<std::pair<std::u32string, std::u32string>> variables;
    parser.ListUserVariables(variables);
    auto has_var = [&variables](const std::u32string& id, const std::u32string& name)
        {
            return std::find_if(variables.begin(), variables.end(),
                [&](auto& p)
                {
                    return p.first == id && p.second == name;
                }) != variables.end();
        };

    EXPECT_TRUE(has_var(U"c", U"velocità della luce"));
    EXPECT_TRUE(has_var(U"G", U"costante gravitazionale"));
    EXPECT_TRUE(has_var(U"h", U"costante di Planck"));
    EXPECT_TRUE(has_var(U"k", U"costante di Boltzmann"));
    EXPECT_TRUE(has_var(U"e_c", U"carica elementare"));
    EXPECT_TRUE(has_var(U"g_a", U"accelerazione di gravità"));
}

TEST_F(CalcTestReal, italian_word_units_parse)
{
    parser.SetLocale(Language::Italian);

    //Italian time identifiers
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"2anno;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.307E+7(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 2}, U"1settimana;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "6.048E+5(s)") << r.ToStdString(3, 3);
    r = parser.Parse(LogicalId{0, 0, 3}, U"1ora;");
    EXPECT_TRUE(r.ToStdString(3, 3) == "3.6E+3(s)") << r.ToStdString(3, 3);

    //American system with Italian identifiers
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1piede{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(piede){us}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1gallone{us};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(gallone){us}") << s;

    //Russian system with Italian identifiers
    s = parser.GetSuitableUnit(LogicalId{0, 0, 6}, parser.Parse(LogicalId{0, 0, 6}, U"1arscin{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(arscin){rus}") << s;
    s = parser.GetSuitableUnit(LogicalId{0, 0, 7}, parser.Parse(LogicalId{0, 0, 7}, U"1zolotnik{rus};")).ToStdString(3, 3);
    EXPECT_TRUE(s == "1.E+0(zolotnik){rus}") << s;
}

TEST_F(CalcTestReal, italian_angle_identifiers)
{
    parser.SetLocale(Language::Italian);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"sin(90°);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"sin(100grad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 3}, U"sin(1.5707963rad);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, italian_constructor_locale)
{
    //the parser must also work when constructed directly with the Italian locale
    Parser<Real> italian_parser(3, Language::Italian);
    std::string s = italian_parser.GetSuitableUnit(LogicalId{0, 0, 1}, italian_parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;
    auto r = italian_parser.Parse(LogicalId{0, 0, 2}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, italian_locale_switch)
{
    parser.SetLocale(Language::Italian);
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    //switching away and back must re-register the Italian unit lists
    parser.SetLocale(Language::Russian);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1км;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(км)") << s;

    parser.SetLocale(Language::Italian);
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    auto r = parser.Parse(LogicalId{0, 0, 4}, U"sin(90deg);");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, pt_BR_si_units_distance)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(m)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1km;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1000m;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(km)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1cm;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(cm)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1mm;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(mm)") << s;
}

TEST_F(CalcTestReal, pt_BR_si_units_time)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(s)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1min;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(min)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"60s;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(min)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1ms;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(ms)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1000ms;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(s)") << s;
}

TEST_F(CalcTestReal, pt_BR_si_units_mass)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1kg;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(kg)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1000g;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(kg)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1ton;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(ton)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1mg;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(g)") << s;
}

TEST_F(CalcTestReal, pt_BR_si_units_electric)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1A;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(A)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1V;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(V)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1Ohm;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(Ohm)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1F;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(F)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1W;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(W)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 6}, parser.Parse(LogicalId{0, 0, 6}, U"1J;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(J)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 7}, parser.Parse(LogicalId{0, 0, 7}, U"1Pa;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(Pa)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 8}, parser.Parse(LogicalId{0, 0, 8}, U"1T;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(T)") << s;
}

TEST_F(CalcTestReal, pt_BR_other_units)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, parser.Parse(LogicalId{0, 0, 1}, U"1l;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(l)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, parser.Parse(LogicalId{0, 0, 2}, U"1ml;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(ml)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, parser.Parse(LogicalId{0, 0, 3}, U"1byte;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "8.E+0(bit)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, parser.Parse(LogicalId{0, 0, 4}, U"1kbyte;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(kbyte)") << s;

    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, parser.Parse(LogicalId{0, 0, 5}, U"1ha;")).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(ha)") << s;
}

TEST_F(CalcTestReal, pt_BR_physical_constants)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"v_l;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("2.998E+8") == 0) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"G;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("6.674E-11") == 0) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 3}, U"h;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("6.626E-34") == 0) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 4}, U"k;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("1.381E-23") == 0) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 5}, U"e_c;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("1.602E-19") != std::string::npos) << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 6}, U"a_g;");
    ASSERT_TRUE(r.ToStdString(3, 3).find("9.807E+0") == 0) << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, physical_constants1)
{
    LogicalId id{0, 0, 0, 1};
    for (Language language :
        {Language::English, Language::Spanish, Language::BrazilianPortuguese, Language::German, Language::French, Language::Italian})
    {
        parser.SetLocale(language);
        std::string s = parser.Parse(id, U"G;").ToStdString(3, 6);
        ASSERT_TRUE(s == "6.6743E-11((m^3)/(kg*s^2))") << s;
    }

    parser.SetLocale(Language::Russian);
    std::string s = parser.Parse(id, U"G;").ToStdString(3, 6);
    ASSERT_TRUE(s == u8"6.6743E-11((м^3)/(кг*с^2))") << s;
}

TEST_F(CalcTestReal, pt_BR_us_units)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1polegada{us};");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("polegada") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 2}, U"1jarda{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("jarda") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 3}, U"1milha{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("milha") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 4}, U"1libra{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("libra") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 5}, U"1galão{us};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 5}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("galão") != std::string::npos) << s;
}

TEST_F(CalcTestReal, pt_BR_russian_units)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1ponto{rus};");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("ponto") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 2}, U"10linha{rus};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 2}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("polegada") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 3}, U"1copo{rus};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("copo") != std::string::npos) << s;

    r = parser.Parse(LogicalId{0, 0, 4}, U"500sazhen{rus};");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 4}, r).ToStdString(3, 3);
    ASSERT_TRUE(s.find("versta") != std::string::npos) << s;
}

TEST_F(CalcTestReal, pt_BR_currency)
{
    parser.SetLocale(Language::BrazilianPortuguese);

    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1R$;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(R$)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 2}, U"1real;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(R$)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 3}, U"1dólar;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0($)") << r.ToStdString(3, 3);

    r = parser.Parse(LogicalId{0, 0, 4}, U"1euro;");
    ASSERT_TRUE(r.ToStdString(3, 3) == "1.E+0(€)") << r.ToStdString(3, 3);
}

TEST_F(CalcTestReal, pt_locale_switch_back)
{
    parser.SetLocale(Language::BrazilianPortuguese);
    auto r = parser.Parse(LogicalId{0, 0, 1}, U"1hora;");
    std::string s = parser.GetSuitableUnit(LogicalId{0, 0, 1}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(hora)") << s;

    parser.SetLocale(Language::English);
    EXPECT_THROW(parser.Parse(LogicalId{0, 0, 2}, U"1hora;"), yutovo_calculator::SyntaxException);
    r = parser.Parse(LogicalId{0, 0, 3}, U"1hour;");
    s = parser.GetSuitableUnit(LogicalId{0, 0, 3}, r).ToStdString(3, 3);
    ASSERT_TRUE(s == "1.E+0(hour)") << s;
}

}
