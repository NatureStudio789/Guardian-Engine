#include "GuardianAssetSystem.h"
#include "../GuardianApplication.h"

namespace GE
{
	std::vector<GString> GuardianAssetSystem::LoadedAssetPath;
	std::map<GString, std::shared_ptr<GuardianMeshAsset>> GuardianAssetSystem::MeshAssetsList;
	std::map<GString, GuardianTexture> GuardianAssetSystem::TextureAssetList;
	std::map<GString, GuardianPhysicsMaterial> GuardianAssetSystem::PhysicsMaterialResourceList;
	std::map<GString, std::shared_ptr<GuardianScene>> GuardianAssetSystem::SceneAssetList;

	std::map<GString, std::shared_ptr<GuardianMesh>> GuardianAssetSystem::MeshAssetRenderingList;


	void GuardianAssetSystem::InitializeAssetSystem(GString assetDirectoryPath)
	{
		AddDefaultAsset();

		IterateDirectory(assetDirectoryPath);

		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to initialize engine asset system.");
	}

	void GuardianAssetSystem::UpdateAssetSystem()
	{
		for (auto& meshAsset : MeshAssetsList)
		{
			MeshAssetRenderingList[meshAsset.first]->UpdateMeshTransform(GuardianTransform({0.0f, 0.0f, 0.0f}, 
				{0.0f, 45.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}).GetTransformMatrix());

			GuardianLightProperties Properties;
			Properties.CameraPosition = { 0.0f, 0.0f, -3.0f };
			Properties.LightNumber = 1;
			Properties.PointLightList[0].LightColor = { 1.0f, 1.0f, 1.0f };
			Properties.PointLightList[0].LightPosition = { 0.5f, 1.0f, -3.0f };
			Properties.PointLightList[0].LightStrength = 300.0f;
			MeshAssetRenderingList[meshAsset.first]->UpdateMeshLighting(Properties);

			MeshAssetRenderingList[meshAsset.first]->SubmitToRenderer(meshAsset.first + " Rendering View");
		}
	}

	void GuardianAssetSystem::AddDefaultAsset()
	{
		MeshAssetsList["Box"] = std::make_shared<GuardianMeshAsset>("../Guardian Engine/Assets/Models/Box/Box.fbx");

		GuardianRenderer::CreateRenderingRenderGraph(MeshAssetsList["Box"]->GetAssetName() + " Rendering View", 800, 800);
		GuardianRenderer::SetRenderingRenderGraphCamera(MeshAssetsList["Box"]->GetAssetName() + " Rendering View", 
			GuardianCamera({ 0.0f, 0.0f, -3.0f }, { 0.0f, 0.0f, 0.0f }, { 60.0f, 800.0f / 800.0f, 0.01f, 1000.0f }));
		
		GuardianRenderer::SetRenderingRenderGraphShaderGroup(MeshAssetsList["Box"]->GetAssetName() + " Rendering View",
			GuardianShaderSystem::PBR_MAIN_SHADER);

		MeshAssetRenderingList[MeshAssetsList["Box"]->GetAssetName()] = std::make_shared<GuardianMesh>();
		MeshAssetRenderingList[MeshAssetsList["Box"]->GetAssetName()]->InitializeMesh(
			MeshAssetsList["Box"]->GetAssetName(), MeshAssetsList[MeshAssetsList["Box"]->GetAssetName()]->GetMeshAssetData());
	}

