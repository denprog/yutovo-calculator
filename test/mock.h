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
        parser(0)
    {
    }

    Parser<Integer> parser;
};

struct CalcTestReal : public testing::Test
{
    CalcTestReal() :
        parser(3)
    {
    }

    Parser<Real> parser;
};

struct CalcTestRational : public testing::Test
{
    CalcTestRational() :
        parser(0)
    {
    }

    Parser<Rational> parser;
};

}

#endif
