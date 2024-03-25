#ifndef __MOCK_H__
#define __MOCK_H__

#include <gmock/gmock.h>
#include "parser.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;

struct CalcTestInteger : public testing::Test
{
    CalcTestInteger() :
        parser(0, Language::English, 0)
    {
    }

    Parser<Integer> parser;
};

struct CalcTestReal : public testing::Test
{
    CalcTestReal() :
        parser(3, Language::English, 0)
    {
    }

    bool FindUnit(const std::vector<Unit>& units, const Unit& unit)
    {
        return std::find(units.begin(), units.end(), unit) != units.end();
    }

    Parser<Real> parser;
};

struct CalcTestRational : public testing::Test
{
    CalcTestRational() :
        parser(0, Language::English, 0)
    {
    }

    bool FindUnit(const std::vector<Unit>& units, const Unit& unit)
    {
        return std::find(units.begin(), units.end(), unit) != units.end();
    }

    Parser<Rational> parser;
};

struct CalcTestComplex : public testing::Test
{
    CalcTestComplex() :
        parser(3, Language::English, 0)
    {
    }

    bool FindUnit(const std::vector<Unit>& units, const Unit& unit)
    {
        return std::find(units.begin(), units.end(), unit) != units.end();
    }

    std::string ToString(std::vector<Complex>& numbers)
    {
        std::string res;
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            Complex& n = numbers[i];
            res += n.ToStdString(3, 3);
            if (i < numbers.size() - 1)
                res += ",";
        }
        return res;
    }

    Parser<Complex> parser;
};

}

#endif
