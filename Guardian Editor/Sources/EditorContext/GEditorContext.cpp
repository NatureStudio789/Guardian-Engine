#include "GEditorContext.h"

namespace GE
{
	GEditorContext::GEditorContext()
	{
		this->ContextFramebuffer = null;
		this->EditorDescriptorHeap = null;
	}

	GEditorContext::GEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer)
	{
		this->InitializeEditorContext(contextFramebuffer);
	}

	GEditorContext::GEditorContext(const GEditorContext& other)
	{
		this->ContextFramebuffer = other.ContextFramebuffer;
		this->EditorDescriptorHeap = other.EditorDescriptorHeap;
	}

	GEditorContext::~GEditorContext()
	{
		this->EditorDescriptorHeap.reset();
		this->EditorDescriptorHeap = null;

		this->ContextFramebuffer = null;
	}

	void GEditorContext::InitializeEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer)
	{
		this->EditorDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice(),
			2, GDescriptorHeap::GE_DESCRIPTOR_HEAP_CBVSRVUAV, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		ImGui::StyleColorsDark();

		auto& ThemeColors = ImGui::GetStyle().Colors;
		ThemeColors[ImGuiCol_TitleBg] = ImColor(11, 11, 11);
		ThemeColors[ImGuiCol_TitleBgActive] = ImColor(16, 16, 16);
		ThemeColors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		ThemeColors[ImGuiCol_Button] = ImColor(56, 56, 56, 200);
		ThemeColors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
		ThemeColors[ImGuiCol_ButtonActive] = ImColor(56, 56, 56, 150);

		ThemeColors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		ThemeColors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		ThemeColors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

		ThemeColors[ImGuiCol_CheckMark] = ImColor(200, 200, 200, 255);

		ThemeColors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		ThemeColors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

		ThemeColors[ImGuiCol_WindowBg] = ImColor(18, 18, 18);

		ThemeColors[ImGuiCol_FrameBg] = ImColor(15, 15, 15);
		ThemeColors[ImGuiCol_FrameBgHovered] = ImColor(15, 15, 15);
		ThemeColors[ImGuiCol_FrameBgActive] = ImColor(15, 15, 15);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 6.0f);
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ChildRounding = 6.0f;
		style.PopupRounding = 6.0f;
		style.FrameRounding = 6.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0F;
			style.Colors[ImGuiCol_WindowBg].w = 1.0;
		}
		
		auto& FontDescriptorHandle = GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->Allocate(1);

		ImGui_ImplWin32_Init((void*)GApplication::Instance->GetMainWindowHandle());
		ImGui_ImplDX12_Init(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject().Get(),
			(int)GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsSwapChain()->GetBufferCount(),
			DXGI_FORMAT_R8G8B8A8_UNORM, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetSRVDescriptorHeap()->GetDescriptorHeapObject().Get(),
			FontDescriptorHandle->CPUHandle, FontDescriptorHandle->GPUHandle);

		this->ContextFramebuffer = contextFramebuffer;
	}

	void GEditorContext::BeginRendering()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void GEditorContext::EndUpRendering()
	{
		ImGui::Render();

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginRendering();
		this->ContextFramebuffer->BeginRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

		this->ContextFramebuffer->ApplyFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject().Get());

		this->ContextFramebuffer->EndUpRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpRendering();

		if (this->GetEditorIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(null, (void*)
				GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject().Get());
		}
	}

	ImGuiIO& GEditorContext::GetEditorIO() const noexcept
	{
		return ImGui::GetIO();
	}
}
