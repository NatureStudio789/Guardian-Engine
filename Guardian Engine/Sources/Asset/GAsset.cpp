#include "GAsset.h"
#include "../Gameplay/Serializer/GSceneSerializer.h"

namespace GE
{
	GAsset::GAsset()
	{
		this->AssetId = 0;
		this->AssetName = "";
		this->AssetPackage = "";
		this->AssetCategory = GE_ASSET_UNKNOWN;
	}

	GAsset::GAsset(const std::string& assetFilePath)
	{
		this->InitializeAsset(assetFilePath);
	}

	GAsset::GAsset(const GAsset& other)
	{
		this->AssetId = other.AssetId;
		this->AssetName = other.AssetName;
		this->AssetPackage = other.AssetPackage;
		this->AssetCategory = other.AssetCategory;

		this->AssetData = other.AssetData;
	}

	GAsset::~GAsset()
	{

	}

	void GAsset::InitializeAsset(const std::string& assetFilePath)
	{
		GAssetSerializer::Import(assetFilePath, this);

		switch (this->AssetCategory)
		{
			case GE_ASSET_TEXTURE:
			{
				std::shared_ptr<GSurface> TextureData = std::make_shared<GSurface>(this->AssetSourcePath);
				this->AssetData = TextureData;

				break;
			}

			case GE_ASSET_STATIC_MODEL:
			{
				GModel::Data ModelData = GModel::Load(this->AssetSourcePath);
				this->AssetData = ModelData;

				break;
			}

			case GE_ASSET_SCENE:
			{
				YAML::Node SceneData = GSceneSerializer::Load(this->AssetSourcePath);
				this->AssetData = SceneData;

				break;
			}

			default:
			{
				throw GUARDIAN_ERROR_EXCEPTION("Unknown asset category!");
				break;
			}
		}
	}

	void GAsset::LoadAsset(const std::string& sourceFilePath)
	{
		this->AssetId = GUUID();
		this->AssetSourcePath = sourceFilePath;
		this->AssetPackage = GUtil::GetFilePathDirectory(this->AssetSourcePath);
		this->AssetName = GUtil::GetFileName(this->AssetSourcePath);

		this->AssetCategory = this->GetAssetCategoryFromExtension(GUtil::GetFileExtension(this->AssetSourcePath));
		if (this->AssetCategory == GE_ASSET_UNKNOWN)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Unknown asset category!");
		}

		switch (this->AssetCategory)
		{
			case GE_ASSET_TEXTURE:
			{
				std::shared_ptr<GSurface> TextureData = std::make_shared<GSurface>(this->AssetSourcePath);
				this->AssetData = TextureData;

				break;
			}

			case GE_ASSET_STATIC_MODEL:
			{
				GModel::Data ModelData = GModel::Load(this->AssetSourcePath);
				this->AssetData = ModelData;

				break;
			}

			case GE_ASSET_SCENE:
			{
				YAML::Node SceneData = GSceneSerializer::Load(this->AssetSourcePath);
				this->AssetData = SceneData;

				break;
			}

			default:
			{
				throw GUARDIAN_ERROR_EXCEPTION("Unknown asset category!");
				break;
			}
		}

