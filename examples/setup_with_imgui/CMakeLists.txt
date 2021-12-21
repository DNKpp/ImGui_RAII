cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(setup_with_imgui)

include(get_cpm)

CPMAddPackage(
	NAME dear_imgui
	VERSION 1.85
	GITHUB_REPOSITORY "ocornut/imgui"
	DOWNLOAD_ONLY YES	# dear imgui has no CMakeLists.txt, thus we can't configure it automatically
)

set(IMGUI_DIR "${dear_imgui_SOURCE_DIR}")
CPMAddPackage("gh:DNKpp/ImGui_RAII#master")

FILE(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp")
add_executable(
	setup_with_imgui
	"${IMGUI_SOURCES}"
	"main.cpp"
)

target_link_libraries(
	setup_with_imgui
	PRIVATE
	ImGui_RAII::ImGui_RAII
)
