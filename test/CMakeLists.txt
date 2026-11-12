cmake_minimum_required(VERSION 3.17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

if(WIN32)
	find_package(GTest CONFIG REQUIRED)
else()
	find_package(GTest REQUIRED PATHS ${INSTALL_PATH}/share/gtest)
endif()

add_executable(${PROJECT_NAME}_tests
	main.cpp
	utils.cpp
	integer_numbers.cpp
	integer_notation.cpp
	integer_variables.cpp
	integer_functions.cpp
	integer_loops.cpp
	real_numbers.cpp
	real_variables.cpp
	real_functions.cpp
	real_trigonometric.cpp
	real_units.cpp
	real_integrals.cpp
	real_derivatives.cpp
	real_limits.cpp
	real_loops.cpp
	rational_numbers.cpp
	rational_variables.cpp
	rational_functions.cpp
	rational_units.cpp
	rational_integrals.cpp
	rational_derivatives.cpp
	rational_limits.cpp
	rational_loops.cpp
	complex_numbers.cpp
	complex_variables.cpp
	complex_functions.cpp
	complex_trigonometric.cpp
	complex_integrals.cpp
	complex_derivatives.cpp
	complex_limits.cpp
	complex_loops.cpp
	real_array_numbers.cpp
	real_array_variables.cpp
	real_array_functions.cpp
	real_array_trigonometric.cpp
	real_array_graphs.cpp
	real_array_units.cpp
	real_array_integrals.cpp
	real_array_loops.cpp
	real_matrix.cpp
	rational_matrix.cpp
	complex_matrix.cpp
	real_vector.cpp
	rational_vector.cpp
	complex_vector.cpp
	symbolic_real_numbers.cpp
	symbolic_real_arithmetic.cpp
	symbolic_real_variables.cpp
	symbolic_real_functions.cpp
	symbolic_real_trigonometric.cpp
	symbolic_real_integrals.cpp
	symbolic_real_derivatives.cpp
	symbolic_real_limits.cpp
	symbolic_real_matrices.cpp
	symbolic_real_loops.cpp
	symbolic_real_tojson.cpp
	symbolic_rational_numbers.cpp
	symbolic_rational_arithmetic.cpp
	symbolic_rational_variables.cpp
	symbolic_rational_functions.cpp
	symbolic_rational_trigonometric.cpp
	symbolic_rational_integrals.cpp
	symbolic_rational_derivatives.cpp
	symbolic_rational_limits.cpp
	symbolic_rational_matrices.cpp
	symbolic_rational_loops.cpp
	symbolic_rational_tojson.cpp
	symbolic_complex_numbers.cpp
	symbolic_complex_arithmetic.cpp
	symbolic_complex_variables.cpp
	symbolic_complex_functions.cpp
	symbolic_complex_trigonometric.cpp
	symbolic_complex_integrals.cpp
	symbolic_complex_derivatives.cpp
	symbolic_complex_limits.cpp
	symbolic_complex_matrices.cpp
	symbolic_complex_loops.cpp
	symbolic_complex_tojson.cpp
)

if (NOT WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-O0 -g -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer)
		target_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=undefined -fsanitize=address)
    endif()
endif()

if(WIN32)
	target_link_libraries(${PROJECT_NAME}_tests PRIVATE
		${PROJECT_NAME}
	)
	target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest GTest::gmock)
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

if(WIN32)
    target_link_options(${PROJECT_NAME}_tests PRIVATE "LINKER:/STACK:16777216")
endif()

install(
    TARGETS ${PROJECT_NAME}_tests
    EXPORT ${PROJECT_NAME}
    DESTINATION ${INSTALL_PATH}/bin
    COMPONENT ${PROJECT_NAME}
)
