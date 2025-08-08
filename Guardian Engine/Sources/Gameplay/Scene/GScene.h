#ifndef _GE_GSCENE_H_
#define _GE_GSCENE_H_
#include "../Component/GComponent.h"

namespace GE
{
	class GEntity;

	class GUARDIAN_API GScene
	{
	public:
		GScene();
		GScene(const std::string& name);
		GScene(const GScene& other) = default;
		~GScene();

		void InitializeScene(const std::string& name);

		std::shared_ptr<GEntity> CreateEntity(const std::string& entityName, std::string rootName = "Root");

		void Update();

		const GUUID& GetSceneId() const noexcept;
		const std::string& GetSceneName() const noexcept;

		static std::shared_ptr<GScene> CreateNewScene(const std::string& name)
		{
			return std::make_shared<GScene>(name);
		}

	private:
		void SetEntityParent(GEntity* entity, const std::string& entityName, std::string rootName);
		void UpdateEntityTransform(GEntity* entity);

		GUUID SceneId = GUUID();
		std::string SceneName;

		entt::registry Registry;

		std::map<std::string, std::shared_ptr<GEntity>> SceneEntityList;
		std::shared_ptr<GEntity> SceneRootEntity;

		friend class GEntity;
	};
}

#endif