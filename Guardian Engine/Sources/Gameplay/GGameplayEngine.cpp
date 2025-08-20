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

		this->Light1 = this->ActiveScene->CreateEntity("Light1");
		this->Light1->GetComponent<GTransformComponent>().Transform.Position = { 20.0f, 20.0f, 0.0f };
		this->Light1->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		this->Light1->AddComponent<GPointLightComponent>().PointLight->Strength = 10000.0f;
		this->Light1->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Box/Box.fbx"));

		this->Light2 = this->ActiveScene->CreateEntity("Light2");
		this->Light2->GetComponent<GTransformComponent>().Transform.Position = { 20.0f, 20.0f, 0.0f };
		this->Light2->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		this->Light2->AddComponent<GPointLightComponent>().PointLight->Strength = 10000.0f;
		this->Light2->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Box/Box.fbx"));

		this->Cerberus = this->ActiveScene->CreateEntity("Cerberus");
		this->Cerberus->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		this->Cerberus->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Cerberus/Cerberus.fbx"));
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
