#ifndef _GE_GENTITY_H_
#define _GE_GENTITY_H_
#include "../Scene/GSceneRegistry.h"

namespace GE
{
	class GUARDIAN_API GEntity
	{
	public:
		GEntity();
		GEntity(const std::string& name, GScene* scene);
		GEntity(const GUUID& id, const std::string& name, GScene* scene);
		GEntity(const GEntity& other);
		~GEntity();

		void InitializeEntity(const std::string& name, GScene* scene);
		void InitializeEntity(const GUUID& id, const std::string& name, GScene* scene);

		void SetParent(GEntity* parent);
		void AddChild(GEntity* child);
		void RemoveChild(GEntity* child);
		void RemoveChild(const std::string& name);
		void RemoveChild(const GUUID& id);

		bool HasParent();
		GEntity* GetParent();
		const std::vector<GEntity*>& GetChildren() const noexcept;
		bool HasChild(const std::string& childName);
		bool HasChild(const GUUID& childId);
		bool HasChildren() const noexcept;
		GEntity* GetChildEntity(const std::string& childName);
		GEntity* GetChildEntity(const GUUID& childId);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("This entity already has this component!");
			}

			return this->EntityScene->EntityRegistry.emplace<T>(this->EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("No matched component found in this entity!");
			}

			this->EntityScene->EntityRegistry.remove<T>(this->EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("No matched component found in this entity!");
			}

			return this->EntityScene->EntityRegistry.get<T>(this->EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return this->EntityScene->EntityRegistry.any_of<T>(this->EntityHandle);
		}

		const GUUID& GetEntityId() const noexcept;
		const std::string& GetEntityName() const noexcept;
		const entt::entity& GetEntityHandle() const noexcept;

		static std::shared_ptr<GEntity> CreateNewEntity(
			const std::string& name, GScene* scene)
		{
			return std::make_shared<GEntity>(name, scene);
		}
		static std::shared_ptr<GEntity> CreateNewEntity(
			const GUUID& id, const std::string& name, GScene* scene)
		{
			return std::make_shared<GEntity>(id, name, scene);
		}

	private:
		GUUID EntityId = GUUID();
		std::string EntityName;

		GEntity* ParentEntity;
		std::vector<GEntity*> ChildrenEntity;

		entt::entity EntityHandle;
		GScene* EntityScene;
		
		//This is only for serializer and scene.
		std::string ParentEntityName;

		friend class GScene;
		friend class GEntitySerializer;
	};
}

#endif