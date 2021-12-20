cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(setup_with_imgui)

include(cmake/CPM.cmake)

CPMAddPackage(
	NAME dear_imgui
	GITHUB_REPOSITORY "ocornut/imgui"
	GIT_TAG "master"
	DOWNLOAD_ONLY YES
)

set(IMGUI_DIR "${dear_imgui_SOURCE_DIR}")
CPMAddPackage("gh:DNKpp/ImGui_RAII#master")

FILE(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp")
add_executable(
	setup_with_imgui
	"main.cpp"
	"${IMGUI_SOURCES}"
)

target_link_libraries(
	setup_with_imgui
	PRIVATE
	ImGui_RAII::ImGui_RAII
)
