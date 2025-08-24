#include "GProjectEngine.h"

namespace GE
{
	GProjectEngine::GProjectEngine()
	{
		this->ModuleName = "Project";
	}

	GProjectEngine::GProjectEngine(const GProjectEngine& other)
	{

	}

	GProjectEngine::~GProjectEngine()
	{

	}

	void GProjectEngine::InitializeModule()
	{
		//GProject::Instance->CreateProject("Sandbox", "Sandbox\\");
		GProject::Instance->InitializeProject("Sandbox\\Sandbox.geproj");
	}

	void GProjectEngine::UpdateModule()
	{

	}

	void GProjectEngine::ReleaseModule()
	{

	}
}
