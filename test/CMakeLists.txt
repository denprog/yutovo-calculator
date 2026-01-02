cmake_minimum_required(VERSION 3.17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest REQUIRED PATHS ${INSTALL_PATH}/share/gtest)

add_executable(${PROJECT_NAME}_tests
	main.cpp
	integer.cpp
	real.cpp
	rational.cpp
	complex.cpp
	array.cpp
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME}_tests PRIVATE
		${PROJECT_NAME}
	)
	target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
else()
	target_link_libraries(${PROJECT_NAME}_tests PRIVATE
		stdc++
	    pthread
		gtest
		gtest_main
		gmock
		gmock_main
		m
		dl
		${PROJECT_NAME}
	)
endif()

target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ../src
)

if(NOT WIN32)
	include(GoogleTest)
	gtest_discover_tests(${PROJECT_NAME}_tests)
endif()

install(
    TARGETS ${PROJECT_NAME}_tests
    EXPORT ${PROJECT_NAME}
    DESTINATION ${INSTALL_PATH}/bin
    COMPONENT ${PROJECT_NAME}
)