	void GuardianAssetSystem::IterateDirectory(const std::filesystem::path& path)
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(path))
		{
			auto& directoryPath = directoryEntry.path();
			if (directoryEntry.is_regular_file())
			{
				LoadAsset(directoryPath);
			}
			else
			{
				IterateDirectory(directoryPath);
			}
		}
	}

	void GuardianAssetSystem::LoadAsset(const std::filesystem::path& path)
	{
		for (auto& loaded : LoadedAssetPath)
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

			GuardianRenderer::CreateRenderingRenderGraph(assetHandle->GetAssetName() + " Rendering View", 800, 800);
			GuardianRenderer::SetRenderingRenderGraphCamera(assetHandle->GetAssetName() + " Rendering View", 
				GuardianCamera({ 0.0f, 0.0f, -3.0f }, { 0.0f, 0.0f, 0.0f }, { 60.0f, 800.0f / 800.0f, 0.01f, 1000.0f }));

			GuardianRenderer::SetRenderingRenderGraphShaderGroup(assetHandle->GetAssetName() + " Rendering View",
				GuardianShaderSystem::PBR_MAIN_SHADER);

			MeshAssetRenderingList[assetHandle->GetAssetName()] = std::make_shared<GuardianMesh>();
			MeshAssetRenderingList[assetHandle->GetAssetName()]->InitializeMesh(
				assetHandle->GetAssetName(), MeshAssetsList[assetHandle->GetAssetName()]->GetMeshAssetData());

			LoadedAssetPath.push_back(path.string());
		}
		else if (extension == ".png" || extension == ".jpg")
		{
			GuardianTexture texture = GuardianTexture(GuardianSurface(path.string()));

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
			while (TextureAssetList.count(TextureName + Addition) > 0)
			{
				Addition = "_" + std::to_string(index);
				index++;
			}
			TextureName += Addition;

			TextureAssetList[TextureName] = texture;

			LoadedAssetPath.push_back(path.string());
		}
		else if (extension == ".gscene")
		{
			auto scene = std::make_shared<GuardianScene>();
			scene->LoadSceneAs(path.string());
			
			SceneAssetList[scene->GetFileName()] = scene;

			LoadedAssetPath.push_back(path.string());
		}
	}

	const GuardianMeshAsset& GuardianAssetSystem::GetMeshAsset(GString meshName)
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

	std::shared_ptr<GuardianFramebuffer> GuardianAssetSystem::GetMeshAssetRenderingView(GString meshName)
	{
		return GuardianRenderer::GetRenderingRenderGraphFramebuffer(meshName + " Rendering View");
	}

	std::shared_ptr<GuardianFramebuffer> GuardianAssetSystem::GetMeshAssetRenderingViewFromPath(GString meshFilePath)
	{
		return GuardianRenderer::GetRenderingRenderGraphFramebuffer(GetMeshAssetFromPath(meshFilePath).GetAssetName() + " Rendering View");
	}

	const GuardianMeshAsset& GuardianAssetSystem::GetMeshAssetFromPath(GString meshFilePath)
	{
		for (auto& meshAsset : MeshAssetsList)
		{
			if (meshAsset.second->GetAssetPath() == meshFilePath)
			{
				return *meshAsset.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("Cannot the mesh asset of '" + meshFilePath + "' !");
	}

	GuardianTexture GuardianAssetSystem::GetTexture(GString textureName)
	{
		if (TextureAssetList.count(textureName) > 0)
		{
			return TextureAssetList[textureName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the texture called : '" + textureName + "' !");
		}
	}

	const GString GuardianAssetSystem::GetTextureName(GuardianTexture texture)
	{
		for (auto& itexture : TextureAssetList)
		{
			if (itexture.second == texture)
			{
				return itexture.first;
			}
		}

		return "Unknown";
	}

	std::shared_ptr<GuardianScene> GuardianAssetSystem::GetScene(GString sceneName)
	{
		if (SceneAssetList.count(sceneName) > 0)
		{
			return SceneAssetList[sceneName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the scene called : '" + sceneName + "' !");
		}
	}

	std::map<GString, std::shared_ptr<GuardianMeshAsset>> GuardianAssetSystem::GetMeshAssetList()
	{
		return MeshAssetsList;
	}

	std::map<GString, GuardianTexture> GuardianAssetSystem::GetTextureList()
	{
		return TextureAssetList;
	}

	std::map<GString, std::shared_ptr<GuardianScene>> GuardianAssetSystem::GetSceneList()
	{
		return SceneAssetList;
	}
}
