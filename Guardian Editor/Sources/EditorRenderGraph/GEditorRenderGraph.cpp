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

		this->EditorMainDockspace = std::make_shared<EUI::GDockspace>("Guardian Editor Dockspace");
		this->AddEditor(std::make_shared<GSceneEditor>());
		this->AddEditor(std::make_shared<GAssetEditor>());
	}

	void GEditorRenderGraph::Execute()
	{
		this->InitializeGraphGraphics();
		this->EditorContext->BeginRendering();

		this->EditorMainDockspace->Update();
		this->EditorMainDockspace->Render();

		this->EditorContext->EndUpRendering();
	}

	void GEditorRenderGraph::Reset()
	{

	}

	void GEditorRenderGraph::AddEditor(std::shared_ptr<EUI::GEditor> editor)
	{
		GUARDIAN_CHECK_POINTER(editor);

		editor->Initialize();
		this->EditorMainDockspace->AddEditorToDockspace(editor);
	}
}
