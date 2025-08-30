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

	GModel::GModel(const Data& data, std::string renderGraphName)
	{
		this->InitializeModel(data, renderGraphName);
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

		const auto& Data = Load(filePath);

		for (const auto& meshData : Data.MeshDataList)
		{
			this->ModelMeshList.push_back(GMesh::CreateNewMesh(meshData));
		}

		this->RootMeshNode = this->BuildMeshNode(Data.RootMeshNodeData);

		this->LinkTechnique(renderGraphName);
	}

	void GModel::InitializeModel(const Data& data, std::string renderGraphName)
	{
		this->ModelFilePath = data.FilePath;
		this->ModelFileDirectory = data.FileDirectory;

		for (const auto& meshData : data.MeshDataList)
		{
			this->ModelMeshList.push_back(GMesh::CreateNewMesh(meshData));
		}

		this->RootMeshNode = this->BuildMeshNode(data.RootMeshNodeData);

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

	std::shared_ptr<GMeshNode> GModel::BuildMeshNode(GMeshNode::Data meshNodeData)
	{
		std::shared_ptr<GMeshNode> Node;
		std::vector<std::shared_ptr<GMesh>> NodeMeshList;
		for (auto& index : meshNodeData.MeshDataIndexList)
		{
			NodeMeshList.push_back(this->ModelMeshList[index]);
		}

		Node = std::make_shared<GMeshNode>(meshNodeData.NodeName, NodeMeshList);
		Node->SetExtraMatrix(meshNodeData.ExtraMatrix);

		for (auto& child : meshNodeData.ChildrenList)
		{
			Node->AddChild(BuildMeshNode(child));
		}

		return Node;
	}

	std::shared_ptr<GMeshNode> GModel::GetRootMeshNode()
	{
		return this->RootMeshNode;
	}

	GMesh::Data GModel::ParseMesh(const aiScene* scene, aiMesh* mesh, std::string modelFileDirectory)
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
		if (!GMaterialRegistry::HasMaterial(Material->GetName().C_Str()))
		{
			std::shared_ptr<GMaterial> MeshMaterial = GMaterial::CreateNewMaterial(Material->GetName().C_Str());

			LoadTexture(Material, MeshMaterial, aiTextureType_DIFFUSE, modelFileDirectory);
			LoadTexture(Material, MeshMaterial, aiTextureType_SHININESS, modelFileDirectory);
			LoadMetallicTexture(Material, MeshMaterial, modelFileDirectory);
			LoadTexture(Material, MeshMaterial, aiTextureType_AMBIENT_OCCLUSION, modelFileDirectory);
			LoadTexture(Material, MeshMaterial, aiTextureType_NORMALS, modelFileDirectory);

			GMaterialRegistry::RegistryMaterial(MeshMaterial);
		}

		MeshData.MeshMaterialId = GMaterialRegistry::GetMaterial(Material->GetName().C_Str())->GetMaterialId();

		return MeshData;
	}

	GMeshNode::Data GModel::ParseNode(const aiScene* scene, aiNode* node, std::vector<GMesh::Data> meshDataList)
	{
		GMeshNode::Data NodeData;
		
		std::vector<UINT> NodeMeshDataList;
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			NodeMeshDataList.push_back(i);
		}

		NodeData.NodeName = node->mName.C_Str();
		NodeData.MeshDataIndexList = NodeMeshDataList;
		NodeData.ExtraMatrix = XMMatrixTranspose(XMLoadFloat4x4((XMFLOAT4X4*)&node->mTransformation));
		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			NodeData.ChildrenList.push_back(ParseNode(scene, node->mChildren[i], meshDataList));
		}

		return NodeData;
	}

	void GModel::LoadTexture(aiMaterial* material, std::shared_ptr<GMaterial> outMat, 
		aiTextureType type, std::string modelFileDirectory)
	{
		if (material->GetTextureCount(type))
		{
			aiString Path;
			material->GetTexture(type, 0, &Path);

			std::string AssetName = GUtil::GetFileName(Path.C_Str());

			switch (type)
			{
				case aiTextureType_DIFFUSE:
				{
					outMat->SetAlbedoTexture(AssetName);
					break;
				}

				case aiTextureType_NORMALS:
				{
					outMat->SetNormalTexture(AssetName);
					break;
				}

				case aiTextureType_SHININESS:
				{
					outMat->SetRoughnessTexture(AssetName);
					break;
				}

				case aiTextureType_AMBIENT_OCCLUSION:
				{
					outMat->SetAoTexture(AssetName);
					break;
				}
			}
		}
	}

	void GModel::LoadMetallicTexture(aiMaterial* material, std::shared_ptr<GMaterial> outMat, std::string modelFileDirectory)
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

					std::string AssetName = GUtil::GetFileName(Path);

					outMat->SetMetallicTexture(AssetName);

					return;
				}
			}
		}
	}
}
