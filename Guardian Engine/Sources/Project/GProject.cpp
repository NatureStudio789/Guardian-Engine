#include "GProject.h"
#include "../Gameplay/Serializer/GSceneSerializer.h"
#include "../Asset/Loader/GAssetLoader.h"

namespace GE
{
	std::shared_ptr<GProject> GProject::Instance;


	GProject::GProject()
	{
		this->ProjectId = 0;
		this->ProjectName = "";
		this->ProjectDirectory = "";
		this->ProjectAssetDirectory = "";

		this->ProjectAssetLoader = null;

		this->ActiveSceneName = "";
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

		this->ActiveSceneName = other.ActiveSceneName;
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

		this->ActiveSceneName.clear();
		this->ActiveScene.reset();
		this->ActiveScene = null;
	}

	void GProject::InitializeProject(const std::string& projectFilePth)
	{
		this->ProjectDirectory = GUtil::GetFilePathDirectory(projectFilePth);

		GProjectSerializer::Import(this->GetProjectFilePath(), this);

		this->ProjectAssetLoader = std::make_shared<GAssetLoader>(this->GetFullAssetDirectory());

		GSceneSerializer::Deserialize(this->ActiveScene, this->ProjectAssetLoader->GetAsset(this->ActiveSceneName)->GetAssetData<YAML::Node>());
		GSceneRegistry::RegisterScene(this->ActiveScene);
		GSceneRegistry::SetActiveScene(this->ActiveSceneName);
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

		this->ActiveSceneName = "Sample Scene";
		this->ActiveScene = GScene::CreateNewScene(this->ActiveSceneName);

		auto MainCamera = this->ActiveScene->CreateEntity("Main Camera");
		MainCamera->AddComponent<GCameraComponent>();

		GSceneSerializer::Export(GUtil::ExtendDirectory(this->ProjectAssetDirectory, this->ActiveSceneName + ".gscene"), this->ActiveScene);
		GSceneRegistry::RegisterScene(this->ActiveScene);
		GSceneRegistry::SetActiveScene(this->ActiveSceneName);

		this->ProjectAssetLoader = std::make_shared<GAssetLoader>(this->GetFullAssetDirectory());
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

	const std::string& GProject::GetActiveSceneName() const noexcept
	{
		return this->ActiveSceneName;
	}

	std::shared_ptr<GAssetLoader> GProject::GetProjectAssetLoader()
	{
		return this->ProjectAssetLoader;
	}

	std::shared_ptr<GScene> GProject::GetActiveScene()
	{
		return this->ActiveScene;
	}
}
