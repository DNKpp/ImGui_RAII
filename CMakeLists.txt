cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(imgui_raii)

option(IMGUI_RAII_BUILD_TESTS "Determines whether library tests will be built or not." OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

if(IMGUI_RAII_BUILD_TESTS)

	message(WARNING "ImGui_RAII test building is enabled, thus dear_imgui library will be fetched.")
	add_subdirectory("${PROJECT_SOURCE_DIR}/tests")

else()

	add_subdirectory("${PROJECT_SOURCE_DIR}/src")

endif()
