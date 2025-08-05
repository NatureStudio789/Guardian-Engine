#include "GEditorContext.h"

namespace GE
{
	GEditorContext::GEditorContext()
	{
		this->ContextFramebuffer = null;
		this->EditorDescriptorHeap = null;
		this->ContextEventProcesser = null;
	}

	GEditorContext::GEditorContext(std::shared_ptr<GFramebuffer> contextFramebuffer)
	{
		this->InitializeEditorContext(contextFramebuffer);
	}

	GEditorContext::GEditorContext(const GEditorContext& other)
	{
		this->ContextFramebuffer = other.ContextFramebuffer;
		this->EditorDescriptorHeap = other.EditorDescriptorHeap;
		this->ContextEventProcesser = other.ContextEventProcesser;
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

		this->ContextEventProcesser = std::make_shared<GEventProcesser>();
		this->ContextEventProcesser->OnEvent<GWindowResizeEvent>([=](const GWindowResizeEvent& event)
		{
			if (this->ContextFramebuffer && event.WindowHandle == GApplication::Instance->GetMainWindowHandle())
			{
				GUARDIAN_SETUP_AUTO_THROW();

				GGraphicsContextRegistry::GetCurrentGraphicsContext()->SetCurrentCommandList("Editor");
				GGraphicsContextRegistry::GetCurrentGraphicsContext()->FlushCommandQueue();

				GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
					Reset(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListAllocator().Get(), null));

				GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsSwapChain()->ResizeBuffer(event.ResizeWidth, event.ResizeHeight);

				this->ContextFramebuffer->ResizeFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext(), event.ResizeWidth, event.ResizeHeight);

				GGraphicsContextRegistry::GetCurrentGraphicsContext()->ExecuteCommandList();

				GGraphicsContextRegistry::GetCurrentGraphicsContext()->FlushCommandQueue();
			}
		});

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
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
		
		this->ContextFramebuffer->ApplyFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());
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
