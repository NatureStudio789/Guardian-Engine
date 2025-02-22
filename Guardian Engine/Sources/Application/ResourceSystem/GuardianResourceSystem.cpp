#include "GuardianResourceSystem.h"
#include "../GuardianApplication.h"

namespace guardian
{
	std::vector<GString> GuardianResourceSystem::LoadedResourcePath;
	std::map<GString, GuardianMesh> GuardianResourceSystem::MeshResourceList;
	std::map<GString, GuardianTexture> GuardianResourceSystem::TextureResourceList;
	std::map<GString, GuardianMaterial> GuardianResourceSystem::MaterialResourceList;
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
		std::vector<GuardianMesh::Vertex> vertices;
		std::vector<UINT> indices;
		vertices.resize(24);

		// 前面 (Z+)
		vertices[0] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[1] = { {0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[2] = { {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[3] = { {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };

		// 后面 (Z-)
		vertices[4] = { {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[5] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[6] = { {-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[7] = { {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} };

		// 右面 (X+)
		vertices[8] = { {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[9] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[10] = { {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[11] = { {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };

		// 左面 (X-)
		vertices[12] = { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[13] = { {-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[14] = { {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[15] = { {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} };

		// 顶面 (Y+)
		vertices[16] = { {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[17] = { {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[18] = { {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[19] = { {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };

		// 底面 (Y-)
		vertices[20] = { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[21] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[22] = { {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[23] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };

		// 索引数据（每个面两个三角形）
		indices = {
			// 前面
			0, 1, 2,  2, 3, 0,
			// 后面
			4, 5, 6,  6, 7, 4,
			// 右面
			8, 9, 10, 10, 11, 8,
			// 左面
			12, 13, 14, 14, 15, 12,
			// 顶面
			16, 17, 18, 18, 19, 16,
			// 底面
			20, 21, 22, 22, 23, 20
		};

		GuardianMesh box;
		box.InitializeMesh(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), vertices, indices);
		auto surface = GuardianSurface(1, 1);
		surface.ReplacePixel(0, 0, GuardianColor(255, 255, 255));
		std::shared_ptr<GuardianMaterial> material = std::make_shared<GuardianMaterial>();
		material->SetAlbedoTexture(GuardianTexture::CreateNewTexture(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), surface));
		box.SetMeshMaterial(material);

		MeshResourceList["Box"] = box;
		MaterialResourceList["Default Material"] = *material;
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
		
		if (extension == ".obj")
		{
			GuardianModel model(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), path.string());

			auto& meshes = model.GetModelMeshList();
			for (auto& mesh : meshes)
			{
				GString MeshName = mesh.first;
				int index = 1;
				GString Addition;
				while (MeshResourceList.count(MeshName + Addition) > 0)
				{
					Addition = "_" + std::to_string(index);
					index++;
				}
				MeshName += Addition;

				MeshResourceList[MeshName] = *mesh.second;
			}

			auto& materials = model.GetModelMaterialList();
			for (auto& material : materials)
			{
				GString MaterialName = material.first;
				int index = 1;
				GString Addition;
				while (MaterialResourceList.count(MaterialName + Addition) > 0)
				{
					Addition = "_" + std::to_string(index);
					index++;
				}
				MaterialName += Addition;

				MaterialResourceList[MaterialName] = *material.second;
			}

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

	GuardianMesh GuardianResourceSystem::GetMesh(GString meshName)
	{
		if (MeshResourceList.count(meshName) > 0)
		{
			return MeshResourceList[meshName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the mesh called : '" + meshName + "' !");
		}
	}

	const GString GuardianResourceSystem::GetMeshName(GuardianMesh mesh)
	{
		for (auto& imesh : MeshResourceList)
		{
			if (imesh.second == mesh)
			{
				return imesh.first;
			}
		}

		return "";
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

	GuardianMaterial GuardianResourceSystem::GetMaterial(GString materialName)
	{
		if (MaterialResourceList.count(materialName) > 0)
		{
			return MaterialResourceList[materialName];
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the mesh called : '" + materialName + "' !");
		}
	}

	GuardianMaterial GuardianResourceSystem::GetMaterial(const GuardianUUID& materialId)
	{
		for (auto& material : MaterialResourceList)
		{
			if (material.second.GetMaterialId() == materialId)
			{
				return material.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("Cannot find the material with id : '" + std::to_string(materialId) + "' !");
	}

	const GString GuardianResourceSystem::GetMaterialName(GuardianMaterial material)
	{
		for (auto& imaterial : MaterialResourceList)
		{
			if (imaterial.second == material)
			{
				return imaterial.first;
			}
		}

		return "";
	}

	std::map<GString, GuardianMesh> GuardianResourceSystem::GetMeshList()
	{
		return MeshResourceList;
	}

	std::map<GString, GuardianTexture> GuardianResourceSystem::GetTextureList()
	{
		return TextureResourceList;
	}

	std::map<GString, GuardianMaterial> GuardianResourceSystem::GetMaterialList()
	{
		return MaterialResourceList;
	}
}
