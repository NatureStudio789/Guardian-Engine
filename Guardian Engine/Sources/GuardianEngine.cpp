#include "GuardianEngine.h"

namespace GE
{
	std::unique_ptr<GuardianEngine> GuardianEngine::EngineInstance = std::make_unique<GuardianEngine>();


	GuardianEngine::GuardianEngine()
	{
		this->EngineProgram = null;
		this->EngineEventProcesser = std::make_unique<GuardianEventProcesser>();
		this->EngineProject = std::make_shared<GuardianProject>();
		this->FinishedInitialization = false;
	}

	GuardianEngine::~GuardianEngine()
	{
		this->EngineProgram = null;
	}

	void GuardianEngine::InitializeEngine(GuardianCreator* creator, GuardianProgram* program)
	{
		this->EngineCreator = creator;
		if (!this->EngineCreator)
		{
			throw GUARDIAN_VALUE_EXCEPTION((long long)this->EngineCreator);
		}

		this->EngineProgram = program;
		if (!this->EngineProgram)
		{
			throw GUARDIAN_VALUE_EXCEPTION((long long)this->EngineProgram);
		}
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Guardian Engine v1.0d");
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_WARNING, "Current engine is a debug version.");
		
		GuardianWindowProperties EngineWindowProperties;
		EngineWindowProperties.SetWindowResolution((int)(GetSystemMetrics(SM_CXSCREEN) * 0.8), (int)(GetSystemMetrics(SM_CYSCREEN) * 0.8));
		EngineWindowProperties.SetWindowTitle(this->EngineProgram->GetProgramName());

		GuardianApplication::ApplicationInstance->InitializeApplication(EngineWindowProperties);
		GuardianApplication::ApplicationInstance->MainWindow->AvailableWindowGraphics();
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Availabled engine graphics.");

		this->EngineCreator->Initialize();
		this->LaunchCreator();

		GuardianScriptEngine::InitializeScriptEngine();

		GuardianThread Initialization;
		Initialization.LaunchThread([=]()
		{
			try
			{
				GuardianShaderSystem::InitializeShaderSystem();
				GuardianRenderer::InitializeRenderer();

				GuardianPhysicsEngine::InitializePhysicsEngine();

				this->EngineProject->LoadProject("Guardian Example\\GuardianExample.gproject");
				this->EngineProgram->Initialize();

				GuardianTime::InitializeTime();

				this->FinishedInitialization = true;
			}
			catch (const GE::GuardianException& e)
			{
				MessageBoxA(null, e.what(), "Guardian Exception", MB_OK | MB_ICONEXCLAMATION);
			}
			catch (const std::exception& e)
			{
				MessageBoxA(null, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
			}
			catch (...)
			{
				MessageBoxA(null, "No detail description", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
			}
		});
		Initialization.DetachMainThread();
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Guardian Engine Initialization Successful!");

		GuardianApplication::ApplicationInstance->LaunchStartupWindow();
	}

	void GuardianEngine::LaunchEngine()
	{
		GuardianApplication::ApplicationInstance->DisplayMainWindow();
		while (GuardianApplication::ApplicationInstance->IsApplicationRunning())
		{
			GuardianTime::UpdateTime();
			GuardianApplication::ApplicationInstance->UpdateApplication();
			GuardianMaterialSystem::UpdateMaterialSystem();
			GuardianAssetSystem::UpdateAssetSystem();
			this->EngineProject->UpdateProject();
			this->EngineProgram->Update();

			{
				GuardianRenderer::Render();

				this->EngineProgram->Render();
			}
			GuardianApplication::ApplicationInstance->EndUpRendering();
		}

		if (this->EngineProject->GetProjectState() == GE_SCENE_RUNTIME)
		{
			this->EngineProject->StopRuntime();
		}

		this->EngineProject->SaveProject("Guardian Example/GuardianExample.gproject");
	}

	void GuardianEngine::SetCurrentScene(const GString& sceneName)
	{
		this->EngineProject->SetCurrentScene(sceneName);
	}

	std::shared_ptr<GuardianScene> GuardianEngine::GetScene() noexcept
	{
		return this->EngineProject->GetCurrentScene();
	}

	void GuardianEngine::LaunchCreator()
	{
		GuardianApplication::ApplicationInstance->DisplayCreatorWindow();
		while (GuardianApplication::ApplicationInstance->CreatorWindow->GetIsWindowRunning())
		{
			GuardianApplication::ApplicationInstance->CreatorWindow->UpdateWindowMessage();
			
			this->EngineCreator->Update();
			this->EngineCreator->Render();

			GuardianApplication::ApplicationInstance->CreatorWindow->GetWindowGraphicsContext()->EndUpRendering();
		}

		GuardianApplication::ApplicationInstance->CreatorWindow->HideWindow();
		this->EngineCreator->Release();
	}
}
