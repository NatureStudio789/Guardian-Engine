#ifndef _GE_GPROJECT_H_
#define _GE_GPROJECT_H_
#include "Packer/GProjectPacker.h"

namespace GE
{
	class GUARDIAN_API GAsset;
	class GUARDIAN_API GScene;

	class GUARDIAN_API GProject
	{
	public:
		GProject();
		GProject(const std::string& projectFilePath);
		GProject(const GProject& other);
		~GProject();

		void InitializeProject(const std::string& projectFilePath);
		void CreateProject(const std::string& projectName, const std::string& projectDirectory,
			const std::string& projectAssetDirectory = "Assets\\");

		const GUUID& GetProjectId() const noexcept;
		const std::string& GetProjectName() const noexcept;
		const std::string& GetProjectDirectory() const noexcept;
		const std::string GetProjectFilePath() const noexcept;
		const std::string& GetProjectAssetDirectory() const noexcept;
		const std::string GetFullAssetDirectory() const noexcept;

		const GUUID& GetStartupSceneAssetId() const noexcept;

		std::shared_ptr<GScene> GetActiveScene();

		static std::shared_ptr<GProject> Instance;

	private:
		GUUID ProjectId;
		std::string ProjectName;
		std::string ProjectDirectory;
		std::string ProjectAssetDirectory;
		
		std::shared_ptr<GAssetLoader> ProjectAssetLoader;
		
		GUUID StartupSceneAssetId;
		std::shared_ptr<GScene> ActiveScene;

		friend class GProjectSerializer;
		friend class GProjectPacker;
	};
}

#endif