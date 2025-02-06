#ifndef _GE_GUARDIANSCENE_H_
#define _GE_GUARDIANSCENE_H_
#include "../Component/GuardianComponent.h"

namespace guardian
{
	class GUARDIAN_API GuardianEntity;

	enum GUARDIAN_API GuardianSceneState
	{
		GE_SCENE_EDIT,
		GE_SCENE_RUNTIME,
	};

	class GUARDIAN_API GuardianScene
	{
	public:
		GuardianScene();
		GuardianScene(std::shared_ptr<GuardianGraphics> graphics);
		GuardianScene(const GuardianScene& other);
		~GuardianScene() = default;

		void InitializeScene(std::shared_ptr<GuardianGraphics> graphics);
		void InitializeSceneAs(std::shared_ptr<GuardianGraphics> graphics, const GString& sceneFilePath);
		void SaveScene();
		void SaveSceneAs(const GString& filePath);

		void UpdateScene(GuardianTimestep deltaTime);
		void UpdateEditScene(GuardianTimestep deltaTime);
		void StartRuntime();
		void UpdateRuntimeScene();
		void StopRuntime();

		std::shared_ptr<GuardianEntity> CreateEntity(const GString& name);
		std::shared_ptr<GuardianEntity> CreateEntity(const GString& name, const GuardianUUID& uuid);
		void RemoveEntity(const entt::entity& handle);
		void RemoveAllEntity();

		const GuardianSceneState& GetSceneState() const;
		std::shared_ptr<GuardianEntity> GetEntity(const GuardianUUID& uuid);

	private:
		void DeserializeScene(const GString& filePath);
		void SerializeScene(const GString& filePath);

		void SaveEntity(YAML::Emitter& output, std::shared_ptr<GuardianEntity> entity);

		void UpdateProjectionAspect(float newWidth, float newHeight);

		GuardianSceneState SceneState;
		entt::registry SceneRegistry;
		std::map<entt::entity, std::shared_ptr<GuardianEntity>> SceneEntityList;
		GString CurrentScenePath;

		std::shared_ptr<GuardianCamera> EditorCamera;
		bool ShouldOperateCamera;
		std::shared_ptr<GuardianModel> SceneGrid;
		std::shared_ptr<GuardianCamera> RuntimeCamera;

		friend class GuardianEntity;
		friend class GuardianEditor;
		friend class GuardianSceneHierarchyPanel;
		friend class GuardianScenePanel;
	};
}

#endif