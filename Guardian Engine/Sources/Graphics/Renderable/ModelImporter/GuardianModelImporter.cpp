#include "GuardianModelImporter.h"
#include "../../Renderer/GuardianRenderer.h"

namespace guardian
{
	GuardianModelImporter::GuardianModelImporter()
	{
		this->ModelMeshInstanceDataList.clear();
	}

	GuardianModelImporter::GuardianModelImporter(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		this->ImportModel(graphics, modelFilePath);
	}

	GuardianModelImporter::GuardianModelImporter(const GuardianModelImporter& other)
	{
		this->ModelFileDirectory = other.ModelFileDirectory;
		this->ModelFilePath = other.ModelFilePath;
		this->ModelMeshInstanceDataList = other.ModelMeshInstanceDataList;
	}

	GuardianModelImporter::~GuardianModelImporter()
	{
		this->ClearModelMeshList();
	}

	void GuardianModelImporter::ImportModel(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		this->ModelFilePath = modelFilePath;
		this->ModelFileDirectory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
		if (this->ModelFileDirectory == modelFilePath)
		{
			this->ModelFileDirectory = modelFilePath.substr(0, modelFilePath.find_last_of("\\"));
		}

		Assimp::Importer Importer;
		const aiScene* ModelScene = Importer.ReadFile(
			modelFilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (!ModelScene || ModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ModelScene->mRootNode)
		{
			throw GUARDIAN_ERROR_EXCEPTION(Importer.GetErrorString());
		}

		this->ProcessModelNode(graphics, ModelScene->mRootNode, ModelScene);
	}

	void GuardianModelImporter::ClearModelMeshList()
	{
		for (auto& mesh : this->ModelMeshInstanceDataList)
		{
			mesh.VertexData.clear();
			mesh.IndexData.clear();
		}
		this->ModelMeshInstanceDataList.clear();
	}

	const GString& GuardianModelImporter::GetModelFilePath() const noexcept
	{
		return this->ModelFilePath;
	}

	const std::vector<GuardianMeshInstance::Data>& GuardianModelImporter::GetModelMeshInstanceDataList() const noexcept
	{
		return this->ModelMeshInstanceDataList;
	}

	void GuardianModelImporter::ProcessModelNode(std::shared_ptr<GuardianGraphics> graphics, aiNode* node, const aiScene* scene)
	{
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->ModelMeshInstanceDataList.push_back(this->ProcessModelMeshInstanceData(graphics, mesh, scene));
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessModelNode(graphics, node->mChildren[i], scene);
		}
	}

	GuardianMeshInstance::Data GuardianModelImporter::ProcessModelMeshInstanceData(std::shared_ptr<GuardianGraphics> graphics, aiMesh* mesh, const aiScene* scene)
	{
		GuardianMeshInstance::Data MeshInstanceData;
		std::vector<GuardianMeshInstance::Vertex> Vertices;
		std::vector<UINT> Indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			GuardianMeshInstance::Vertex Vertex;

			Vertex.Position.x = mesh->mVertices[i].x;
			Vertex.Position.y = mesh->mVertices[i].y;
			Vertex.Position.z = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				Vertex.TextureCoord.x = mesh->mTextureCoords[0][i].x;
				Vertex.TextureCoord.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				Vertex.TextureCoord = GVector2(0.0f, 0.0f);
			}

			Vertex.Normal.x = mesh->mNormals[i].x;
			Vertex.Normal.y = mesh->mNormals[i].y;
			Vertex.Normal.z = mesh->mNormals[i].z;

			Vertices.push_back(Vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace Face = mesh->mFaces[i];
			for (UINT j = 0; j < Face.mNumIndices; j++)
			{
				Indices.push_back(Face.mIndices[j]);
			}
		}

		MeshInstanceData.MeshInstanceName = mesh->mName.C_Str();

		MeshInstanceData.VertexData = Vertices;
		MeshInstanceData.IndexData = Indices;
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		GuardianUUID id;
		if (GuardianMaterialSystem::GetMaterialList().count(material->GetName().C_Str()) <= 0)
		{
			id = GuardianMaterialSystem::CreateNewMaterial(material->GetName().C_Str());

			std::shared_ptr<GuardianMaterial> mat = GuardianMaterialSystem::GetMaterial(id);
			mat->SetAlbedoTexture(this->LoadMaterialTexture(graphics, material, aiTextureType_DIFFUSE));
		}
		else
		{
			id = GuardianMaterialSystem::GetMaterialList()[material->GetName().C_Str()]->GetMaterialId();
		}

		MeshInstanceData.MaterialId = id;

		return MeshInstanceData;
	}

	std::shared_ptr<GuardianTexture> GuardianModelImporter::LoadMaterialTexture(
		std::shared_ptr<GuardianGraphics> graphics, aiMaterial* material, aiTextureType type)
	{
		std::shared_ptr<GuardianTexture> Texture = std::make_shared<GuardianTexture>();

		if (material->GetTextureCount(type) > 0)
		{
			aiString path;
			material->GetTexture(type, 0, &path);

			GString FilePath = this->ModelFileDirectory + "/" + path.C_Str();
			if (!std::filesystem::exists(FilePath))
			{
				FilePath = this->ModelFileDirectory + "\\" + path.C_Str();
			}
			Texture = GuardianTexture::CreateNewTexture(graphics, GuardianSurface(FilePath));
		}

		return Texture;
	}

	std::shared_ptr<GuardianModelImporter> GuardianModelImporter::CreateNewModel(
		std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		return std::make_shared<GuardianModelImporter>(graphics, modelFilePath);
	}
}
