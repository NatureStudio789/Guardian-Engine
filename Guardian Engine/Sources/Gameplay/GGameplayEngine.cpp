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

		this->Nanosuit = this->ActiveScene->CreateEntity("Nanosuit");
		this->Nanosuit->AddComponent<GPointLightComponent>().PointLight->Strength = 10000.0f;
		this->Nanosuit->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		this->Nanosuit->GetComponent<GTransformComponent>().Transform.Position = { -10.0f, 0.0f, 0.0f };
		this->Nanosuit->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Nanosuit/Nanosuit.fbx"));

		this->Hasake = this->ActiveScene->CreateEntity("Hasake", "Nanosuit");
		this->Hasake->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Box/Box.fbx"));

		this->House = this->ActiveScene->CreateEntity("House");
		this->House->AddComponent<GModelComponent>(GModel::CreateNewModel("Assets/Models/Cerberus/Cerberus.fbx"));
	}

	void GGameplayEngine::UpdateModule()
	{
		this->ActiveScene->GetEntity("Main Camera")->GetComponent<GTransformComponent>().Transform.Position = {0.0f, 0.0f, -50.0f};

		this->House->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		if (GetAsyncKeyState('R'))
		{
			this->House->GetComponent<GTransformComponent>().Transform.Rotate(0.0f, 1.0f, 0.0f);
		}
		if (GetAsyncKeyState('T'))
		{
			this->House->GetComponent<GTransformComponent>().Transform.Rotate(0.0f, -1.0f, 0.0f);
		}

		if (GetAsyncKeyState('Q'))
		{
			this->Hasake->GetComponent<GTransformComponent>().Transform.Translate({ -1.0f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState('E'))
		{
			this->Hasake->GetComponent<GTransformComponent>().Transform.Translate({ 1.0f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState('W'))
		{
			this->ActiveScene->GetEditCamera()->Translate({ 0.0f, 0.0f, 0.1f });
		}
		if (GetAsyncKeyState('S'))
		{
			this->ActiveScene->GetEditCamera()->Translate({ 0.0f, 0.0f, -0.1f });
		}
		if (GetAsyncKeyState('A'))
		{
			this->ActiveScene->GetEditCamera()->Translate({ -0.1f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState('D'))
		{
			this->ActiveScene->GetEditCamera()->Translate({ 0.1f, 0.0f, 0.0f });
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			this->Nanosuit->GetComponent<GTransformComponent>().Transform.Translate({ -0.1f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			this->Nanosuit->GetComponent<GTransformComponent>().Transform.Translate({ 0.1f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState(VK_ADD))
		{
			this->Nanosuit->GetComponent<GTransformComponent>().Transform.Scaling({ 0.01f, 0.01f, 0.01f });
		}
		if (GetAsyncKeyState(VK_SUBTRACT))
		{
			this->Nanosuit->GetComponent<GTransformComponent>().Transform.Scaling({ -0.01f, -0.01f, -0.01f });
		}

		this->ActiveScene->Update();
	}

	void GGameplayEngine::ReleaseModule()
	{
		this->ActiveScene.reset();
		this->ActiveScene = null;
	}
}
