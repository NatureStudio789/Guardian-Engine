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
		GLogger::InitializeLogger();

		GUARDIAN_ENGINE_INFO("Guardian Engine v1.0 - <Nature Studio>");
		GUARDIAN_ENGINE_INFO("Starting Engine Initializing!");

		this->EngineProgram = program;
		GUARDIAN_CHECK_POINTER(program);

		GUARDIAN_ENGINE_INFO("Preinitializing the program for Guardian Engine...");
		this->EngineProgram->PreInitializeProgram();
		GUARDIAN_ENGINE_INFO("Engine Program : {0}", this->EngineProgram->GetProgramName());
		GUARDIAN_ENGINE_INFO("Successfully preinitialized the engine program!");

		GUARDIAN_ENGINE_INFO("Initializing Application...");
		GApplication::Instance->InitializeApplication(this->EngineProgram->GetProgramAttribute().ProgramWindowAttribute);
		GUARDIAN_ENGINE_INFO("Successfully initialized application!");

		GUARDIAN_ENGINE_INFO("Getting and loading required modules from engine program...");
		GModuleSystem::RegisterMultiModules(this->EngineProgram->GetProgramRequiredModuleList());
		GModuleSystem::LoadAllModules();
		GUARDIAN_ENGINE_INFO("All Required Modules Loaded!");

		GUARDIAN_ENGINE_INFO("Initializing the program for Guardian Engine...");
		this->EngineProgram->InitializeProgram();
		GUARDIAN_ENGINE_INFO("Successfully initialized the engine program!");

		GUARDIAN_ENGINE_INFO("Successfully Initialized the Guardian Engine!");
		GUARDIAN_ENGINE_INFO("All Initializations Have Been Done.");
	}

	void GEngine::LaunchEngine()
	{
		GUARDIAN_ENGINE_INFO("Launching the Guardian Engine...");
		GApplication::Instance->DisplayMainWindow();

		GUARDIAN_ENGINE_INFO("Started The Engine Application!");
		while (GApplication::Instance->IsApplicationRunning())
		{
			GApplication::Instance->UpdateApplication();

			GModuleSystem::UpdateAllModules();

			this->EngineProgram->UpdateProgram();
		}
	}

	void GEngine::ReleaseEngine()
	{
		GUARDIAN_ENGINE_INFO("Releasing the Guardian Engine..");

		GModuleSystem::ReleaseAllModules();

		this->EngineProgram->ReleaseProgram();
		this->EngineProgram.reset();
		this->EngineProgram = null;

		GUARDIAN_ENGINE_INFO("Guardian Engine Shutdown.");
	}
}
