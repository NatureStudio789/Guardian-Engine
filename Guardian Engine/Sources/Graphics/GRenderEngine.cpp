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
			{GVector3{ 0.5f,  0.5f, 0.0f}, GVector2{1.0f, 0.0f}},
			{GVector3{ 0.5f, -0.5f, 0.0f}, GVector2{1.0f, 1.0f}},
			{GVector3{-0.5f, -0.5f, 0.0f}, GVector2{0.0f, 1.0f}},
			{GVector3{-0.5f,  0.5f, 0.0f}, GVector2{0.0f, 0.0f}},
		};

		std::vector<UINT> i =
		{
			0, 1, 2,
			0, 2, 3
		};

		this->TestM = GMeshInstance::CreateNewMeshInstance("Test", { v, i });

		std::shared_ptr<GTechnique> tech = GTechnique::CreateNewTechnique("PBR", "main");
		std::shared_ptr<GStep> step = GStep::CreateNewStep("Lighting");

		auto& surface = GSurface("Assets/Textures/Header.jpg");
		static bool saved = false;
		if (!saved)
		{
			surface.SaveSurface("Saved.jpg");
		}
		step->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));
		step->AddApplicable(GTexture::CreateNewTexture(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
			surface));

		tech->AddStep(step);

		this->TestM->AddTechnique(tech);

		this->TestM->LinkTechnique("Lit");
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
	}

	void GRenderEngine::UpdateModule()
	{
		TestM->Submit("main");
		static float s = 1.0f;
		static float x = 0.0f;
		static float y = 0.0f;
		static float sdelta = 0.01f;
		static float xdelta = 0.01f;
		static float ydelta = 0.01f;
		if (GetAsyncKeyState(VK_CONTROL))
		{
			if (GetAsyncKeyState(VK_ADD))
			{
				s += sdelta;
			}
			if (GetAsyncKeyState(VK_SUBTRACT))
			{
				s -= sdelta;
			}
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			x -= xdelta;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			x += xdelta;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			y += ydelta;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			y -= ydelta;
		}

		if (s <= 0.0f)
		{
			s = 0.0f;
		}
		TestM->SetTransform({ {x, y, 0.0f}, {}, {s, s, s} });

		GRenderer::Render();
	}

	void GRenderEngine::ReleaseModule()
	{
		this->MainGraphicsContext.reset();
		this->MainGraphicsContext = null;
	}
}
