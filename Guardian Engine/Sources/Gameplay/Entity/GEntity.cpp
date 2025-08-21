#include "GEntity.h"

namespace GE
{
	GEntity::GEntity()
	{
		this->EntityName = "";
		this->EntityScene = null;
		this->EntityHandle = entt::entity();

		this->ParentEntity = null;
		this->ChildrenEntity.clear();
	}

	GEntity::GEntity(const std::string& name, GScene* scene) : GEntity()
	{
		this->InitializeEntity(name, scene);
	}

	GEntity::GEntity(const GUUID& id, const std::string& name, GScene* scene)
	{
		this->InitializeEntity(id, name, scene);
	}

	GEntity::GEntity(const GEntity& other)
	{
		this->EntityId = other.EntityId;
		this->EntityName = other.EntityName;
		this->EntityHandle = other.EntityHandle;

		this->ParentEntity = other.ParentEntity;
		this->ChildrenEntity = other.ChildrenEntity;

		this->EntityScene = other.EntityScene;
	}

	GEntity::~GEntity()
	{
		this->EntityId = 0;
		this->EntityName.clear();
		this->EntityHandle = (entt::entity)0;

		this->ParentEntity = null;
		this->ChildrenEntity.clear();

		this->EntityScene = null;
	}

	void GEntity::InitializeEntity(const std::string& name, GScene* scene)
	{
		this->EntityName = name;
		this->EntityScene = scene;

		this->EntityHandle = this->EntityScene->EntityRegistry.create();
	}

	void GEntity::InitializeEntity(const GUUID& id, const std::string& name, GScene* scene)
	{
		this->EntityId = id;
		this->EntityName = name;
		this->EntityScene = scene;

		this->EntityHandle = this->EntityScene->EntityRegistry.create();
	}

	void GEntity::SetParent(GEntity* parent)
	{
		this->ParentEntityName = parent->EntityName;
		parent->AddChild(this);
	}

	void GEntity::AddChild(GEntity* child)
	{
		if (child->HasParent())
		{
			child->GetParent()->RemoveChild(this->EntityName);
		}

		child->ParentEntity = this;
		this->ChildrenEntity.push_back(child);
	}

	void GEntity::RemoveChild(GEntity* child)
	{
		for (auto it = this->ChildrenEntity.begin(); it != this->ChildrenEntity.end(); it++)
		{
			if (child == *it)
			{
				this->ChildrenEntity.erase(it);
				break;
			}
		}
	}

	void GEntity::RemoveChild(const std::string& name)
	{
		for (auto it = this->ChildrenEntity.begin(); it != this->ChildrenEntity.end(); it++)
		{
			if ((*it)->EntityName == name)
			{
				this->ChildrenEntity.erase(it);
				break;
			}
		}
	}

	void GEntity::RemoveChild(const GUUID& id)
	{
		for (auto it = this->ChildrenEntity.begin(); it != this->ChildrenEntity.end(); it++)
		{
			if ((*it)->EntityId == id)
			{
				this->ChildrenEntity.erase(it);
				break;
			}
		}
	}

	bool GEntity::HasParent()
	{
		return this->ParentEntity != null;
	}

	GEntity* GEntity::GetParent()
	{
		return this->ParentEntity;
	}

	const std::vector<GEntity*>& GEntity::GetChildren() const noexcept
	{
		return this->ChildrenEntity;
	}

	bool GEntity::HasChild(const std::string& childName)
	{
		for (auto child : this->ChildrenEntity)
		{
			if (child->EntityName == childName)
			{
				return true;
			}
		}

		return false;
	}

	bool GEntity::HasChild(const GUUID& childId)
	{
		for (auto child : this->ChildrenEntity)
		{
			if (child->EntityId == childId)
			{
				return true;
			}
		}

		return false;
	}

	bool GEntity::HasChildren() const noexcept
	{
		return this->ChildrenEntity.empty() != true;
	}

	GEntity* GEntity::GetChildEntity(const std::string& childName)
	{
		for (auto child : this->ChildrenEntity)
		{
			if (child->EntityName == childName)
			{
				return child;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("This entity has not child named '{}'!", childName));
	}

	GEntity* GEntity::GetChildEntity(const GUUID& childId)
	{
		for (auto child : this->ChildrenEntity)
		{
			if (child->EntityId == childId)
			{
				return child;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("This entity has not child with id '{}'!", (unsigned long long)childId));
	}

	const GUUID& GEntity::GetEntityId() const noexcept
	{
		return this->EntityId;
	}

	const std::string& GEntity::GetEntityName() const noexcept
	{
		return this->EntityName;
	}

	const entt::entity& GEntity::GetEntityHandle() const noexcept
	{
		return this->EntityHandle;
	}
}
