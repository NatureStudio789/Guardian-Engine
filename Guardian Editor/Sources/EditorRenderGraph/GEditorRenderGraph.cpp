#include "GEditorRenderGraph.h"

namespace GE
{
	GEditorRenderGraph::GEditorRenderGraph(const std::string& name) : GRenderGraph()
	{
		this->RenderGraphName = name;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->RegisterGraphicsCommandList(this->RenderGraphName);

		this->InitializeGraphGraphics();
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginInitializing();
		this->RenderGraphFramebuffer = GFramebuffer::CreateNewFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		this->EditorContext = GEditorContext::CreateNewEditorContext(this->RenderGraphFramebuffer);
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
	}

	void GEditorRenderGraph::Execute()
	{
		this->InitializeGraphGraphics();
		this->EditorContext->BeginRendering();

		static bool open = true;
		if (open)
		{
			ImGui::ShowDemoWindow(&open);
		}

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &open);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->EditorContext->GetEditorIO().Framerate,
				this->EditorContext->GetEditorIO().Framerate);
			ImGui::End();
		}

		this->EditorContext->EndUpRendering();
	}

	void GEditorRenderGraph::Reset()
	{

	}
}
