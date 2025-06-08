#include "GuardianProject.h"

namespace GE
{
	GuardianProject::GuardianProject() : GuardianSerializable()
	{
		this->ProjectName = "New Project";
		this->ProjectAssetPath = "";
		this->ProjectSceneList.clear();
		this->CurrentSceneName = "Default Scene";
		this->ProjectState = GE_PROJECT_EDIT;
	}

	GuardianProject::GuardianProject(const GString& projectName, const GString& projectDirectory)
	{
		this->InitializeProject(projectName, projectDirectory);
	}

	GuardianProject::GuardianProject(const GuardianProject& other) : 
		GuardianSerializable(other)
	{
		this->ProjectName = other.ProjectName;
		this->ProjectAssetPath = other.ProjectAssetPath;
		this->ProjectSceneList = other.ProjectSceneList;
		this->CurrentSceneName = other.CurrentSceneName;
		this->ProjectState = other.ProjectState;
	}

	GuardianProject::~GuardianProject()
	{
		this->ClearFile();
		this->ProjectName = "";
		this->ProjectAssetPath = "";
		
		for (auto& scene : this->ProjectSceneList)
		{
			scene.second.reset();
			scene.second = null;
		}
		this->ProjectSceneList.clear();
		this->CurrentSceneName = "";

		this->ProjectState = GE_PROJECT_EDIT;
	}

	void GuardianProject::InitializeProject(const GString& projectName, const GString& projectDirectory)
	{
		this->ProjectName = projectName;
		this->ProjectFileDirectory = projectDirectory;
		if (this->ProjectFileDirectory[this->ProjectFileDirectory.size() - 1] == '/' ||
			this->ProjectFileDirectory[this->ProjectFileDirectory.size() - 1] == '\\')
		{
			this->ProjectAssetPath = this->ProjectFileDirectory + "Assets\\";
		}
		else
		{
			this->ProjectAssetPath = this->ProjectFileDirectory + "\\Assets\\";
		}

		GuardianAssetSystem::InitializeAssetSystem(this->ProjectAssetPath);

		this->AddProjectScene("Default Scene");
		this->CurrentSceneName = "Default Scene";
		this->ProjectSceneList["Default Scene"]->SaveSceneAs(this->ProjectAssetPath + "Scenes\\Default Scene.gscene");

		for (auto& scene : GuardianAssetSystem::GetSceneList())
		{
			if (this->ProjectSceneList.count(scene.first) <= 0)
			{
				this->ProjectSceneList[scene.first] = scene.second;
			}
		}

		for (auto& scene : this->ProjectSceneList)
		{
			scene.second->InitializeScene();
		}
	}

	void GuardianProject::LoadProject()
	{
		if (this->ProjectState == GE_PROJECT_RUNTIME)
		{
			this->StopRuntime();
		}

		this->Deserialize();
	}

	void GuardianProject::LoadProject(const GString& projectFilePath)
	{
		if (this->ProjectState == GE_PROJECT_RUNTIME)
		{
			this->StopRuntime();
		}

		this->Deserialize(projectFilePath);
	}

	void GuardianProject::SaveProject()
	{
		this->Serialize();
	}

	void GuardianProject::SaveProject(const GString& projectFilePath)
	{
		this->Serialize(projectFilePath);
	}

	void GuardianProject::AddProjectScene(GString sceneName)
	{
		if (this->ProjectSceneList.count(sceneName) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The project already has a scene called : '{}' !", sceneName));
		}

		auto scene = std::make_shared<GuardianScene>();
		scene->SetSceneName(sceneName);

		auto& camera = scene->CreateEntity("Camera");
		camera->AddComponent<GuardianTransformComponent>();
		auto& cam = camera->AddComponent<GuardianCameraComponent>();
		cam.Position = { 0.0f, 2.0f, -3.0f };
		cam.Direction = { 0.0f, 0.0f, 0.0f };
		cam.IsFreelook = false;

		this->ProjectSceneList[sceneName] = scene;
	}

	void GuardianProject::SetCurrentScene(GString sceneName)
	{
		if (this->ProjectSceneList.count(sceneName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No scene found in project called : '{}' !", sceneName));
		}

		this->ProjectSceneList[sceneName];
	}

	void GuardianProject::UpdateProject()
	{
		switch (this->ProjectState)
		{
			case GE_PROJECT_EDIT:
			{
				this->UpdateEditProject();
				break;
			}
			case GE_PROJECT_RUNTIME:
			{
				this->UpdateRuntimeProject();
				break;
			}
		}
	}

	void GuardianProject::StartRuntime()
	{
		this->ProjectState = GE_PROJECT_RUNTIME;

		this->ProjectSceneList[this->CurrentSceneName]->StartRuntime();
	}

	void GuardianProject::StopRuntime()
	{
		this->ProjectState = GE_PROJECT_EDIT;

		this->ProjectSceneList[this->CurrentSceneName]->StopRuntime();
	}

	const GuardianProject::State& GuardianProject::GetProjectState() const noexcept
	{
		return this->ProjectState;
	}

	std::shared_ptr<GuardianScene> GuardianProject::GetCurrentScene()
	{
		return this->ProjectSceneList[this->CurrentSceneName];
	}

	const GString GuardianProject::WriteSerializationData()
	{
		YAML::Emitter ProjectOutput;
		ProjectOutput << YAML::BeginMap;

		ProjectOutput << YAML::Key << "Project";
		ProjectOutput << YAML::Value << this->ProjectName;
		ProjectOutput << YAML::Key << "File Directory";
		ProjectOutput << YAML::Value << this->ProjectFileDirectory;
		ProjectOutput << YAML::Key << "Default Scene";
		ProjectOutput << YAML::Value << this->CurrentSceneName;

		ProjectOutput << YAML::EndMap;

		GString ProjectFileString = ProjectOutput.c_str();
		return ProjectFileString;
	}

	void GuardianProject::ReadSerializationData(const GString& serializationData)
	{
		YAML::Node ProjectData = YAML::Load(serializationData);

		this->ProjectName = ProjectData["Project"].as<GString>();
		this->ProjectFileDirectory = ProjectData["File Directory"].as<GString>();
		if (this->ProjectFileDirectory[this->ProjectFileDirectory.size() - 1] == '/' ||
			this->ProjectFileDirectory[this->ProjectFileDirectory.size() - 1] == '\\')
		{
			this->ProjectAssetPath = this->ProjectFileDirectory + "Assets\\";
		}
		else
		{
			this->ProjectAssetPath = this->ProjectFileDirectory + "\\Assets\\";
		}

		this->CurrentSceneName = ProjectData["Default Scene"].as<GString>();

		GuardianAssetSystem::InitializeAssetSystem(this->ProjectAssetPath);

		for (auto& scene : GuardianAssetSystem::GetSceneList())
		{
			this->ProjectSceneList[scene.first] = scene.second;
		}

		for (auto& scene : this->ProjectSceneList)
		{
			scene.second->InitializeScene();
		}
	}

	void GuardianProject::UpdateEditProject()
	{
		this->ProjectSceneList[this->CurrentSceneName]->UpdateEditScene(GuardianTime::GetDeltaTime());
	}

	void GuardianProject::UpdateRuntimeProject()
	{
		this->ProjectSceneList[this->CurrentSceneName]->UpdateRuntimeScene(GuardianTime::GetDeltaTime());
	}
}
