#include "GRenderEngine.h"
#include "../Aplication/GApplication.h"

namespace GE
{
	GRenderEngine::GRenderEngine()
	{
		this->MainGraphicsContext = null;

		this->ModuleName = "Render";
	}

	GRenderEngine::GRenderEngine(const GRenderEngine& other) : GModule(other)
	{
		this->MainGraphicsContext = other.MainGraphicsContext;
	}

	GRenderEngine::~GRenderEngine()
	{
		
	}

	void GRenderEngine::InitializeModule()
	{
		RECT ApplicationRect = GApplication::Instance->GetMainWindow()->GetWindowArea();
		const auto& ApplicationAttribute = GApplication::Instance->GetMainWindow()->GetWindowAttribute();
		this->MainGraphicsContext = std::make_shared<GGraphicsContext>(GApplication::Instance->GetMainWindowHandle(),
			ApplicationRect.right - ApplicationRect.left, ApplicationRect.bottom - ApplicationRect.top,
			ApplicationAttribute.WindowStyle == GWindow::GE_STYLE_FULLSCREEN);

		GGraphicsContextRegistry::RegisterGraphicsContext("Main", this->MainGraphicsContext);
		GGraphicsContextRegistry::SetCurrentGraphicsContext("Main");

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginInitializing();

		std::vector<GMeshInstance::Vertex> v =
		{
			{{0.0f, 0.5f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}},
			{{-0.5f, -0.5f, 0.0f}},
		};
		std::vector<UINT> i =
		{
			0, 1, 2
		};
		this->TestMesh = GMeshInstance::CreateNewMeshInstance({ "Test", v, i });

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
	}

	void GRenderEngine::UpdateModule()
	{
		this->MainGraphicsContext->BeginRendering();

		this->MainGraphicsContext->ApplyMainFramebuffer();

		this->TestMesh->Render();

		this->MainGraphicsContext->EndUpRendering(true);
	}

	void GRenderEngine::ReleaseModule()
	{
		this->MainGraphicsContext.reset();
		this->MainGraphicsContext = null;
	}
}
