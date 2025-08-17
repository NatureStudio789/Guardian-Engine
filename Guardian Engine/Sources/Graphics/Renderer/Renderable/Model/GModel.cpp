#include "GModel.h"

namespace GE
{
	GModel::GModel()
	{
		this->RootMeshNode = null;
		this->ModelMeshList.clear();

		this->ModelAccumulatedMatrix = GMatrix::IdentityMatrix();
	}

	GModel::GModel(const std::string& filePath, std::string renderGraphName)
	{
		this->InitializeModel(filePath, renderGraphName);
	}

	GModel::GModel(const GModel& other)
	{
		this->RootMeshNode = other.RootMeshNode;
		this->ModelMeshList = other.ModelMeshList;

		this->ModelAccumulatedMatrix = other.ModelAccumulatedMatrix;

		this->ModelFilePath = other.ModelFilePath;
		this->ModelFileDirectory = other.ModelFileDirectory;
	}

	GModel::~GModel()
	{
		this->RootMeshNode.reset();
		this->RootMeshNode = null;

		this->ModelMeshList.clear();
	}

	void GModel::InitializeModel(const std::string& filePath, std::string renderGraphName)
	{
		this->ModelFilePath = filePath;
		this->ModelFileDirectory = GUtil::GetFilePathDirectory(this->ModelFilePath);

		Assimp::Importer Importer;
		const aiScene* Scene = Importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			throw GUARDIAN_ERROR_EXCEPTION(Importer.GetErrorString());
		}

		for (UINT i = 0; i < Scene->mNumMeshes; i++)
		{
			this->ModelMeshList.push_back(this->ParseMesh(Scene, Scene->mMeshes[i]));
		}

		this->RootMeshNode = this->ParseNode(Scene, Scene->mRootNode);

		this->LinkTechnique(renderGraphName);
	}

	void GModel::SetTransform(const GTransform& transform)
	{
		this->RootMeshNode->SetTransform(transform);
	}

	void GModel::SetAccumulatedMatrix(const GMatrix& accumulatedMatrix)
	{
		this->ModelAccumulatedMatrix = accumulatedMatrix;
	}

	void GModel::Submit(const std::string& channel)
	{
		this->RootMeshNode->Submit(channel, this->ModelAccumulatedMatrix);
	}

	void GModel::LinkTechnique(std::string renderGraphName)
	{
		for (auto& mesh : this->ModelMeshList)
		{
			mesh->LinkTechnique(renderGraphName);
		}
	}

	std::shared_ptr<GMeshNode> GModel::GetRootMeshNode()
	{
		return this->RootMeshNode;
	}

	std::shared_ptr<GMesh> GModel::ParseMesh(const aiScene* scene, aiMesh* mesh)
	{
		GMesh::Data MeshData;
		std::vector<GMesh::Vertex> Vertices;
		std::vector<UINT> Indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			GMesh::Vertex Vertex = {};

			Vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			Vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->mTextureCoords[0])
			{
				Vertex.TextureCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}

			Vertices.push_back(Vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				Indices.push_back(face.mIndices[j]);
			}
		}

		MeshData.Vertices = Vertices;
		MeshData.Indices = Indices;

		aiMaterial* Material = scene->mMaterials[mesh->mMaterialIndex];
		std::shared_ptr<GMaterial> MeshMaterial = GMaterial::CreateNewMaterial(Material->GetName().C_Str());

		auto& albedo = this->LoadTexture(Material, aiTextureType_DIFFUSE, 0);
		if (albedo)
		{
			MeshMaterial->SetAlbedoTexture(albedo);
		}
		else
		{
			MeshMaterial->SetAlbedoValue(GVector3(0.8f, 0.8f, 0.8f));
		}

		auto& roughness = this->LoadTexture(Material, aiTextureType_SHININESS, 1);
		if (roughness)
		{
			MeshMaterial->SetRoughnessTexture(roughness);
		}
		else
		{
			MeshMaterial->SetRoughnessValue(0.8f);
		}

		auto& metallic = this->LoadMetallicTexture(Material, 2);
		if (metallic)
		{
			MeshMaterial->SetMetallicTexture(metallic);
		}
		else
		{
			MeshMaterial->SetMetallicValue(0.1f);
		}

		auto& ao = this->LoadTexture(Material, aiTextureType_AMBIENT_OCCLUSION, 3);
		if (ao)
		{
			MeshMaterial->SetAoTexture(ao);
		}
		else
		{
			MeshMaterial->SetAoValue(0.8f);
		}

		auto& normal = this->LoadTexture(Material, aiTextureType_NORMALS, 4);
		if (normal)
		{
			MeshMaterial->SetNormalTexture(normal);
		}
		else
		{
			MeshMaterial->SetNormalTextureEnable(false);
		}

		return GMesh::CreateNewMesh(mesh->mName.C_Str(), MeshData, MeshMaterial);
	}

	std::shared_ptr<GMeshNode> GModel::ParseNode(const aiScene* scene, aiNode* node)
	{
		std::vector<std::shared_ptr<GMesh>> NodeMeshList;
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			NodeMeshList.push_back(this->ModelMeshList.at(node->mMeshes[i]));
		}

		auto Node = std::make_shared<GMeshNode>(node->mName.C_Str(), NodeMeshList);
		Node->SetExtraMatrix(XMMatrixTranspose(XMLoadFloat4x4((XMFLOAT4X4*)&node->mTransformation)));
		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			Node->AddChild(this->ParseNode(scene, node->mChildren[i]));
		}

		return Node;
	}

	std::shared_ptr<GTexture> GModel::LoadTexture(aiMaterial* material, aiTextureType type, int index)
	{
		std::shared_ptr<GTexture> Texture = null;

		if (material->GetTextureCount(type))
		{
			aiString Path;
			material->GetTexture(type, 0, &Path);

			std::string TextureFilePath;
			if (std::filesystem::exists(Path.C_Str()))
			{
				TextureFilePath = Path.C_Str();
			}
			else
			{
				TextureFilePath = GUtil::ExtendDirectory(this->ModelFileDirectory, Path.C_Str());
			}

			Texture = GTexture::CreateNewTexture(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
				GSurface(TextureFilePath), index);
		}

		return Texture;
	}

	std::shared_ptr<GTexture> GModel::LoadMetallicTexture(aiMaterial* material, int index)
	{
		std::shared_ptr<GTexture> MetallicTexture = null;

		for (UINT i = 0; i < material->mNumProperties; i++)
		{
			auto Properties = material->mProperties[i];

			if (Properties->mType == aiPTI_String)
			{
				std::string key = Properties->mKey.data;
				if (key == "$raw.ReflectionFactor|file")
				{
					UINT StringLength = *(UINT*)Properties->mData;
					std::string Path = { Properties->mData + 4, StringLength };

					std::string FilePath;
					if (!std::filesystem::exists(Path))
					{
						FilePath = GUtil::ExtendDirectory(this->ModelFileDirectory, Path);
					}
					else
					{
						FilePath = Path;
					}

					MetallicTexture = GTexture::CreateNewTexture(
						GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
						GSurface(FilePath), index);

					break;
				}
			}
		}

		return MetallicTexture;
	}
}
