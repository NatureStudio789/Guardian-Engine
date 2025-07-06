#include "GRenderEngine.h"
#include "../Aplication/GApplication.h"

namespace GE
{
	GRenderEngine::GRenderEngine()
	{
		this->MainGraphicsContext = null;

		this->ModuleName = "Render";
	}

	GRenderEngine::~GRenderEngine()
	{
		this->MainGraphicsContext.reset();
		this->MainGraphicsContext = null;
	}

	void GRenderEngine::InitializeModule()
	{
		RECT ApplicationRect = GApplication::Instance->GetMainWindow()->GetWindowArea();
		const auto& ApplicationAttribute = GApplication::Instance->GetMainWindow()->GetWindowAttribute();
		this->MainGraphicsContext = GGraphicsContext::CreateNewGraphicsContext(GApplication::Instance->GetMainWindowHandle(),
			ApplicationRect.right - ApplicationRect.left, ApplicationRect.bottom - ApplicationRect.top,
			ApplicationAttribute.WindowStyle == GWindow::GE_STYLE_FULLSCREEN);

		GGraphicsContextRegistry::RegisterGraphicsContext("Main", this->MainGraphicsContext);
		GGraphicsContextRegistry::SetCurrentGraphicsContext("Main");
	}

	void GRenderEngine::UpdateModule()
	{
		this->MainGraphicsContext->BeginRendering();



		this->MainGraphicsContext->EndUpRendering(true);
	}

	void GRenderEngine::ReleaseModule()
	{

	}
}
