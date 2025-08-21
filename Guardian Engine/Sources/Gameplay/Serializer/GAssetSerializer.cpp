#include "GAssetSerializer.h"

namespace GE
{
	void GAssetSerializer::Serialize(std::shared_ptr<GAsset> asset, YAML::Emitter& serializingData)
	{
		auto& SerializingData = serializingData;

		SerializingData << YAML::BeginMap;
		SerializingData << YAML::Key << "Asset";
		SerializingData << YAML::Value << (unsigned long long)asset->GetAssetId();
		SerializingData << YAML::Key << "Name";
		SerializingData << YAML::Value << asset->GetAssetName();
		SerializingData << YAML::Key << "Category";
		SerializingData << YAML::Value << GAsset::GetCategoryName(asset->GetAssetCategory());

		SerializingData << YAML::Key << "File Data";

		SerializingData << YAML::BeginMap;

		SerializingData << YAML::Key << "Package";
		SerializingData << YAML::Value << asset->GetAssetPackage();
		SerializingData << YAML::Key << "Source File";
		SerializingData << YAML::Value << asset->GetAssetSourcePath();

		SerializingData << YAML::EndMap;

		SerializingData << YAML::EndMap;
	}

	void GAssetSerializer::Serialize(GAsset* asset, YAML::Emitter& serializingData)
	{
		auto& SerializingData = serializingData;

		SerializingData << YAML::BeginMap;
		SerializingData << YAML::Key << "Asset";
		SerializingData << YAML::Value << (unsigned long long)asset->GetAssetId();
		SerializingData << YAML::Key << "Name";
		SerializingData << YAML::Value << asset->GetAssetName();
		SerializingData << YAML::Key << "Category";
		SerializingData << YAML::Value << GAsset::GetCategoryName(asset->GetAssetCategory());

		SerializingData << YAML::Key << "File Data";

		SerializingData << YAML::BeginMap;

		SerializingData << YAML::Key << "Package";
		SerializingData << YAML::Value << asset->GetAssetPackage();
		SerializingData << YAML::Key << "Source File";
		SerializingData << YAML::Value << asset->GetAssetSourcePath();

		SerializingData << YAML::EndMap;

		SerializingData << YAML::EndMap;
	}

	void GAssetSerializer::Deserialize(std::shared_ptr<GAsset> asset, YAML::Node& deserializingData)
	{
		asset->AssetId = deserializingData["Asset"].as<unsigned long long>();
		asset->AssetName = deserializingData["Name"].as<std::string>();
		asset->AssetCategory = GAsset::GetCategoryFromName(deserializingData["Category"].as<std::string>());
		if (asset->AssetCategory == GAsset::GE_ASSET_UNKNOWN)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Unknown Asset Category!");
		}

		auto FileData = deserializingData["File Data"];
		asset->AssetPackage = FileData["Package"].as<std::string>();
		asset->AssetSourcePath = FileData["Source File"].as<std::string>();
	}

	void GAssetSerializer::Deserialize(GAsset* asset, YAML::Node& deserializingData)
	{
		asset->AssetId = deserializingData["Asset"].as<unsigned long long>();
		asset->AssetName = deserializingData["Name"].as<std::string>();
		asset->AssetCategory = GAsset::GetCategoryFromName(deserializingData["Category"].as<std::string>());
		if (asset->AssetCategory == GAsset::GE_ASSET_UNKNOWN)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Unknown Asset Category!");
		}

		auto FileData = deserializingData["File Data"];
		asset->AssetPackage = FileData["Package"].as<std::string>();
		asset->AssetSourcePath = FileData["Source File"].as<std::string>();
	}

	void GAssetSerializer::Export(const std::string& filePath, std::shared_ptr<GAsset> asset)
	{
		YAML::Emitter SerializingData;
		Serialize(asset, SerializingData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializingData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	void GAssetSerializer::Export(const std::string& filePath, GAsset* asset)
	{
		YAML::Emitter SerializingData;
		Serialize(asset, SerializingData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializingData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	void GAssetSerializer::Import(const std::string& filePath, std::shared_ptr<GAsset> asset)
	{
		Deserialize(asset, Load(filePath));
	}

	void GAssetSerializer::Import(const std::string& filePath, GAsset* asset)
	{
		Deserialize(asset, Load(filePath));
	}

	YAML::Node GAssetSerializer::Load(const std::string& filePath)
	{
		std::ifstream InputFileStream(filePath);
		if (!InputFileStream.is_open())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The scene file is invalid!");
		}

		std::stringstream InputStringStream;
		InputStringStream << InputFileStream.rdbuf();

		std::string SceneFileString = InputStringStream.str();

		YAML::Node SceneData = YAML::Load(SceneFileString);

		return SceneData;
	}
}
