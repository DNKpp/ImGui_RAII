#include <imgui_raii/imgui_raii.hpp>

imgui_raii::BeginContext setup_imgui_context()
{
	imgui_raii::BeginContext context{};

	ImGuiIO& io = ImGui::GetIO();

	// Build atlas
	unsigned char* tex_pixels = nullptr;
	int tex_w, tex_h;
	io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

	io.DisplaySize = ImVec2(1920, 1080);
	io.DeltaTime = 1.0f / 60.0f;

	return context;
}

int main()
{
	auto context = setup_imgui_context();

	imgui_raii::NewFrame{} /
		[]
		{
			imgui_raii::Begin{ "Hello, World!" } >
				[]
				{
					if (ImGui::Button("Test Button"))
					{
					}
				};
		};
}
