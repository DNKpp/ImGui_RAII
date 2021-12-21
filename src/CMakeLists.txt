cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(imgui_raii)

option(IMGUI_RAII_BUILD_TESTS "Determines whether library tests will be built or not." OFF)

# thankfully adapt the idea of: https://github.com/eliasdaler/imgui-sfml
list(APPEND IMGUI_SEARCH_PATH
  "${IMGUI_DIR}"
)

find_path(IMGUI_INCLUDE_DIR
  NAMES imgui.h
  PATHS "${IMGUI_SEARCH_PATH}"
  NO_DEFAULT_PATH
)

if(NOT IMGUI_INCLUDE_DIR)
  message(FATAL_ERROR "IMGUI imgui.h not found. Set IMGUI_DIR to imgui's top-level path (containing \"imgui.cpp\" and \"imgui.h\" files).\n")
endif()

add_library(ImGui_RAII INTERFACE)

# Add pretty alias
add_library(ImGui_RAII::ImGui_RAII ALIAS ImGui_RAII)

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
)
