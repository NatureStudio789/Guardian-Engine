#ifndef _GE_GASSET_H_
#define _GE_GASSET_H_
#include "../Project/GProject.h"

namespace GE
{
	class GUARDIAN_API GAsset
	{
	public:
		enum Category
		{
			GE_ASSET_UNKNOWN = 0,
			GE_ASSET_TEXTURE,
			GE_ASSET_STATIC_MODEL,
			GE_ASSET_SCENE
		};

	public:
		GAsset();
		GAsset(const std::string& assetFilePath);
		GAsset(const GAsset& other);
		virtual ~GAsset();

		void InitializeAsset(const std::string& assetFilePath);
		void LoadAsset(const std::string& sourceFilePath);

		const GUUID& GetAssetId() const noexcept;
		const std::string& GetAssetName() const noexcept;
		const std::string& GetAssetPackage() const noexcept;
		const std::string GetAssetPath() const noexcept;
		const std::string GetAssetSourcePath() const noexcept;
		const Category& GetAssetCategory() const noexcept;

		template<typename T>
		T GetAssetData();

		static std::shared_ptr<GAsset> CreateNewAsset(const std::string& assetFilePath)
		{
			return std::make_shared<GAsset>(assetFilePath);
		}

	private:
		static Category GetAssetCategoryFromExtension(const std::string& extension);
		static std::string GetCategoryName(Category category);
		static Category GetCategoryFromName(std::string name);

		GUUID AssetId;
		std::string AssetName;
		std::string AssetPackage;
		std::string AssetSourcePath;
		Category AssetCategory;

		std::any AssetData;

		friend class GAssetSerializer;
		friend class GAssetLoader;
	};

	template<typename T>
	inline T GAsset::GetAssetData()
	{
		if (this->AssetData.type() != typeid(T))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The asse is not the type of '{}'.", typeid(T).name()));
		}

		return std::any_cast<T>(this->AssetData);
	}
}

#endif