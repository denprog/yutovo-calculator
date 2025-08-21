cmake_minimum_required(VERSION 3.10)

project(yutovo-calculator
    LANGUAGES CXX
)

set(PROJECT_LICENSE "GPL-3.0-only")

message(STATUS "Project license: ${PROJECT_LICENSE}")

if(WIN32)
    set(INSTALL_PATH "c:/Lang/vcpkg/installed/x64-windows")
else()
    set(INSTALL_PATH "$ENV{YUTOVO_DEPLOY}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

option(BUILD_TESTS "Build unit tests" ON)

add_subdirectory(src)
if((NOT DEFINED EMSCRIPTEN) AND BUILD_TESTS)
    add_subdirectory(test)
endif()