		GAssetSerializer::Export(this->GetAssetPath(), this);
	}
		
	void GAsset::LoadAssetData(const std::string& sourceFilePath)
	{
		this->AssetSourcePath = sourceFilePath;

		if (this->AssetCategory != this->GetAssetCategoryFromExtension(GUtil::GetFileExtension(sourceFilePath)))
		{
			throw GUARDIAN_ERROR_EXCEPTION("Incorrect asset source file!");
		}

		switch (this->AssetCategory)
		{
			case GE_ASSET_TEXTURE:
			{
				std::shared_ptr<GSurface> TextureData = std::make_shared<GSurface>(this->AssetSourcePath);
				this->AssetData = TextureData;

				break;
			}

			case GE_ASSET_STATIC_MODEL:
			{
				GModel::Data ModelData = GModel::Load(this->AssetSourcePath);
				this->AssetData = ModelData;

				break;
			}

			case GE_ASSET_SCENE:
			{
				YAML::Node SceneData = GSceneSerializer::Load(this->AssetSourcePath);
				this->AssetData = SceneData;

				break;
			}

			default:
			{
				throw GUARDIAN_ERROR_EXCEPTION("Unknown asset category!");
				break;
			}
		}

		GAssetSerializer::Export(this->GetAssetPath(), this);
	}

	void GAsset::LoadAssetData(const std::string& sourceFilePath, char* data, unsigned long long dataSize)
	{
		this->AssetSourcePath = sourceFilePath;

		switch (this->AssetCategory)
		{
			case GE_ASSET_TEXTURE:
			{
				std::shared_ptr<GSurface> TextureData = std::make_shared<GSurface>((unsigned char*)data, dataSize);
				this->AssetData = TextureData;

				break;
			}

			case GE_ASSET_STATIC_MODEL:
			{
				GModel::Data ModelData = GModel::Load(this->AssetSourcePath, data, dataSize);
				this->AssetData = ModelData;

				break;
			}

			case GE_ASSET_SCENE:
			{
				std::stringstream SceneFileStringStream;
				SceneFileStringStream << data;
				YAML::Node SceneData = YAML::Load(SceneFileStringStream.str());
				this->AssetData = SceneData;

				break;
			}

			default:
			{
				throw GUARDIAN_ERROR_EXCEPTION("Unknown asset category!");
				break;
			}
		}
	}

	const GUUID& GAsset::GetAssetId() const noexcept
	{
		return this->AssetId;
	}

	const std::string& GAsset::GetAssetName() const noexcept
	{
		return this->AssetName;
	}

	const std::string& GAsset::GetAssetPackage() const noexcept
	{
		return this->AssetPackage;
	}

	const std::string GAsset::GetAssetPath() const noexcept
	{
		return GUtil::ExtendDirectory(this->AssetPackage, this->AssetName + ".gasset");
	}

	const std::string GAsset::GetAssetSourcePath() const noexcept
	{
		return this->AssetSourcePath;
	}

	const GAsset::Category& GAsset::GetAssetCategory() const noexcept
	{
		return this->AssetCategory;
	}

	GAsset::Category GAsset::GetAssetCategoryFromExtension(const std::string& extension)
	{
		if (extension == ".png" || extension == ".jpg" || extension == ".bmp" || 
			extension == ".dds" || extension == ".jpeg" || extension == "gif")
		{
			return GE_ASSET_TEXTURE;
		}
		else if (extension == ".fbx" || extension == ".obj")
		{
			return GE_ASSET_STATIC_MODEL;
		}
		else if (extension == ".gscene")
		{
			return GE_ASSET_SCENE;
		}
		else
		{
			return GE_ASSET_UNKNOWN;
		}
	}

	std::string GAsset::GetCategoryName(Category category)
	{
		switch (category)
		{
			case GE_ASSET_TEXTURE:
			{
				return "GAsset.Texture";
				break;
			}

			case GE_ASSET_STATIC_MODEL:
			{
				return "GAsset.StaticModel";
				break;
			}

			case GE_ASSET_SCENE:
			{
				return "GAsset.Scene";
				break;
			}

			default:
			{
				return "GUnknown.Invalid";
				break;
			}
		}
	}

	GAsset::Category GAsset::GetCategoryFromName(std::string name)
	{
		if (name == "GAsset.Texture")
		{
			return GE_ASSET_TEXTURE;
		}
		else if (name == "GAsset.StaticModel")
		{
			return GE_ASSET_STATIC_MODEL;
		}
		else if (name == "GAsset.Scene")
		{
			return GE_ASSET_SCENE;
		}
		else
		{
			return GE_ASSET_UNKNOWN;
		}
	}
}
