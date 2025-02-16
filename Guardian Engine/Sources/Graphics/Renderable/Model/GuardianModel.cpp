#include "GuardianModel.h"
#include "../../Renderer/GuardianRenderer.h"

namespace guardian
{
	GuardianModel::GuardianModel()
	{
		this->ModelMeshList.clear();
	}

	GuardianModel::GuardianModel(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		this->InitializeModel(graphics, modelFilePath);
	}

	GuardianModel::GuardianModel(const GuardianModel& other)
	{
		this->ModelFileDirectory = other.ModelFileDirectory;
		this->ModelFilePath = other.ModelFilePath;
		this->ModelMeshList = other.ModelMeshList;
	}

	GuardianModel::~GuardianModel()
	{
		this->ClearModelMeshList();
	}

	void GuardianModel::InitializeModel(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
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

	void GuardianModel::SubmitToRenderer()
	{
		for (auto& mesh : this->ModelMeshList)
		{
			GuardianRenderer::SubmitRenderable(GE_SUBMIT_DEFAULT3D, mesh.second);
		}
	}

	void GuardianModel::UpdateModel(XMMATRIX transformMatrix)
	{
		for (auto& mesh : this->ModelMeshList)
		{
			mesh.second->GetTransformConstantBuffer()->UpdateData(transformMatrix);
		}
	}

	void GuardianModel::ClearModelMeshList()
	{
		for (auto& mesh : this->ModelMeshList)
		{
			mesh.second.reset();
			mesh.second = null;
		}
		this->ModelMeshList.clear();
	}

	const GString& GuardianModel::GetModelFilePath() const noexcept
	{
		return this->ModelFilePath;
	}

	const std::map<GString, std::shared_ptr<GuardianMesh>>& GuardianModel::GetModelMeshList() const noexcept
	{
		return this->ModelMeshList;
	}

	const std::map<GString, std::shared_ptr<GuardianMaterial>>& GuardianModel::GetModelMaterialList() const noexcept
	{
		return this->ModelMaterialList;
	}

	void GuardianModel::ProcessModelNode(std::shared_ptr<GuardianGraphics> graphics, aiNode* node, const aiScene* scene)
	{
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->ModelMeshList[mesh->mName.C_Str()] = this->ProcessModelMesh(graphics, mesh, scene);
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessModelNode(graphics, node->mChildren[i], scene);
		}
	}

	std::shared_ptr<GuardianMesh> GuardianModel::ProcessModelMesh(std::shared_ptr<GuardianGraphics> graphics, aiMesh* mesh, const aiScene* scene)
	{
		std::shared_ptr<GuardianMesh> Mesh = std::make_shared<GuardianMesh>();
		std::vector<GuardianMesh::Vertex> Vertices;
		std::vector<UINT> Indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			GuardianMesh::Vertex Vertex;

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

		Mesh->InitializeMesh(graphics, Vertices, Indices);
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::shared_ptr<GuardianMaterial> mat = std::make_shared<GuardianMaterial>();
		mat->SetAlbedoTexture(this->LoadMaterialTexture(graphics, material, aiTextureType_DIFFUSE));

		Mesh->SetMeshMaterial(mat);
		this->ModelMaterialList[material->GetName().C_Str()] = mat;

		return Mesh;
	}

	std::shared_ptr<GuardianTexture> GuardianModel::LoadMaterialTexture(
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

	std::shared_ptr<GuardianModel> GuardianModel::CreateNewModel(
		std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		return std::make_shared<GuardianModel>(graphics, modelFilePath);
	}
}
