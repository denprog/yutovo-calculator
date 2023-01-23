#ifndef __MOCK_H__
#define __MOCK_H__

#include <gmock/gmock.h>
#include "parser.h"

namespace yutovo_calc_test
{

using namespace yutovo_calc;

struct CalcTestInteger : public testing::Test
{
    CalcTestInteger() :
        parser(10)
    {
    }

    Parser<Integer> parser;
};

}

#endif
