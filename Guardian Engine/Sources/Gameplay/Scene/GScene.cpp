#include "GScene.h"
#include "../Entity/GEntity.h"

namespace GE
{
	GScene::GScene()
	{
		this->SceneName = "";
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

		this->SceneRootEntity = GEntity::CreateNewEntity("Root", this);
	}

	std::shared_ptr<GEntity> GScene::CreateEntity(const std::string& entityName, std::string rootName)
	{
		auto entity =  GEntity::CreateNewEntity(entityName, this);
		this->SceneEntityList[entityName] = entity;

		this->SetEntityParent(this->SceneRootEntity.get(), entityName, rootName);
		this->SceneEntityList[entityName]->AddComponent<GTagComponent>(entityName);
		this->SceneEntityList[entityName]->AddComponent<GTransformComponent>(GTransform({}, {}, {1.0f, 1.0f, 1.0f}));

		return this->SceneEntityList[entityName];
	}

	void GScene::Update()
	{
		this->UpdateEntityTransform(this->SceneRootEntity.get());

		{
			auto view = this->Registry.view<GTransformComponent, GModelComponent>();
			view.each([=](const auto& e, GTransformComponent& TComponent, GModelComponent& MComponent)
			{
				MComponent.Model->SetTransform(TComponent.Transform);
				MComponent.Model->SetAccumulatedMatrix(TComponent.AccumulatedMatrix);

				MComponent.Model->Submit("main");
			});
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

	void GScene::SetEntityParent(GEntity* entity, const std::string& entityName, std::string rootName)
	{
		if (rootName == entity->GetEntityName())
		{
			this->SceneEntityList[entityName]->SetParent(entity);

			return;
		}
		else
		{
			for (auto child : entity->GetChildren())
			{
				this->SetEntityParent(child, entityName, rootName);
			}
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
