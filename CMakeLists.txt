cmake_minimum_required(VERSION 3.10)

if(WIN32 AND POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(yutovo-calculator LANGUAGES CXX C)

set(PROJECT_LICENSE "GPL-3.0-only")

message(STATUS "Project license: ${PROJECT_LICENSE}")
message(STATUS "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

if(WIN32)
    set(INSTALL_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /bigobj /EHsc /MP")
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
    set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
    set(CMAKE_C_FLAGS_DEBUG "/Zi /Od")
else()
    set(INSTALL_PATH "$ENV{YUTOVO_DEPLOY}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT WIN32)
        add_compile_definitions(DEBUG)
    endif()
endif()

option(BUILD_TESTS "Build unit tests" ON)

add_subdirectory(src)
if((NOT DEFINED EMSCRIPTEN) AND BUILD_TESTS)
    add_subdirectory(test)
endif()
