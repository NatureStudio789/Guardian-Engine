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

	}

	void GEngine::InitializeEngine(std::shared_ptr<GProgram> program)
	{
		this->EngineProgram = program;
		GUARDIAN_CHECK_POINTER(program)

		this->EngineProgram->PreInitializeProgram();

		GApplication::Instance->InitializeApplication(this->EngineProgram->GetProgramAttribute().ProgramWindowAttribute);

		GModuleSystem::RegisterMultiModules(this->EngineProgram->GetProgramRequiredModuleList());
		GModuleSystem::LoadAllModules();
	}

	void GEngine::LaunchEngine()
	{
		GApplication::Instance->DisplayMainWindow();

		while (GApplication::Instance->IsApplicationRunning())
		{
			GApplication::Instance->UpdateApplication();

			GModuleSystem::UpdateAllModules();
		}
	}

	void GEngine::ReleaseEngine()
	{
		GModuleSystem::ReleaseAllModules();
	}
}
