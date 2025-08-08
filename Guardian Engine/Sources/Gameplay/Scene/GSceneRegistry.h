#ifndef _GE_GSCENEREGISTRY_H_
#define _GE_GSCENEREGISTRY_H_
#include "GScene.h"

namespace GE
{
	class GUARDIAN_API GSceneRegistry
	{
	public:
		static void RegisterScene(std::shared_ptr<GScene> scene);
		static void SetActiveScene(const std::string& sceneName);

		static std::shared_ptr<GScene> GetActiveScene();
		static std::shared_ptr<GScene> GetScene(const std::string& name);
		static std::shared_ptr<GScene> GetScene(const GUUID& id);

	private:
		static std::string ActiveSceneName;
		static std::map<std::string, std::shared_ptr<GScene>> SceneList;
	};
}

#endif