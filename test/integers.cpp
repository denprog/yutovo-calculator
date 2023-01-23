#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>

namespace yutovo_calc_test
{

using namespace yutovo_calc;
using namespace std::chrono_literals;

TEST_F(CalcTestInteger, integers1)
{
    ASSERT_TRUE(parser.Parse("1+1") == parser.Parse("2"));
}

}
