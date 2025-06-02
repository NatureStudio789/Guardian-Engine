#include "GuardianEntity.h"

namespace GE
{
	GuardianEntity::GuardianEntity()
	{
		this->EntityId = GuardianUUID();
		this->EntityName = "Unnamed Entity";
		this->EntityScene = null;
		this->IsEntitySelected = false;
	}

	GuardianEntity::GuardianEntity(entt::entity handle, const GString name, GuardianScene* scene)
	{
		this->EntityId = GuardianUUID();
		this->EntityHandle = handle;
		this->EntityName = name;
		this->EntityScene = scene;
		this->IsEntitySelected = false;
	}

	GuardianEntity::GuardianEntity(entt::entity handle, const GString name, 
		const GuardianUUID& uuid, GuardianScene* scene)
	{
		this->EntityId = uuid;
		this->EntityHandle = handle;
		this->EntityName = name;
		this->EntityScene = scene;
		this->IsEntitySelected = false;
	}

	GuardianEntity::GuardianEntity(const GuardianEntity& other)
	{
		this->EntityHandle = other.EntityHandle;
		this->EntityId = other.EntityId;
		this->EntityName = other.EntityName;
		this->EntityScene = other.EntityScene;
		this->IsEntitySelected = other.IsEntitySelected;
	}

	GuardianEntity::~GuardianEntity()
	{
		this->EntityScene = null;
	}

	void GuardianEntity::SelectEntity()
	{
		this->IsEntitySelected = true;
	}

	void GuardianEntity::UnselectEntity()
	{
		this->IsEntitySelected = false;
	}

	const bool& GuardianEntity::IsSelected() const noexcept
	{
		return this->IsEntitySelected;
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
