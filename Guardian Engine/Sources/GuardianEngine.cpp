#include "GuardianEngine.h"

namespace guardian
{
	std::unique_ptr<GuardianEngine> GuardianEngine::EngineInstance = std::make_unique<GuardianEngine>();


	GuardianEngine::GuardianEngine()
	{
		this->EngineProgram = null;
		this->EngineEventProcesser = std::make_unique<GuardianEventProcesser>();
		this->EngineScene = std::make_shared<GuardianScene>();
	}

	GuardianEngine::~GuardianEngine()
	{
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
		EngineWindowProperties.SetWindowResolution((int)(GetSystemMetrics(SM_CXSCREEN) * 0.8), (int)(GetSystemMetrics(SM_CYSCREEN) * 0.8));
		EngineWindowProperties.SetWindowTitle(this->EngineProgram->GetProgramName());
		GuardianApplication::ApplicationInstance->InitializeApplication(EngineWindowProperties);

		GuardianScriptEngine::InitializeScriptEngine();

		this->EngineProgram->Initialize();
	}

	void GuardianEngine::LaunchEngine()
	{
		GuardianApplication::ApplicationInstance->DisplayWindow();
		while (GuardianApplication::ApplicationInstance->IsApplicationRunning())
		{
			GuardianApplication::ApplicationInstance->UpdateApplication();
			switch (this->EngineScene->GetSceneState())
			{
				case GE_SCENE_EDIT:
				{
					this->EngineScene->UpdateEditScene(16.666f);
					break;
				}

				case GE_SCENE_RUNTIME:
				{
					this->EngineScene->UpdateRuntimeScene();
					break;
				}
			}
			this->EngineProgram->Update();

			GuardianApplication::ApplicationInstance->BeginRendering({0.0f, 0.0f, 0.0f});
			{
				GuardianRenderer::RenderScene();

				this->EngineProgram->Render();
			}
			GuardianApplication::ApplicationInstance->EndUpRendering();
		}
	}

	std::shared_ptr<GuardianScene> GuardianEngine::GetScene() noexcept
	{
		return this->EngineScene;
	}
}
