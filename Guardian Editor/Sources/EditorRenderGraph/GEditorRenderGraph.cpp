#include "GEditorRenderGraph.h"

namespace GE
{
	GEditorRenderGraph::GEditorRenderGraph(const std::string& name) : GRenderGraph()
	{
		this->RenderGraphName = name;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->RegisterGraphicsCommandList(this->RenderGraphName);

		this->RenderGraphFramebuffer = GFramebuffer::CreateNewFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		this->EditorContext = GEditorContext::CreateNewEditorContext(this->RenderGraphFramebuffer);

		this->EditorEventProcesser = std::make_shared<GEventProcesser>();
		this->EditorEventProcesser->OnEvent<GWindowResizeEvent>([this](const GWindowResizeEvent& event)
		{
			if (event.WindowHandle == GApplication::Instance->GetMainWindowHandle())
			{
				this->Resize(event.ResizeWidth, event.ResizeHeight);
			}
		});
	}

	void GEditorRenderGraph::Execute()
	{
		this->InitializeGraphGraphics();
		this->EditorContext->BeginRendering();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const bool isMaximized = IsZoomed(GApplication::Instance->GetMainWindowHandle());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f });
		ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::PopStyleVar(2);

		ImGui::DockSpace(ImGui::GetID("MyDockspace"));

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
            static bool open;
			ImGui::Checkbox("Demo Window", &open);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->EditorContext->GetEditorIO().Framerate,
				this->EditorContext->GetEditorIO().Framerate);
			ImGui::End();

			static bool IsFirst = true;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Image");

			ImGui::PopStyleVar();

			ImGui::Image(
				(ImTextureID)GRenderer::GetSceneRenderGraph()->GetRenderGraphFramebuffer()->
				GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle.ptr,
				ImGui::GetContentRegionAvail());

			ImGui::End();
		}


		ImGui::End();

		this->EditorContext->EndUpRendering();
	}

	void GEditorRenderGraph::Reset()
	{

	}
}
