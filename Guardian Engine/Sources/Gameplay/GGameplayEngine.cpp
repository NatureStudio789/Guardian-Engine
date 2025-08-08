#include "GGameplayEngine.h"

namespace GE
{
	GGameplayEngine::GGameplayEngine()
	{
		this->ActiveScene = null;
		this->ModuleName = "Gameplay";
	}

	GGameplayEngine::GGameplayEngine(const GGameplayEngine& other)
	{
		this->ActiveScene = other.ActiveScene;
	}

	GGameplayEngine::~GGameplayEngine()
	{

	}

	void GGameplayEngine::InitializeModule()
	{
		this->ActiveScene = GScene::CreateNewScene("Main");
		GSceneRegistry::RegisterScene(this->ActiveScene);
		GSceneRegistry::SetActiveScene("Main");
	}

	void GGameplayEngine::UpdateModule()
	{
		this->ActiveScene->Update();
	}

	void GGameplayEngine::ReleaseModule()
	{
		this->ActiveScene.reset();
		this->ActiveScene = null;
	}
}
