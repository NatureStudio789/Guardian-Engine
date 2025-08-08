#include "GSceneRegistry.h"

namespace GE
{
	std::string GSceneRegistry::ActiveSceneName;
	std::map<std::string, std::shared_ptr<GScene>> GSceneRegistry::SceneList;


	void GSceneRegistry::RegisterScene(std::shared_ptr<GScene> scene)
	{
		if (SceneList.count(scene->GetSceneName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The scene named '{}' already exists in registry!", scene->GetSceneName()));
		}

		SceneList[scene->GetSceneName()] = scene;
	}

	void GSceneRegistry::SetActiveScene(const std::string& sceneName)
	{
		if (!SceneList.count(sceneName))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No scene named '{}' found in registry!", sceneName));
		}

		ActiveSceneName = sceneName;
	}

	std::shared_ptr<GScene> GSceneRegistry::GetActiveScene()
	{
		return SceneList[ActiveSceneName];
	}

	std::shared_ptr<GScene> GSceneRegistry::GetScene(const std::string& name)
	{
		if (!SceneList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No scene named '{}' found in registry!", name));
		}

		return SceneList[name];
	}

	std::shared_ptr<GScene> GSceneRegistry::GetScene(const GUUID& id)
	{
		for (auto& scene : SceneList)
		{
			if (scene.second->GetSceneId() == id)
			{
				return scene.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No scene with id : '{}' found in registry!", (unsigned long long)id));
	}
}
