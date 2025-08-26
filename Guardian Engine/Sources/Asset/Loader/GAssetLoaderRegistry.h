#ifndef _GE_GASSETLOADERREGISTRY_H_
#define _GE_GASSETLOADERREGISTRY_H_
#include "GAssetLoader.h"

namespace GE
{
	class GUARDIAN_API GAssetLoaderRegistry
	{
	public:
		static void RegistryAssetLoader(std::shared_ptr<GAssetLoader> assetLoader);
		static void SetCurrentAssetLoader(const std::string& name);

		static std::shared_ptr<GAssetLoader> GetAssetLoader(const std::string& name);
		static std::shared_ptr<GAssetLoader> GetAssetLoader(const GUUID& id);
		static std::shared_ptr<GAssetLoader> GetCurrentAssetLoader();

	private:
		static std::string CurrentAssetLoaderName;
		static std::map<std::string, std::shared_ptr<GAssetLoader>> AssetLoaderList;
	};
}

#endif