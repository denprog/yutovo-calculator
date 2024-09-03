cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if(EMSCRIPTEN)
    set(CMAKE_FIND_ROOT_PATH ${INSTALL_PATH}/wasm/cmake/)
else()
    find_package(Boost 1.81.0 REQUIRED PATHS ${INSTALL_PATH}/lib/cmake/)
endif()

if(EMSCRIPTEN)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -s USE_PTHREADS=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s NO_DISABLE_EXCEPTION_CATCHING=1")
endif()

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT_NAME}
    parser.cpp
    definition.cpp
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
)

if(EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        stdc++
        ${INSTALL_PATH}/wasm/libmpfr.a
        ${INSTALL_PATH}/wasm/libgmp.a
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        stdc++
        mpfr
        gmp
    )
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
        script.h
        solver.h
        stdafx.h
        utils.h
        unit.h
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
