cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(imgui_raii)

option(IMGUI_RAII_BUILD_TESTS "Determines whether library tests will be built or not." OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

if(IMGUI_RAII_BUILD_TESTS)

	add_subdirectory("${PROJECT_SOURCE_DIR}/tests")

endif()
