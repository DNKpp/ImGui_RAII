cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(get_cpm)
include(CTest)

CPMAddPackage("gh:catchorg/Catch2#v2.x")

CPMAddPackage(
	NAME dear_imgui
	VERSION 1.85
	GITHUB_REPOSITORY "ocornut/imgui"
	DOWNLOAD_ONLY YES
)
set(IMGUI_DIR "${dear_imgui_SOURCE_DIR}")
FILE(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp")

add_subdirectory("../src" "../out")

add_executable(
	ImGui_RAII_Tests
	"${IMGUI_SOURCES}"
	"main.cpp"
	"tests.cpp"
 )

target_link_libraries(
	ImGui_RAII_Tests
	PRIVATE
	ImGui_RAII::ImGui_RAII
	Catch2::Catch2
)

add_test(ImGui_RAII_TestSuite ImGui_RAII_Tests)
