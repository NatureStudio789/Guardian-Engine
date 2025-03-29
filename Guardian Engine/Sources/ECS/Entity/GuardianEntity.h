#ifndef _GE_GUARDIANENTITY_H_
#define _GE_GUARDIANENTITY_H_
#include "../Scene/GuardianScene.h"

namespace GE
{
	class GUARDIAN_API GuardianEntity
	{
	public:
		GuardianEntity();
		GuardianEntity(entt::entity handle, const GString name, GuardianScene* scene);
		GuardianEntity(entt::entity handle, 
			const GString name, const GuardianUUID& uuid, GuardianScene* scene);
		GuardianEntity(const GuardianEntity& other);
		~GuardianEntity();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("The entity has already had this component!");
			}

			return this->EntityScene->SceneRegistry.emplace<T>(this->EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("The entity hasn't already had this component!");
			}

			this->EntityScene->SceneRegistry.remove<T>(this->EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!this->HasComponent<T>())
			{
				throw GUARDIAN_ERROR_EXCEPTION("The entity hasn't already had this component!");
			}

			return this->EntityScene->SceneRegistry.get<T>(this->EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return this->EntityScene->SceneRegistry.any_of<T>(this->EntityHandle);
		}

		const GString& GetEntityName() const noexcept;
		const entt::entity& GetEntityHandle() const noexcept;
		const GuardianUUID& GetEntityId() const noexcept;

		operator uint32_t() { return (uint32_t)this->EntityHandle; }

	protected:
		entt::entity EntityHandle;
		GuardianUUID EntityId;
		GString EntityName;
		GuardianScene* EntityScene;
	};
}

#endif