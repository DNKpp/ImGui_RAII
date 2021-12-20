#include <catch2/catch.hpp>

#include "imgui_raii/imgui_raii.hpp"

using namespace imgui_raii;

namespace
{
	BeginContext setup_context()
	{
		BeginContext context{};

		ImGuiIO& io = ImGui::GetIO();

		// Build atlas
		unsigned char* tex_pixels = nullptr;
		int tex_w, tex_h;
		io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

		io.DisplaySize = ImVec2(1920, 1080);
		io.DeltaTime = 1.0f / 60.0f;

		return context;
	}
}

TEST_CASE("BeginContext should be movabel.")
{
	BeginContext context{};
	BeginContext otherContext{ std::move(context) };

	REQUIRE(context.context() == nullptr);
	REQUIRE(otherContext.context() != nullptr);

	context = std::move(otherContext);
	REQUIRE(context.context() != nullptr);
	REQUIRE(otherContext.context() == nullptr);
}

TEST_CASE("ConditionalRAIIWrapper should be usable in boolean contextes", "compile_check")
{
	auto context = setup_context();
	NewFrame frame{};

	// just some arbitrary alias of that type
	if (Begin begin{ "Hello, World!" })
	{
	}
}

TEMPLATE_TEST_CASE
(
	"Wrapped actions accepting one string parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check]",
	Begin,
	BeginChild,
	BeginTabBar
)
{
	auto context = setup_context();

	NewFrame{} /
		[]
		{
			TestType{ "Hello, World " } /
				[] { ImGui::LabelText("label", "test"); };

			TestType{ "Hello, World - again" } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEMPLATE_TEST_CASE
(
	"Wrapped actions without parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check]",
	BeginDisabled,
	BeginGroup,
	BeginMenuBar,
	BeginMainMenuBar,
	BeginTooltip
)
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			TestType{} /
				[] { ImGui::LabelText("label", "test"); };

			TestType{} /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("BeginTabItem should compile and not trigger an assertion", "[compile_check]")
{
	auto context = setup_context();

	NewFrame frame{};
	BeginTabBar{ "test" } /
		[]
		{
			BeginTabItem{ "Hello, World " } /
				[] { ImGui::LabelText("label", "test"); };

			BeginTabItem{ "Hello, World - again" } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}
