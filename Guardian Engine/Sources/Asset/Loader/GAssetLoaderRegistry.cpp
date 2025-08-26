#include "GAssetLoaderRegistry.h"

namespace GE
{
	std::string GAssetLoaderRegistry::CurrentAssetLoaderName;
	std::map<std::string, std::shared_ptr<GAssetLoader>> GAssetLoaderRegistry::AssetLoaderList;


	void GAssetLoaderRegistry::RegistryAssetLoader(std::shared_ptr<GAssetLoader> assetLoader)
	{
		if (AssetLoaderList.count(assetLoader->GetAssetLoaderName()) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The asset loader named '{}' already exists in registry!", assetLoader->GetAssetLoaderName()));
		}

		AssetLoaderList[assetLoader->GetAssetLoaderName()] = assetLoader;
	}

	void GAssetLoaderRegistry::SetCurrentAssetLoader(const std::string& name)
	{
		if (AssetLoaderList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Invalid asset loader!");
		}

		CurrentAssetLoaderName = name;
	}

	std::shared_ptr<GAssetLoader> GAssetLoaderRegistry::GetAssetLoader(const std::string& name)
	{
		if (AssetLoaderList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No asset loader named '{}' found in registry!"));
		}

		return AssetLoaderList[name];
	}

	std::shared_ptr<GAssetLoader> GAssetLoaderRegistry::GetAssetLoader(const GUUID& id)
	{
		for (auto& loader : AssetLoaderList)
		{
			if (loader.second->GetAssetLoaderId() == id)
			{
				return loader.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No asset loader with id : '{}' found in registry!", (unsigned long long)id));
	}

	std::shared_ptr<GAssetLoader> GAssetLoaderRegistry::GetCurrentAssetLoader()
	{
		if (AssetLoaderList.count(CurrentAssetLoaderName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Current asset loader is invalid!");
		}

		return AssetLoaderList[CurrentAssetLoaderName];
	}
}
