#include "GuardianEngine.h"

namespace guardian
{
	std::unique_ptr<GuardianEngine> GuardianEngine::EngineInstance = std::make_unique<GuardianEngine>();


	GuardianEngine::GuardianEngine()
	{
		this->EngineProgram = null;
		this->EngineEventProcesser = std::make_unique<GuardianEventProcesser>();
	}

	GuardianEngine::~GuardianEngine()
	{
		if (this->EngineProgram)
		{
			delete this->EngineProgram;
		}
		this->EngineProgram = null;
	}

	void GuardianEngine::InitializeEngine(GuardianProgram* program)
	{
		this->EngineProgram = program;
		if (!this->EngineProgram)
		{
			throw GUARDIAN_VALUE_EXCEPTION((long long)this->EngineProgram);
		}

		GuardianWindowProperties EngineWindowProperties;
		EngineWindowProperties.SetWindowResolution(1536, 864);
		EngineWindowProperties.SetWindowTitle(this->EngineProgram->GetProgramName());
		GuardianApplication::ApplicationInstance->InitializeApplication(EngineWindowProperties);

		this->EngineProgram->Initialize();
	}

	void GuardianEngine::LaunchEngine()
	{
		GuardianApplication::ApplicationInstance->DisplayWindow();
		while (GuardianApplication::ApplicationInstance->IsApplicationRunning())
		{
			GuardianApplication::ApplicationInstance->UpdateApplication();
			this->EngineProgram->Update();

			GuardianApplication::ApplicationInstance->BeginRendering({0.0f, 0.0f, 0.0f});
			{
				GuardianRenderer::RenderScene(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext());

				this->EngineProgram->Render();
			}
			GuardianApplication::ApplicationInstance->EndUpRendering();
		}
	}
}
