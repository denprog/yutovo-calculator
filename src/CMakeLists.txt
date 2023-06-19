cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.81.0 REQUIRED PATHS ${INSTALL_PATH}/lib/cmake/)

add_library(${PROJECT_NAME}
    parser.cpp
    definition.cpp
    expression.cpp
    integer.cpp
    math_helper.cpp
    rational.cpp
    real.cpp
    script.cpp
    solver.cpp
    utils.cpp
    unit.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    stdc++
    mpfr
    gmp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${INSTALL_PATH}/include
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:${INSTALL_PATH}/include>
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    DESTINATION ${INSTALL_PATH}/lib
    COMPONENT ${PROJECT_NAME}
)

install(
    FILES 
        annotation.h
        ast.h
        definition.h
        error_handler.h
        definition.h
        expression.h
        integer.h
        math_helper.h
        parser_exception.h
        parser.h
        rational.h
        real.h
        script.h
        solver.h
        stdafx.h
        utils.h
        unit.h
    DESTINATION ${INSTALL_PATH}/include/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)

install(
    EXPORT ${PROJECT_NAME}
    DESTINATION ${INSTALL_PATH}/cmake/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}.cmake
)

install(
    FILES ${PROJECT_NAME}-config.cmake
    DESTINATION ${INSTALL_PATH}/cmake/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)
