#include "GProjectPacker.h"
#include "../GProject.h"
#include "../../Asset/Loader/GAssetLoader.h"

namespace GE
{
	void GProjectPacker::PackProject(std::shared_ptr<GProject> project, const std::string& packDirectory)
	{
		inicpp::IniManager RuntimeConfig(GUtil::ExtendDirectory(packDirectory, "Config/RuntimeConfig.ini"));
		RuntimeConfig.set("Core", "Name", project->ProjectName);

		GAssetPacker::PackAsset(project->ProjectAssetLoader, 
			GUtil::ExtendDirectory(packDirectory, GUtil::ExtendDirectory(project->ProjectName, project->ProjectName + "-Asset.gepack")));

		RuntimeConfig.set("Gameplay", "StartupScene", std::to_string((unsigned long long)project->GetStartupSceneAssetId()));
		RuntimeConfig.set("Gameplay", "AssetPackage", project->ProjectAssetLoader->GetAssetDirectory());

		// TODO: Move executable files and library files to directory by command.
	}
}
