#include "GuardianResourceSystem.h"
#include "../GuardianApplication.h"

namespace guardian
{
	std::vector<GString> GuardianResourceSystem::LoadedResourcePath;
	std::map<GString, std::shared_ptr<GuardianMeshAsset>> GuardianResourceSystem::MeshAssetsList;
	std::map<GString, GuardianTexture> GuardianResourceSystem::TextureResourceList;
	std::map<GString, GuardianPhysicsMaterial> GuardianResourceSystem::PhysicsMaterialResourceList;


	void GuardianResourceSystem::InitializeResourceSystem()
	{
		AddDefaultResource();

		UpdateResourceSystem();
	}

	void GuardianResourceSystem::UpdateResourceSystem()
	{
		IterateDirectory("Resources\\");
	}

	void GuardianResourceSystem::AddDefaultResource()
	{
		MeshAssetsList["Box"] = std::make_shared<GuardianMeshAsset>("../Guardian Engine/Resources/Models/Box/Box.fbx");
	}

	void GuardianResourceSystem::IterateDirectory(const std::filesystem::path& path)
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(path))
		{
			auto& directoryPath = directoryEntry.path();
			if (directoryEntry.is_regular_file())
			{
				LoadResource(directoryPath);
			}
			else
			{
				IterateDirectory(directoryPath);
			}
		}
	}

	void GuardianResourceSystem::LoadResource(const std::filesystem::path& path)
	{
		for (auto& loaded : LoadedResourcePath)
		{
			if (path.string() == loaded)
			{
				return;
			}
		}

		GString extension = path.extension().string();
		
		if (extension == ".obj" || extension == ".fbx")
		{
			auto& assetHandle = std::make_shared<GuardianMeshAsset>(path.string());
			MeshAssetsList[assetHandle->GetAssetName()] = assetHandle;

			LoadedResourcePath.push_back(path.string());
		}
		else if (extension == ".png" || extension == ".jpg")
		{
			GuardianTexture texture = GuardianTexture(
				GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), GuardianSurface(path.string()));

			GString TexturePath = path.string();
			GString TextureName;
			if (TexturePath.find_last_of('/') != GString::npos)
			{
				TextureName = TexturePath.substr(TexturePath.find_last_of('/') + 1);
			}
			else
			{
				TextureName = TexturePath.substr(TexturePath.find_last_of('\\') + 1);
			}
			int index = 1;
			GString Addition;
			while (TextureResourceList.count(TextureName + Addition) > 0)
			{
				Addition = "_" + std::to_string(index);
				index++;
			}
			TextureName += Addition;

			TextureResourceList[TextureName] = texture;

			LoadedResourcePath.push_back(path.string());
		}
	}

	const GuardianMeshAsset& GuardianResourceSystem::GetMeshAsset(GString meshName)
	{
		if (MeshAssetsList.count(meshName) > 0)
		{
			return *MeshAssetsList[meshName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the mesh called : '" + meshName + "' !");
		}
	}

	GuardianTexture GuardianResourceSystem::GetTexture(GString textureName)
	{
		if (TextureResourceList.count(textureName) > 0)
		{
			return TextureResourceList[textureName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the texture called : '" + textureName + "' !");
		}
	}

	const GString GuardianResourceSystem::GetTextureName(GuardianTexture texture)
	{
		for (auto& itexture : TextureResourceList)
		{
			if (itexture.second == texture)
			{
				return itexture.first;
			}
		}

		return "Unknown";
	}

	std::map<GString, std::shared_ptr<GuardianMeshAsset>> GuardianResourceSystem::GetMeshAssetList()
	{
		return MeshAssetsList;
	}

	std::map<GString, GuardianTexture> GuardianResourceSystem::GetTextureList()
	{
		return TextureResourceList;
	}
}
