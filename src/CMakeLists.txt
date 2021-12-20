cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(imgui_raii)

option(IMGUI_RAII_BUILD_TESTS "Determines whether library tests will be built or not." OFF)

# thankfully use the idea of: https://github.com/eliasdaler/imgui-sfml/blob/master/CMakeLists.txt
if(NOT IMGUI_DIR)
  set(IMGUI_DIR "" CACHE PATH "imgui top-level directory")
  message(FATAL_ERROR "ImGui directory not found. Set IMGUI_DIR to imgui's top-level path (containing 'imgui.h' and other files).\n")
endif()

add_library(ImGui_RAII INTERFACE)

# Add pretty alias
add_library(ImGui_RAII_Wrap::ImGui_RAII ALIAS ImGui_RAII)

target_include_directories(
	ImGui_RAII
	INTERFACE
	"."
	"${IMGUI_DIR}"
)

target_compile_features(
	ImGui_RAII
	INTERFACE
	cxx_std_20
#	cxx_std_23	# forces msvc to use /std:latest due to issues with ranges header
)
