#include "GuardianAsset.h"

namespace GE
{
	GuardianAsset::GuardianAsset()
	{
		this->AssetPath = "";
		this->AssetName = "";
	}

	GuardianAsset::GuardianAsset(const GuardianAsset& other)
	{
		this->AssetPath = other.AssetPath;
		this->AssetName = other.AssetName;
	}

	void GuardianAsset::SetAssetPath(const GString& filePath)
	{
		this->AssetPath = filePath;

		GString FileName = this->AssetPath;
		if (this->AssetPath.find_last_of('/') != std::string::npos)
		{
			FileName = this->AssetPath.substr(this->AssetPath.find_last_of('/') + 1);
		}
		else if (this->AssetPath.find_last_of('\\') != std::string::npos)
		{
			FileName = this->AssetPath.substr(this->AssetPath.find_last_of('\\') + 1);
		}

		this->AssetName = FileName.substr(0, FileName.find_last_of('.'));
	}

	const bool GuardianAsset::IsAvailable() const noexcept
	{
		return std::filesystem::exists(this->AssetPath);
	}

	const GString& GuardianAsset::GetAssetPath() const noexcept
	{
		return this->AssetPath;
	}

	const GString& GuardianAsset::GetAssetName() const noexcept
	{
		return this->AssetName;
	}
}
