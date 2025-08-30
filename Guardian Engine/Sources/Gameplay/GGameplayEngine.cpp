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
		/*auto Light1 = GSceneRegistry::GetActiveScene()->CreateEntity("Light1");
		Light1->GetComponent<GTransformComponent>().Transform.Position = { 20.0f, 20.0f, 0.0f };
		Light1->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		Light1->AddComponent<GPointLightComponent>().PointLight->Strength = 10000.0f;
		Light1->AddComponent<GModelComponent>("Box");

		auto Light2 = GSceneRegistry::GetActiveScene()->CreateEntity("Light2");
		Light2->GetComponent<GTransformComponent>().Transform.Position = { 20.0f, 20.0f, 0.0f };
		Light2->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		Light2->AddComponent<GPointLightComponent>().PointLight->Strength = 10000.0f;
		Light2->AddComponent<GModelComponent>("Box");

		auto Cerberus = GSceneRegistry::GetActiveScene()->CreateEntity("Cerberus");
		Cerberus->GetComponent<GTransformComponent>().Transform.Scale = { 0.1f, 0.1f, 0.1f };
		Cerberus->AddComponent<GModelComponent>("Cerberus");*/
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
