#include "GEngine.h"

namespace GE
{
	std::shared_ptr<GEngine> GEngine::Instance = std::make_shared<GEngine>();


	GEngine::GEngine()
	{
		this->EngineProgram = null;
	}

	GEngine::~GEngine()
	{
		this->ReleaseEngine();
	}

	void GEngine::InitializeEngine(std::shared_ptr<GProgram> program)
	{
		this->EngineProgram = program;
		GUARDIAN_CHECK_POINTER(program)

		this->EngineProgram->PreInitializeProgram();

		GApplication::Instance->InitializeApplication(this->EngineProgram->GetProgramAttribute().ProgramWindowAttribute);

		for (const auto& moduleName : this->EngineProgram->GetProgramRequiredModuleList())
		{
			GModuleSystem::RegisterModule(moduleName);
			GModuleSystem::LoadModule(moduleName);
		}
	}

	void GEngine::LaunchEngine()
	{
		GApplication::Instance->DisplayMainWindow();

		while (GApplication::Instance->IsApplicationRunning())
		{
			GApplication::Instance->UpdateApplication();
		}
	}

	void GEngine::ReleaseEngine()
	{

	}
}
