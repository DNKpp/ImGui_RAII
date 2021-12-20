cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(CTest)

CPMAddPackage("gh:catchorg/Catch2#v2.x")

add_executable(
	ImGui_RAII_Tests
	"${IMGUI_DIR}/imgui_widgets.cpp"
	"${IMGUI_DIR}/imgui_tables.cpp"
	"${IMGUI_DIR}/imgui_draw.cpp"
	"${IMGUI_DIR}/imgui.cpp"

	"main.cpp"
	"tests.cpp"
 )

target_link_libraries(
	ImGui_RAII_Tests
	PRIVATE
	ImGui_RAII
	Catch2::Catch2
)

add_test(ImGui_RAII_TestSuite ImGui_RAII_Tests)
