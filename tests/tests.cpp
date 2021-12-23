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

TEST_CASE("BeginContext should be movabel.", "[context]")
{
	BeginContext context{};
	BeginContext otherContext{ std::move(context) };

	REQUIRE(context.context() == nullptr);
	REQUIRE(otherContext.context() != nullptr);

	context = std::move(otherContext);
	REQUIRE(context.context() != nullptr);
	REQUIRE(otherContext.context() == nullptr);
}

TEST_CASE("ConditionalRAIIWrapper should be usable in boolean contextes", "[compile_check]")
{
	auto context = setup_context();
	NewFrame frame{};

	// just some arbitrary alias of that type
	if (Begin begin{ "Hello, World!" })
	{
	}
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE
(
	"RAIIWrapper actions without parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check][RAIIWrapper]",
	BeginDisabled,
	BeginGroup,
	BeginTooltip
)
#pragma warning(default: 26444)
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

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE
(
	"RAIIWrapper actions with bool parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check][RAIIWrapper]",
	PushAllowKeyboardFocus,
	PushButtonRepeat
)
#pragma warning(default: 26444)
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			TestType{ true } /
				[] { ImGui::LabelText("label", "test"); };

			TestType{ true } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushClipRect should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushClipRect{ ImVec2{ 0, 0 }, ImVec2{ 0, 0 }, true } /
				[] { ImGui::LabelText("label", "test"); };

			PushClipRect{ ImVec2{ 0, 0 }, ImVec2{ 0, 0 }, true } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushFont should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushFont{ nullptr } /
				[] { ImGui::LabelText("label", "test"); };

			PushFont{ nullptr } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushID should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushID{ "Label##Test" } /
				[] { ImGui::LabelText("label", "test"); };

			PushID{ "Label##Test" } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushItemWidth should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushItemWidth{ 1.f } /
				[] { ImGui::LabelText("label", "test"); };

			PushItemWidth{ 1.f } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushStyleColor should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushStyleColor{ 0, 0 } /
				[] { ImGui::LabelText("label", "test"); };

			PushStyleColor{ 0, 0 } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushStyleVar should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushStyleVar{ ImGuiStyleVar_WindowBorderSize, 0.f } /
				[] { ImGui::LabelText("label", "test"); };

			PushStyleVar{ ImGuiStyleVar_WindowBorderSize, 0.f } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("PushTextWrapPos should compile and not trigger an assertion.", "[compile_check][RAIIWrapper]")
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			PushTextWrapPos{ 0.f } /
				[] { ImGui::LabelText("label", "test"); };

			PushTextWrapPos{ 0.f } /
				[] { ImGui::LabelText("label", "test"); } /
				[] { ImGui::LabelText("label2", "test"); };
		};
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE
(
	"ConditionalRAIIWrapper actions without parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check][ConditionalRAIIWrapper]",
	BeginMenuBar,
	BeginMainMenuBar
)
#pragma warning(default: 26444)
{
	auto context = setup_context();
	NewFrame{} /
		[]
		{
			TestType{} >
				[] { ImGui::LabelText("label", "test"); };

			TestType{} >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}

#pragma warning(disable: 26444)
TEMPLATE_TEST_CASE
(
	"ConditionalRAIIWrapper actions accepting one string parameter should be chainable with an arbitrary amount of lambdas.",
	"[compile_check][ConditionalRAIIWrapper]",
	Begin,
	BeginChild,
	BeginMenu,
	BeginTabBar,
	BeginListBox
)
#pragma warning(default: 26444)
{
	auto context = setup_context();

	NewFrame{} /
		[]
		{
			TestType{ "Hello, World " } >
				[] { ImGui::LabelText("label", "test"); };

			TestType{ "Hello, World - again" } >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("BeginChildFrame should compile and not trigger an assertion", "[compile_check][ConditionalRAIIWrapper]")
{
	auto context = setup_context();

	NewFrame frame{};
	Begin{ "Label##test" } >
		[]
		{
			BeginChildFrame{ ImGui::GetID("Label##test"), ImVec2{ 0, 0 } } >
				[] { ImGui::LabelText("label", "test"); };

			BeginChildFrame{ ImGui::GetID("Label##test"), ImVec2{ 0, 0 } } >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("BeginCombo should compile and not trigger an assertion", "[compile_check][ConditionalRAIIWrapper]")
{
	auto context = setup_context();

	NewFrame frame{};
	Begin{ "Label##test" } >
		[]
		{
			BeginCombo{ "Hello, World", nullptr } >
				[] { ImGui::LabelText("label", "test"); };

			BeginCombo{ "Hello, World", nullptr } >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("BeginTable should compile and not trigger an assertion", "[compile_check][ConditionalRAIIWrapper]")
{
	auto context = setup_context();

	NewFrame frame{};
	Begin{ "Label##test" } >
		[]
		{
			BeginTable{ "Hello, World", 1 } >
				[] { ImGui::LabelText("label", "test"); };

			BeginTable{ "Hello, World", 1 } >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}

TEST_CASE("BeginTabItem should compile and not trigger an assertion", "[compile_check][ConditionalRAIIWrapper]")
{
	auto context = setup_context();

	NewFrame frame{};
	BeginTabBar{ "test" } >
		[]
		{
			BeginTabItem{ "Hello, World" } >
				[] { ImGui::LabelText("label", "test"); };

			BeginTabItem{ "Hello, World - again" } >
				[] { ImGui::LabelText("label", "test"); } >
				[] { ImGui::LabelText("label2", "test"); };
		};
}
