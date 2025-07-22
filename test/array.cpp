#include <gtest/gtest.h>
#include "mock.h"
#include <chrono>
#include "parser_exception.h"

namespace yutovo_calc_test
{

using namespace yutovo_calculator;
using namespace std::chrono_literals;

TEST_F(CalcTestArray<Real>, arrays1)
{
    auto res = parser.Parse(LogicalId{0, 0, 0, 0, 1}, U"[1,2,3];");
    ASSERT_TRUE(res.ToStdString(3, 3) == "[1.E+0,2.E+0,3.E+0]") << res.ToStdString(3, 3);
}

}
