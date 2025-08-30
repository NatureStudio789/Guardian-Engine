#include "GScene.h"
#include "../Entity/GEntity.h"

namespace GE
{
	GScene::GScene()
	{
		this->SceneState = GE_STATE_NONE;
	}

	GScene::GScene(const std::string& name)
	{
		this->InitializeScene(name);
	}

	GScene::~GScene()
	{

	}

	void GScene::InitializeScene(const std::string& name)
	{
		this->SceneName = name;
		this->SceneState = GScene::GE_STATE_NONE;

		this->EditCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, -15.0f), GVector3(), GPerspectiveProjection());
		this->RuntimeCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, 0.0f), GVector3(), GPerspectiveProjection());

		this->SceneRootEntity = GEntity::CreateNewEntity("Root", this);

		this->LightRegistry = std::make_shared<GLightRegistry>();

		this->CameraEntity = this->CreateEntity("Main Camera");
		this->CameraEntity->AddComponent<GCameraComponent>(this->RuntimeCamera);
	}

	std::shared_ptr<GEntity> GScene::CreateEntity(const std::string& entityName, std::string rootName)
	{
		auto entity =  GEntity::CreateNewEntity(entityName, this);
		this->SceneEntityList[entityName] = entity;

		this->SetEntityParent(entityName, rootName);
		this->SceneEntityList[entityName]->AddComponent<GTagComponent>(entityName);
		this->SceneEntityList[entityName]->AddComponent<GTransformComponent>(GTransform({}, {}, {1.0f, 1.0f, 1.0f}));

		return this->SceneEntityList[entityName];
	}

	void GScene::SwitchSceneState(State state)
	{
		if (state == GE_STATE_NONE)
		{
			if (this->SceneState == GE_STATE_RUNTIME)
			{
				this->EndUpRuntime();
			}

			this->SceneState = GE_STATE_NONE;
		}
		else if (state == GE_STATE_EDIT)
		{
			if (this->SceneState == GE_STATE_RUNTIME)
			{
				this->EndUpRuntime();
			}

			this->SceneState = GE_STATE_EDIT;
		}
		else if (state == GE_STATE_RUNTIME)
		{
			if (this->SceneState != GE_STATE_RUNTIME)
			{
				this->StartRuntime();
			}

			this->SceneState = GE_STATE_RUNTIME;
		}
	}

	const GScene::State& GScene::GetSceneState() const noexcept
	{
		return this->SceneState;
	}

	void GScene::Update()
	{
		switch (this->SceneState)
		{
			case GE_STATE_NONE:
			{
				//Do nothing.

				break;
			}

			case GE_STATE_EDIT:
			{
				this->UpdateEdit();

				break;
			}

			case GE_STATE_RUNTIME:
			{
				this->UpdateRuntime();

				break;
			}
		}
	}
	
	const GUUID& GScene::GetSceneId() const noexcept
	{
		return this->SceneId;
	}

	const std::string& GScene::GetSceneName() const noexcept
	{
		return this->SceneName;
	}

	std::shared_ptr<GLightRegistry> GScene::GetLightRegistry()
	{
		return this->LightRegistry;
	}

	std::shared_ptr<GCamera> GScene::GetEditCamera()
	{
		return this->EditCamera;
	}

	std::shared_ptr<GCamera> GScene::GetRuntimeCamera()
	{
		return this->RuntimeCamera;
	}

	const std::map<std::string, std::shared_ptr<GEntity>>& GScene::GetSceneEntityList() const noexcept
	{
		return this->SceneEntityList;
	}

	std::shared_ptr<GEntity> GScene::GetSceneRootEntity()
	{
		return this->SceneRootEntity;
	}

	bool GScene::HasEntity(const std::string& name)
	{
		return this->SceneEntityList.count(name) > 0;
	}

	bool GScene::HasEntity(const GUUID& id)
	{
		for (auto& entity : this->SceneEntityList)
		{
			if (entity.second->GetEntityId() == id)
			{
				return true;
			}
		}

		return false;
	}

	std::shared_ptr<GEntity> GScene::GetEntity(const std::string& name)
	{
		if (!this->SceneEntityList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No entity named '{}' found in scene", name));
		}

		return SceneEntityList[name];
	}

	std::shared_ptr<GEntity> GScene::GetEntity(const GUUID& id)
	{
		for (auto& entity : this->SceneEntityList)
		{
			if (entity.second->GetEntityId() == id)
			{
				return entity.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No entity with id : '{}' found in scene", (ULONGLONG)id));
	}

	void GScene::UpdateEdit()
	{
		this->UpdateEntityTransform(this->SceneRootEntity.get());
		this->LightRegistry->Update();

		if (GetAsyncKeyState('W'))
		{
			this->EditCamera->Translate(this->EditCamera->GetForwardVector() * 0.1f);
		}
		if (GetAsyncKeyState('S'))
		{
			this->EditCamera->Translate(this->EditCamera->GetBackwardVector() * 0.1f);
		}
		if (GetAsyncKeyState('A'))
		{
			this->EditCamera->Translate(this->EditCamera->GetLeftVector() * 0.1f);
		}
		if (GetAsyncKeyState('D'))
		{
			this->EditCamera->Translate(this->EditCamera->GetRightVector() * 0.1f);
		}
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			this->EditCamera->Translate(this->EditCamera->GetDownVector() * 0.1f);
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			this->EditCamera->Translate(this->EditCamera->GetUpVector() * 0.1f);
		}
		if (GetAsyncKeyState('Q'))
		{
			this->EditCamera->Rotate({ 0.0f, -0.1f, 0.0f });
		}
		if (GetAsyncKeyState('E'))
		{
			this->EditCamera->Rotate({ 0.0f, 0.1f, 0.0f });
		}
		if (GetAsyncKeyState('R'))
		{
			this->EditCamera->Rotate({ 0.1f, 0.0f, 0.0f });
		}
		if (GetAsyncKeyState('T'))
		{
			this->EditCamera->Rotate({ -0.1f, 0.0f, 0.0f });
		}

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GCameraComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GCameraComponent& CComponent)
				{
					CComponent.Camera->Position = TComponent.Transform.Position;
					CComponent.Camera->Rotation = TComponent.Transform.Rotation;

					this->RuntimeCamera = CComponent.Camera;
				});
		}

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GPointLightComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GPointLightComponent& PLComponent)
				{
					PLComponent.PointLight->Position = TComponent.Transform.Position;

					PLComponent.PointLight->Submit(this);
				});
		}

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GModelComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GModelComponent& MComponent)
				{
					if (!MComponent.Model.get())
					{
						auto AssetData = GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(MComponent.ModelAssetName)->GetAssetData<GModel::Data>();

						MComponent.Model = GModel::CreateNewModel(AssetData);
					}

					MComponent.Model->SetTransform(TComponent.Transform);
					MComponent.Model->SetAccumulatedMatrix(TComponent.AccumulatedMatrix);

					MComponent.Model->Submit("main");
				});
		}
	}

	void GScene::StartRuntime()
	{

	}

	void GScene::UpdateRuntime()
	{
		this->UpdateEntityTransform(this->SceneRootEntity.get());
		this->LightRegistry->Update();

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GCameraComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GCameraComponent& CComponent)
				{
					CComponent.Camera->Position = TComponent.Transform.Position;
					CComponent.Camera->Rotation = TComponent.Transform.Rotation;

					this->RuntimeCamera = CComponent.Camera;
				});
		}

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GPointLightComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GPointLightComponent& PLComponent)
				{
					PLComponent.PointLight->Position = TComponent.Transform.Position;

					PLComponent.PointLight->Submit(this);
				});
		}

		{
			auto view = this->EntityRegistry.view<GTransformComponent, GModelComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GModelComponent& MComponent)
				{
					if (!MComponent.Model.get())
					{
						auto AssetData = GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(MComponent.ModelAssetName)->GetAssetData<GModel::Data>();

						MComponent.Model = GModel::CreateNewModel(AssetData);
					}

					MComponent.Model->SetTransform(TComponent.Transform);
					MComponent.Model->SetAccumulatedMatrix(TComponent.AccumulatedMatrix);

					MComponent.Model->Submit("main");
				});
		}
	}

	void GScene::EndUpRuntime()
	{

	}

	void GScene::BuildEntityTree()
	{
		for (auto& entity : this->SceneEntityList)
		{
			this->SetEntityParent(entity.second->EntityName, entity.second->ParentEntityName);
		}
	}

	void GScene::SetEntityParent(const std::string& entityName, std::string rootName)
	{
		if (rootName == this->SceneRootEntity->GetEntityName())
		{
			this->SceneEntityList[entityName]->SetParent(this->SceneRootEntity.get());

			return;
		}
		else
		{
			if (this->SceneEntityList.count(this->SceneEntityList[entityName]->ParentEntityName) <= 0)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("Unknown parent entity : '{}'", this->SceneEntityList[entityName]->ParentEntityName));
			}

			this->SceneEntityList[entityName]->SetParent(this->SceneEntityList[this->SceneEntityList[entityName]->ParentEntityName].get());
		}
	}

	void GScene::UpdateEntityTransform(GEntity* entity)
	{
		GMatrix AccumulatedMatrix;
		if (entity->GetEntityName() == "Root")
		{
			AccumulatedMatrix = GMatrix::IdentityMatrix();
		}
		else
		{
			auto& Transform = entity->GetComponent<GTransformComponent>();

			AccumulatedMatrix = Transform.AccumulatedMatrix * Transform.Transform.GetTransformMatrix();
		}

		for (auto& child : entity->GetChildren())
		{
			child->GetComponent<GTransformComponent>().AccumulatedMatrix = AccumulatedMatrix;
			this->UpdateEntityTransform(child);
		}
	}
}
