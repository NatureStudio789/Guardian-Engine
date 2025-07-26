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

		ImGui::Begin("Viewport");

		ImGui::End();

		this->EditorContext->EndUpRendering();
	}

	void GEditorRenderGraph::Reset()
	{

	}
}
