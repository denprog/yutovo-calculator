cmake_minimum_required(VERSION 3.17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${PROJECT_NAME}_tests
	main.cpp
	integer.cpp
	real.cpp
	rational.cpp
)

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

target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ../src
)

gtest_discover_tests(${PROJECT_NAME}_tests)

install(
    TARGETS ${PROJECT_NAME}_tests
    EXPORT ${PROJECT_NAME}
    DESTINATION ${INSTALL_PATH}/bin
    COMPONENT ${PROJECT_NAME}
)
