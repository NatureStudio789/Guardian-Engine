#ifndef _GE_GUARDIANSCENE_H_
#define _GE_GUARDIANSCENE_H_
#include "../Component/GuardianComponent.h"

namespace GE
{
	class GUARDIAN_API GuardianEntity;

	enum GUARDIAN_API GuardianSceneState
	{
		GE_SCENE_EDIT,
		GE_SCENE_RUNTIME,
	};

	class GUARDIAN_API GuardianScene : public GuardianSerializable
	{
	public:
		GuardianScene();
		GuardianScene(std::shared_ptr<GuardianGraphics> graphics);
		GuardianScene(const GuardianScene& other);
		~GuardianScene() override = default;

		void InitializeScene();
		void SetSceneName(const GString& name);

		void LoadScene(std::shared_ptr<GuardianGraphics> graphics);
		void LoadSceneAs(std::shared_ptr<GuardianGraphics> graphics, const GString& sceneFilePath);
		void SaveScene();
		void SaveSceneAs(const GString& filePath);

		void UpdateScene(GuardianTimestep deltaTime);
		void UpdateEditScene(GuardianTimestep deltaTime);
		void StartRuntime();
		void UpdateRuntimeScene(GuardianTimestep deltaTime);
		void StopRuntime();

		std::shared_ptr<GuardianEntity> CreateEntity(const GString& name);
		std::shared_ptr<GuardianEntity> CreateEntity(const GString& name, const GuardianUUID& uuid);
		void RemoveEntity(const entt::entity& handle);
		void RemoveAllEntity();

		const GuardianSceneState& GetSceneState() const;
		std::shared_ptr<GuardianEntity> GetEntity(const GuardianUUID& uuid);

	private:
		void CreatePhysicsWorld();

		void Deserialize(const GString& sceneFilePath) override;
		const GString Serialize() override;

		void SaveEntity(YAML::Emitter& output, std::shared_ptr<GuardianEntity> entity);

		void UpdateProjectionAspect(float newWidth, float newHeight);

		GString SceneName;
		GuardianSceneState SceneState;
		entt::registry SceneRegistry;
		std::map<entt::entity, std::shared_ptr<GuardianEntity>> SceneEntityList;
		GString CurrentScenePath;

		std::shared_ptr<GuardianCamera> EditorCamera;
		bool ShouldOperateCamera;
		std::shared_ptr<GuardianMesh> SceneGrid;
		std::shared_ptr<GuardianCamera> RuntimeCamera;

		PxScene* PhysicsWorld;
		GVector3 SceneGravity;

		friend class GuardianEntity;
		friend class GuardianEditor;
		friend class GuardianSceneHierarchyPanel;
		friend class GuardianScenePanel;
	};
}

#endif