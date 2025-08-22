#ifndef _GE_GASSETLOADER_H_
#define _GE_GASSETLOADER_H_
#include "../GAsset.h"

namespace GE
{
	class GUARDIAN_API GAssetLoader
	{
	public:
		GAssetLoader();
		GAssetLoader(const std::string& assetDirectory);
		GAssetLoader(const GAssetLoader& other);
		~GAssetLoader();

		void InitializeAssetLoader(const std::string& assetDirectory);
		void LoadAllAsset();
		void ResetAssetDirectory(const std::string& assetDirectory);
		void ReloadAllAsset();

		void UpdateAssetLoader();

		std::shared_ptr<GAsset> GetAsset(const GUUID& assetId);
		std::shared_ptr<GAsset> GetAsset(const std::string& assetName);

		const std::string& GetAssetDirectory() const noexcept;
		const std::vector<std::string> GetUnloadedSourceList() const noexcept;
		const std::map<std::string, std::shared_ptr<GAsset>>& GetLoadedAssetList() const noexcept;

	private:
		void LoadAsset(const std::string& filePath);
		void IterateAndLoadAsset(const std::string& filePath);
		bool CheckIsAsset(const std::string& filePath);

		std::string AssetDirectory;

		std::vector<std::string> AssetSourcePathList;
		std::map<std::string, std::shared_ptr<GAsset>> LoadedAssetList;
	};
}

#endif	