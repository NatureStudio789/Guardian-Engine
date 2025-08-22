#include "GAssetLoader.h"

namespace GE
{
	GAssetLoader::GAssetLoader()
	{
		this->AssetDirectory = "";
		this->AssetSourcePathList.clear();
		this->LoadedAssetList.clear();
	}

	GAssetLoader::GAssetLoader(const std::string& assetDirectory)
	{
		this->InitializeAssetLoader(assetDirectory);
	}

	GAssetLoader::GAssetLoader(const GAssetLoader& other)
	{
		this->AssetDirectory = other.AssetDirectory;
		this->AssetSourcePathList = other.AssetSourcePathList;
		this->LoadedAssetList = other.LoadedAssetList;
	}

	GAssetLoader::~GAssetLoader()
	{
		this->AssetDirectory.clear();
		this->AssetSourcePathList.clear();

		for (auto& asset : this->LoadedAssetList)
		{
			asset.second.reset();
			asset.second = null;
		}
		this->LoadedAssetList.clear();
	}

	void GAssetLoader::InitializeAssetLoader(const std::string& assetDirectory)
	{
		this->AssetDirectory = assetDirectory;

		LoadAllAsset();
	}

	void GAssetLoader::LoadAllAsset()
	{
		if (!GUtil::FileExists(this->AssetDirectory))
		{
			std::filesystem::create_directory(this->AssetDirectory);
		}

		IterateAndLoadAsset(this->AssetDirectory);
	}

	void GAssetLoader::ResetAssetDirectory(const std::string& assetDirectory)
	{
		this->AssetDirectory = assetDirectory;

		this->ReloadAllAsset();
	}

	void GAssetLoader::ReloadAllAsset()
	{
		this->AssetSourcePathList.clear();
		for (auto& asset : this->LoadedAssetList)
		{
			asset.second.reset();
			asset.second = null;
		}
		this->LoadedAssetList.clear();

		this->LoadAllAsset();
	}

	void GAssetLoader::UpdateAssetLoader()
	{

	}

	std::shared_ptr<GAsset> GAssetLoader::GetAsset(const GUUID& assetId)
	{
		for (auto& asset : this->LoadedAssetList)
		{
			if (asset.second->GetAssetId() == assetId)
			{
				return asset.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No asset with id : '{}' found in the loader!", (unsigned long long)assetId));
	}

	std::shared_ptr<GAsset> GAssetLoader::GetAsset(const std::string& assetName)
	{
		if (this->LoadedAssetList.count(assetName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No asset named '{}' found in the loader!", assetName));
		}

		return this->LoadedAssetList[assetName];
	}

	const std::string& GAssetLoader::GetAssetDirectory() const noexcept
	{
		return this->AssetDirectory;
	}

	const std::vector<std::string> GAssetLoader::GetUnloadedSourceList() const noexcept
	{
		std::vector<std::string> UnloadedSourceList;

		struct DoubleChecker
		{
			int First, Second;
		};

		std::map<std::string, DoubleChecker> LoadedChecker;
		for (const auto& sourcePath : this->AssetSourcePathList)
		{
			LoadedChecker[sourcePath].First = (int)'t' + (int)'e' + (int)'s' + (int)'t';
		}

		for (const auto& asset : this->LoadedAssetList)
		{
			LoadedChecker[asset.second->GetAssetSourcePath()].Second = (int)'t' + (int)'e' + (int)'s' + (int)'t';
		}

		for (const auto& checker : LoadedChecker)
		{
			if (checker.second.First == ((int)'t' + (int)'e' + (int)'s' + (int)'t') && 
				checker.second.Second == ((int)'t' + (int)'e' + (int)'s' + (int)'t'))
			{
				continue;
			}

			UnloadedSourceList.push_back(checker.first);
		}

		return UnloadedSourceList;
	}

	const std::map<std::string, std::shared_ptr<GAsset>>& GAssetLoader::GetLoadedAssetList() const noexcept
	{
		return this->LoadedAssetList;
	}

	void GAssetLoader::LoadAsset(const std::string& filePath)
	{
		if (GUtil::GetFileExtension(filePath) == ".gasset")
		{
			std::shared_ptr<GAsset> asset = null;
			asset = GAsset::CreateNewAsset(filePath);

			this->LoadedAssetList[asset->GetAssetName()] = asset;
		}
		else
		{
			this->AssetSourcePathList.push_back(filePath);
		}
	}

	void GAssetLoader::IterateAndLoadAsset(const std::string& filePath)
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(filePath))
		{
			auto& path = directoryEntry.path();
			if (directoryEntry.is_regular_file() && CheckIsAsset(path.string()))
			{
				this->LoadAsset(path.string());
			}
			else
			{
				IterateAndLoadAsset(path.string());
			}
		}
	}

	bool GAssetLoader::CheckIsAsset(const std::string& filePath)
	{
		const auto& category = GAsset::GetAssetCategoryFromExtension(GUtil::GetFileExtension(filePath));
		return category != GAsset::GE_ASSET_UNKNOWN;
	}
}
