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

		GRenderer::RegisterRenderGraph(GLitRenderGraph::CreateNewLitRenderGraph("Lit"));

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->SetCurrentCommandList("Lit");
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginInitializing();
		std::vector<GMeshInstance::Vertex> v =
		{
			{{0.5f, 0.5f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}},
			{{-0.5f, -0.5f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}},
		};

		std::vector<UINT> i =
		{
			0, 1, 2,
			0, 2, 3
		};

		this->TestM = GMeshInstance::CreateNewMeshInstance("Test", { v, i });

		std::shared_ptr<GTechnique> tech = GTechnique::CreateNewTechnique("Phong", "main");
		std::shared_ptr<GStep> step = GStep::CreateNewStep("Lighting");

		step->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));

		tech->AddStep(step);

		this->TestM->AddTechnique(tech);

		this->TestM->LinkTechnique("Lit");
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
	}

	void GRenderEngine::UpdateModule()
	{
		TestM->Submit("main");

		GRenderer::Render();
	}

	void GRenderEngine::ReleaseModule()
	{
		this->MainGraphicsContext.reset();
		this->MainGraphicsContext = null;
	}
}
