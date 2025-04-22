cmake_minimum_required(VERSION 3.10)

project(yutovo_calculator
    VERSION 0.1.0 
    LANGUAGES CXX
)

if(NOT WIN32)
    set(INSTALL_PATH "$ENV{YUTOVO_DEPLOY}")
endif()

add_subdirectory(src)
if(NOT DEFINED EMSCRIPTEN)
    add_subdirectory(test)
endif()
