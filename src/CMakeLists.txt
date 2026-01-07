cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if(WIN32)
    find_package(PkgConfig)
	set(CMAKE_PREFIX_PATH "${INSTALL_PATH}")
    pkg_check_modules(mpfr REQUIRED IMPORTED_TARGET mpfr)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

if(EMSCRIPTEN)
    set(CMAKE_FIND_ROOT_PATH ${INSTALL_PATH}/wasm/cmake/)
else()
    find_package(Boost 1.83.0 REQUIRED)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        if (NOT WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()
endif()

if(EMSCRIPTEN)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -s USE_PTHREADS=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s DISABLE_EXCEPTION_THROWING=0")
endif()

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT_NAME}
    parser.cpp
    expression.cpp
    integer.cpp
    math_helper.cpp
    rational.cpp
    real.cpp
    complex.cpp
    script.cpp
    solver.cpp
    utils.cpp
    unit.cpp
    export.cpp
 
    stdafx.h
    utils.h
    script.h
    solver.h
    unit.h
    real.h
    rational.h
    parser.h
    parser_exception.h
    math_helper.h
    integer.h
    custom_unit.h
    expression.h
    error_handler.h
    definition.h
    complex.h
    ast.h
    annotation.h
    export.h
    array.h
    graph.h
)

if(EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        stdc++
        ${INSTALL_PATH}/wasm/libmpfr.a
        ${INSTALL_PATH}/wasm/libgmp.a
    )
    target_link_options(${PROJECT_NAME} PUBLIC "-fexceptions -s DISABLE_EXCEPTION_THROWING=0")
    target_compile_options(${PROJECT_NAME} PUBLIC -Wno-overloaded-shift-op-parentheses)
else()
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::mpfr)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE 
            stdc++
            ${INSTALL_PATH}/lib/libmpfr.a
            ${INSTALL_PATH}/lib/libgmp.a
    )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    ${INSTALL_PATH}/include
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:${INSTALL_PATH}/include>
)

if(EMSCRIPTEN)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        DESTINATION ${INSTALL_PATH}/wasm
        COMPONENT ${PROJECT_NAME}
    )
else()
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        DESTINATION ${INSTALL_PATH}/lib
        COMPONENT ${PROJECT_NAME}
    )
endif()

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
        complex.h
        array.h
        custom_unit.h
        script.h
        solver.h
        stdafx.h
        utils.h
        unit.h
        export.h
        graph.h
    DESTINATION ${INSTALL_PATH}/include/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)

if(EMSCRIPTEN)
    install(
        EXPORT ${PROJECT_NAME}
        DESTINATION ${INSTALL_PATH}/wasm/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}.cmake
    )
else()
    install(
        EXPORT ${PROJECT_NAME}
        DESTINATION ${INSTALL_PATH}/lib/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}.cmake
    )
endif()

if(EMSCRIPTEN)
    install(
        FILES ${PROJECT_NAME}-config.cmake
        DESTINATION ${INSTALL_PATH}/wasm/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
    )
else()
    install(
        FILES ${PROJECT_NAME}-config.cmake
        DESTINATION ${INSTALL_PATH}/lib/cmake/${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
    )
endif()
