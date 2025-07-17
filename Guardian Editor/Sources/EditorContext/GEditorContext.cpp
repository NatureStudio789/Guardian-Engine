#include "GEditorContext.h"

namespace GE
{
	GEditorContext::GEditorContext()
	{
		this->EditorDescriptorHeap = null;
	}

	GEditorContext::GEditorContext(const GEditorContext& other)
	{
		this->EditorDescriptorHeap = other.EditorDescriptorHeap;
	}

	GEditorContext::~GEditorContext()
	{
		this->EditorDescriptorHeap.reset();
		this->EditorDescriptorHeap = null;
	}

	void GEditorContext::InitializeEditorContext()
	{
		this->EditorDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice(),
			1, GDescriptorHeap::GE_DESCRIPTOR_HEAP_CBVSRVUAV, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0F;
			style.Colors[ImGuiCol_WindowBg].w = 1.0;
		}

		ImGui_ImplWin32_Init((void*)GApplication::Instance->GetMainWindowHandle());
		ImGui_ImplDX12_Init(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject().Get(),
			(int)GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsSwapChain()->GetBufferCount(),
			DXGI_FORMAT_R8G8B8A8_UNORM, this->EditorDescriptorHeap->GetDescriptorHeapObject().Get(),
			this->EditorDescriptorHeap->GetFirstCPUDescriptorHandle(), this->EditorDescriptorHeap->GetFirstGPUDescriptorHandle());

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginInitializing();

		GFramebufferRegistry::RegisterFramebuffer("Editor");

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
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
		
		GFramebufferRegistry::GetFramebuffer("Editor")->ApplyFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			SetDescriptorHeaps(1, this->EditorDescriptorHeap->GetDescriptorHeapObject().GetAddressOf());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject().Get());

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
