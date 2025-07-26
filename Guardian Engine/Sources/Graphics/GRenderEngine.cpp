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
		this->MainGraphicsContext = GGraphicsContext::CreateNewGraphicsContext(
			GApplication::Instance->GetMainWindowHandle(),
			ApplicationRect.right - ApplicationRect.left, 
			ApplicationRect.bottom - ApplicationRect.top,
			ApplicationAttribute.WindowStyle == GWindow::GE_STYLE_FULLSCREEN);

		GGraphicsContextRegistry::RegisterGraphicsContext("Main", this->MainGraphicsContext);
		GGraphicsContextRegistry::SetCurrentGraphicsContext("Main");

		GPipelineStateRegistry::InitializePipelineStateRegistry();
	}

	void GRenderEngine::UpdateModule()
	{
		GRenderer::Render();
	}

	void GRenderEngine::ReleaseModule()
	{
		this->MainGraphicsContext.reset();
		this->MainGraphicsContext = null;
	}
}
