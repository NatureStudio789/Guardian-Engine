#include "GGameplayEngine.h"

namespace GE
{
	GGameplayEngine::GGameplayEngine()
	{
		this->ModuleName = "Gameplay";
	}

	GGameplayEngine::GGameplayEngine(const GGameplayEngine& other)
	{
	}

	GGameplayEngine::~GGameplayEngine()
	{

	}

	void GGameplayEngine::InitializeModule()
	{

	}

	void GGameplayEngine::UpdateModule()
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('S'))
		{
			GSceneSerializer::Export("Sandbox\\Assets\\Sample Scene.gscene", GSceneRegistry::GetActiveScene());
		}

		GSceneRegistry::GetActiveScene()->Update();
	}

	void GGameplayEngine::ReleaseModule()
	{

	}
}
