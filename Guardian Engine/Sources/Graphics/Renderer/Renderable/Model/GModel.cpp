#include "GModel.h"

namespace GE
{
	GModel::GModel()
	{
		this->RootMeshNode = null;
		this->ModelMeshList.clear();
	}

	GModel::GModel(const std::string& filePath)
	{
		this->InitializeModel(filePath);
	}

	GModel::GModel(const GModel& other)
	{
		this->RootMeshNode = other.RootMeshNode;
		this->ModelMeshList = other.ModelMeshList;
	}

	GModel::~GModel()
	{
		this->RootMeshNode.reset();
		this->RootMeshNode = null;

		this->ModelMeshList.clear();
	}

	void GModel::InitializeModel(const std::string& filePath)
	{
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
	}

	void GModel::SetTransform(const GTransform& transform)
	{
		this->RootMeshNode->SetTransform(transform);
	}

	void GModel::Submit(const std::string& channel)
	{
		this->RootMeshNode->Submit(channel, GMatrix::IdentityMatrix());
	}

	void GModel::LinkTechnique(std::string renderGraphName)
	{
		for (auto& mesh : this->ModelMeshList)
		{
			mesh->LinkTechnique(renderGraphName);
		}
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

		return GMesh::CreateNewMesh(mesh->mName.C_Str(), MeshData);
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
}
