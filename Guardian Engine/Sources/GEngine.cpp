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
		if (!this->EngineProgram)
		{
			// TODO: Throw error exception.
			/*...*/
		}

		this->EngineProgram->PreInitializeProgram();
	}

	void GEngine::LaunchEngine()
	{

	}

	void GEngine::ReleaseEngine()
	{

	}
}
