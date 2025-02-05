#include "GuardianEntity.h"

namespace guardian
{
	GuardianEntity::GuardianEntity()
	{
		this->EntityId = GuardianUUID();
		this->EntityName = "Unnamed Entity";
		this->EntityScene = null;
	}

	GuardianEntity::GuardianEntity(entt::entity handle, const GString name, GuardianScene* scene)
	{
		this->EntityId = GuardianUUID();
		this->EntityHandle = handle;
		this->EntityName = name;
		this->EntityScene = scene;
	}

	GuardianEntity::GuardianEntity(entt::entity handle, const GString name, 
		const GuardianUUID& uuid, GuardianScene* scene)
	{
		this->EntityId = uuid;
		this->EntityHandle = handle;
		this->EntityName = name;
		this->EntityScene = scene;
	}

	GuardianEntity::GuardianEntity(const GuardianEntity& other)
	{
		this->EntityHandle = other.EntityHandle;
		this->EntityId = other.EntityId;
		this->EntityName = other.EntityName;
		this->EntityScene = other.EntityScene;
	}

	GuardianEntity::~GuardianEntity()
	{
		this->EntityScene = null;
	}

	const GString& GuardianEntity::GetEntityName() const noexcept
	{
		return this->EntityName;
	}

	const entt::entity& GuardianEntity::GetEntityHandle() const noexcept
	{
		return this->EntityHandle;
	}

	const GuardianUUID& GuardianEntity::GetEntityId() const noexcept
	{
		return this->EntityId;
	}
}
