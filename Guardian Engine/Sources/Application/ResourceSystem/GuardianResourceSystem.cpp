#include "GuardianResourceSystem.h"
#include "../GuardianApplication.h"

namespace guardian
{
	std::vector<GString> GuardianResourceSystem::LoadedResourcePath;
	std::map<GString, std::vector<GuardianMeshInstance::Data>> GuardianResourceSystem::MeshDataResourceList;
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
		std::vector<GuardianMeshInstance::Vertex> vertices;
		std::vector<UINT> indices;
		vertices.resize(24);

		vertices[0] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[1] = { {0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[2] = { {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
		vertices[3] = { {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };

		vertices[4] = { {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[5] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[6] = { {-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} };
		vertices[7] = { {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} };

		vertices[8] = { {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[9] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[10] = { {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };
		vertices[11] = { {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };

		vertices[12] = { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[13] = { {-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[14] = { {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} };
		vertices[15] = { {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} };

		vertices[16] = { {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[17] = { {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[18] = { {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
		vertices[19] = { {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };

		vertices[20] = { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[21] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[22] = { {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
		vertices[23] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };

		indices = {
			0, 1, 2,  2, 3, 0,
			4, 5, 6,  6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		auto& id = GuardianMaterialSystem::CreateNewMaterial("Default Material");

		GuardianMeshInstance::Data data{ "Box", vertices, indices, id };
		MeshDataResourceList["Box"].push_back(data);
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
			GuardianModelImporter model(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), path.string());

			auto& meshes = model.GetModelMeshInstanceDataList();
			GString name;
			if (model.GetModelFilePath().find_last_of('/') != GString::npos)
			{
				name = model.GetModelFilePath().substr(model.GetModelFilePath().find_last_of('/') + 1,
					model.GetModelFilePath().find('.'));
			}
			else
			{
				name = model.GetModelFilePath().substr(model.GetModelFilePath().find_last_of('\\') + 1,
					model.GetModelFilePath().find('.'));
			}
			for (auto& mesh : meshes)
			{
				GString MeshName = mesh.MeshInstanceName;
				int index = 1;
				GString Addition;
				while (MeshDataResourceList.count(MeshName + Addition) > 0)
				{
					Addition = "_" + std::to_string(index);
					index++;
				}
				MeshName += Addition;

				GuardianMeshInstance::Data m = mesh;
				m.MeshInstanceName = MeshName;
				MeshDataResourceList[name].push_back(m);
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

	const std::vector<GuardianMeshInstance::Data>& GuardianResourceSystem::GetMeshData(GString meshName)
	{
		if (MeshDataResourceList.count(meshName) > 0)
		{
			return MeshDataResourceList[meshName];
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

	std::map<GString, std::vector<GuardianMeshInstance::Data>> GuardianResourceSystem::GetMeshDataList()
	{
		return MeshDataResourceList;
	}

	std::map<GString, GuardianTexture> GuardianResourceSystem::GetTextureList()
	{
		return TextureResourceList;
	}
}
