#ifndef _GE_GUARDIANPROJECT_H_
#define _GE_GUARDIANPROJECT_H_
#include "../AssetSystem/GuardianAssetSystem.h"

namespace GE
{
	class GUARDIAN_API GuardianProject : public GuardianSerializable
	{
	public:
		enum State
		{
			GE_PROJECT_EDIT,
			GE_PROJECT_RUNTIME
		};

	public:
		GuardianProject();
		GuardianProject(const GString& projectName, const GString& projectDirectory);
		GuardianProject(const GuardianProject& other);
		~GuardianProject() override;

		void InitializeProject(const GString& projectName, const GString& projectDirectory);

		void LoadProject();
		void LoadProject(const GString& projectFilePath);

		void SaveProject();
		void SaveProject(const GString& projectFilePath);

		void AddProjectScene(GString sceneName);
		void SetCurrentScene(GString sceneName);

		void UpdateProject();
		void StartRuntime();
		void StopRuntime();

		const State& GetProjectState() const noexcept;
		std::shared_ptr<GuardianScene> GetCurrentScene();

	private:
		const GString WriteSerializationData();
		void ReadSerializationData(const GString& serializationData);

		void UpdateEditProject();
		void UpdateRuntimeProject();

		GString ProjectName;
		GString ProjectFileDirectory;
		GString ProjectAssetPath;

		State ProjectState;
		GString CurrentSceneName;
		std::map<GString, std::shared_ptr<GuardianScene>> ProjectSceneList;
	};
}

#endif