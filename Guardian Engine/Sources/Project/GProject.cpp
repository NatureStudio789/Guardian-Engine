#include "GProject.h"
#include "../Gameplay/Serializer/GSceneSerializer.h"
#include "../Asset/Loader/GAssetLoader.h"

namespace GE
{
	std::shared_ptr<GProject> GProject::Instance = std::make_shared<GProject>();


	GProject::GProject()
	{
		this->ProjectId = 0;
		this->ProjectName = "";
		this->ProjectDirectory = "";
		this->ProjectAssetDirectory = "";

		this->ProjectAssetLoader = null;

		this->StartupSceneAssetId = 0;
		this->ActiveScene = null;
	}

	GProject::GProject(const std::string& projectFilePath)
	{
		this->InitializeProject(projectFilePath);
	}

	GProject::GProject(const GProject& other)
	{
		this->ProjectId = other.ProjectId;
		this->ProjectName = other.ProjectName;
		this->ProjectDirectory = other.ProjectDirectory;
		this->ProjectAssetDirectory = other.ProjectAssetDirectory;
		
		this->ProjectAssetLoader = other.ProjectAssetLoader;

		this->StartupSceneAssetId = other.StartupSceneAssetId;
		this->ActiveScene = other.ActiveScene;
	}

	GProject::~GProject()
	{
		this->ProjectId = 0;
		this->ProjectName.clear();
		this->ProjectDirectory.clear();
		this->ProjectAssetDirectory.clear();

		this->ProjectAssetLoader.reset();
		this->ProjectAssetLoader = null;

		this->StartupSceneAssetId = 0;
		this->ActiveScene.reset();
		this->ActiveScene = null;
	}

	void GProject::InitializeProject(const std::string& projectFilePth)
	{
		this->ProjectDirectory = GUtil::GetFilePathDirectory(projectFilePth);

		GProjectSerializer::Import(projectFilePth, this);

		this->ProjectAssetLoader = std::make_shared<GAssetLoader>(this->ProjectName + "AssetLoader", this->GetFullAssetDirectory());
		GAssetLoaderRegistry::RegistryAssetLoader(this->ProjectAssetLoader);
		GAssetLoaderRegistry::SetCurrentAssetLoader(this->ProjectAssetLoader->GetAssetLoaderName());

		this->ActiveScene = std::make_shared<GScene>();
		GSceneSerializer::Deserialize(this->ActiveScene, GAssetLoaderRegistry::GetCurrentAssetLoader()->
			GetAsset(this->StartupSceneAssetId)->GetAssetData<YAML::Node>());
		GSceneRegistry::RegisterScene(this->ActiveScene);
		GSceneRegistry::SetActiveScene(this->ActiveScene->GetSceneName());
	}

	void GProject::CreateProject(const std::string& projectName, 
		const std::string& projectDirectory, const std::string& projectAssetDirectory)
	{
		this->ProjectId = GUUID();
		this->ProjectName = projectName;
		this->ProjectDirectory = projectDirectory;
		this->ProjectAssetDirectory = projectAssetDirectory;
		if (!std::filesystem::exists(this->GetFullAssetDirectory()))
		{
			std::filesystem::create_directory(this->GetFullAssetDirectory());
		}

		this->ActiveScene = GScene::CreateNewScene("Sample Scene");

		GSceneSerializer::Export(GUtil::ExtendDirectory(this->GetFullAssetDirectory(), this->ActiveScene->GetSceneName() + ".gscene"), this->ActiveScene);
		GSceneRegistry::RegisterScene(this->ActiveScene);
		GSceneRegistry::SetActiveScene(this->ActiveScene->GetSceneName());

		this->ProjectAssetLoader = std::make_shared<GAssetLoader>(this->ProjectName + "AssetLoader", this->GetFullAssetDirectory());
		GAssetLoaderRegistry::RegistryAssetLoader(this->ProjectAssetLoader);
		GAssetLoaderRegistry::SetCurrentAssetLoader(this->ProjectAssetLoader->GetAssetLoaderName());

		this->StartupSceneAssetId = this->ProjectAssetLoader->GetAsset("Sample Scene")->GetAssetId();

		GProjectSerializer::Export(this->GetProjectFilePath(), this);
	}

	const GUUID& GProject::GetProjectId() const noexcept
	{
		return this->ProjectId;
	}

	const std::string& GProject::GetProjectName() const noexcept
	{
		return this->ProjectName;
	}

	const std::string& GProject::GetProjectDirectory() const noexcept
	{
		return this->ProjectDirectory;
	}

	const std::string GProject::GetProjectFilePath() const noexcept
	{
		return GUtil::ExtendDirectory(this->ProjectDirectory, this->ProjectName + ".geproj");
	}

	const std::string& GProject::GetProjectAssetDirectory() const noexcept
	{
		return this->ProjectAssetDirectory;
	}

	const std::string GProject::GetFullAssetDirectory() const noexcept
	{
		return GUtil::ExtendDirectory(this->ProjectDirectory, this->ProjectAssetDirectory);
	}

	const GUUID& GProject::GetStartupSceneAssetId() const noexcept
	{
		return this->StartupSceneAssetId;
	}

	std::shared_ptr<GScene> GProject::GetActiveScene()
	{
		return this->ActiveScene;
	}
}
