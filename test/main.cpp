/*
 * Yutovo Calculator
 * Copyright (C) 2022-2025 Yutovo developers. All rights reserved.
 * This file is a part of the Yutovo project
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#ifdef _WIN32
#include <boost/throw_exception.hpp>

namespace boost
{
    BOOST_NORETURN void throw_exception(std::exception const& e)
    {
        throw e;
    }

    BOOST_NORETURN void throw_exception(std::exception const& e, boost::source_location const&)
    {
        throw e;
    }
}
#endif

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
