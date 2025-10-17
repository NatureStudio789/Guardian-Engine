#ifndef _GE_GSCENE_H_
#define _GE_GSCENE_H_
#include "../Component/GComponent.h"

namespace GE
{
	class GEntity;

	class GUARDIAN_API GScene
	{
	public:
		enum State
		{
			GE_STATE_NONE,
			GE_STATE_EDIT,
			GE_STATE_RUNTIME
		};

	public:
		GScene();
		GScene(const std::string& name);
		GScene(const GScene& ) = default;
		~GScene();

		void InitializeScene(const std::string& name);

		std::shared_ptr<GEntity> CreateEntity(const std::string& entityName, std::string rootName = "Root");
		void RemoveAllEntity();

		void SwitchSceneState(State state);
		const State& GetSceneState() const noexcept;

		void Update();

		const GUUID& GetSceneId() const noexcept;
		const std::string& GetSceneName() const noexcept;
		bool GetRenderingInitialized() const noexcept;
		std::shared_ptr<GLightRegistry> GetLightRegistry();
		std::shared_ptr<GCamera> GetEditCamera();
		std::shared_ptr<GCamera> GetRuntimeCamera();
		std::shared_ptr<GFramebuffer> GetEditFramebuffer();
		std::shared_ptr<GFramebuffer> GetRuntimeFramebuffer();
		std::shared_ptr<GMission> GetEditRenderMission();
		std::shared_ptr<GMission> GetRuntimeRenderMission();
		const std::map<std::string, std::shared_ptr<GEntity>>& GetSceneEntityList() const noexcept;
		std::shared_ptr<GEntity> GetSceneRootEntity();
		bool HasEntity(const std::string& name);
		bool HasEntity(const GUUID& id);
		bool HasEntity(const entt::entity& handle);
		std::shared_ptr<GEntity> GetEntity(const std::string& name);
		std::shared_ptr<GEntity> GetEntity(const GUUID& id);
		std::shared_ptr<GEntity> GetEntity(const entt::entity& handle);

		static std::shared_ptr<GScene> CreateNewScene(const std::string& name)
		{
			return std::make_shared<GScene>(name);
		}

	private:
		void InitializeSceneRender();

		void UpdateEdit();

		void StartRuntime();
		void UpdateRuntime();
		void EndUpRuntime();

		void BuildEntityTree();
		void SetEntityParent(const std::string& entityName, std::string rootName);
		void UpdateEntityTransform(GEntity* entity);

		GUUID SceneId = GUUID();
		std::string SceneName;
		State SceneState;

		entt::registry EntityRegistry;

		std::map<std::string, std::shared_ptr<GEntity>> SceneEntityList;
		std::shared_ptr<GEntity> SceneRootEntity;

		std::shared_ptr<GLightRegistry> LightRegistry;

		std::shared_ptr<GPhysicsWorld> PhysicsWorld;

		std::shared_ptr<GEntity> CameraEntity;

		bool IsRenderingInitialized;
		std::shared_ptr<GMission> EditRenderMission;
		std::shared_ptr<GMission> RuntimeRenderMission;
		std::shared_ptr<GCamera> EditCamera;
		std::shared_ptr<GCamera> RuntimeCamera;

		friend class GEntity;
		friend class GSceneSerializer;
		friend class GSceneEditor;
	};
}

#endif