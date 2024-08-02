cmake_minimum_required(VERSION 3.10)

project(yutovo_calculator
    VERSION 0.1.0 
    LANGUAGES CXX
)

set(INSTALL_PATH "$ENV{YUTOVO_DEPLOY}")

add_subdirectory(src)
if(NOT DEFINED EMSCRIPTEN)
    add_subdirectory(test)
endif()
