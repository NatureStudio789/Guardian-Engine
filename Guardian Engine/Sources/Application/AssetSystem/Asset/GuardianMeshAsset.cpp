#include "GuardianMeshAsset.h"
#include "../../../Application/GuardianApplication.h"
#include "../../../Graphics/Renderable/Mesh/GuardianMeshInstance.h"
#include "../../../Graphics/Renderable/ModelImporter/GuardianModelImporter.h"

namespace guardian
{
	GuardianMeshAsset::GuardianMeshAsset() :
		GuardianAsset()
	{
		this->MeshAssetData.clear();
	}

	GuardianMeshAsset::GuardianMeshAsset(const GString& filePath)
	{
		this->InitializeMeshAsset(filePath);
	}

	GuardianMeshAsset::GuardianMeshAsset(const GuardianMeshAsset& other) : 
		GuardianAsset(other)
	{
		this->MeshAssetData = other.MeshAssetData;
	}

	void GuardianMeshAsset::InitializeMeshAsset(const GString& filePath)
	{
		this->SetAssetPath(filePath);

		GuardianModelImporter importer = GuardianModelImporter(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), filePath);
		this->MeshAssetData = importer.GetModelMeshInstanceDataList();
	}

	const bool GuardianMeshAsset::IsAvailable() const noexcept
	{
		return std::filesystem::exists(this->AssetPath) && !this->MeshAssetData.empty();
	}

	const std::vector<GuardianMeshInstance::Data>& GuardianMeshAsset::GetMeshAssetData() const noexcept
	{
		return this->MeshAssetData;
	}
}
