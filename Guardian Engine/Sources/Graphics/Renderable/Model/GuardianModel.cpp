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
		for (auto& mesh : this->ModelMeshList)
		{
			mesh.reset();
		}
		this->ModelMeshList.clear();
	}

	void GuardianModel::InitializeModel(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath)
	{
		this->ModelFilePath = modelFilePath;
		this->ModelFileDirectory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

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
			GuardianRenderer::SubmitRenderable(GE_SUBMIT_DEFAULT3D, mesh);
		}
	}

	void GuardianModel::UpdateModel(XMMATRIX transformMatrix)
	{
		for (auto& mesh : this->ModelMeshList)
		{
			mesh->GetTransformConstantBuffer()->UpdateData(transformMatrix);
		}
	}

	const GString& GuardianModel::GetModelFilePath() const noexcept
	{
		return this->ModelFilePath;
	}

	void GuardianModel::ProcessModelNode(std::shared_ptr<GuardianGraphics> graphics, aiNode* node, const aiScene* scene)
	{
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->ModelMeshList.push_back(this->ProcessModelMesh(graphics, mesh, scene));
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessModelNode(graphics, node->mChildren[i], scene);
		}
	}

	std::shared_ptr<GuardianMesh> GuardianModel::ProcessModelMesh(std::shared_ptr<GuardianGraphics> graphics, aiMesh* mesh, const aiScene* scene)
	{
		struct ModelVertex
		{
			GVector3 Position;
			GVector2 TextureCoord;
			GVector3 Normal;
		};

		std::shared_ptr<GuardianMesh> Mesh = std::make_shared<GuardianMesh>();
		std::vector<ModelVertex> Vertices;
		std::vector<UINT> Indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			ModelVertex Vertex;

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

		if (!Mesh->IsStaticApplicablesInitialized())
		{
			Mesh->AddStaticApplicable(GuardianRasterizerState::CreateNewRasterizerState(graphics, GE_FILL_SOLID, GE_CULL_BACK));
			Mesh->AddStaticApplicable(GuardianTopology::CreateNewTopology(GE_TOPOLOGY_TRIANGLELIST));

			auto vs = GuardianVertexShader::CreateNewVertexShader(graphics, "../Guardian Engine/Shaders/ModelVertexShader.hlsl");
			Mesh->AddStaticApplicable(vs);
			Mesh->AddStaticApplicable(
				GuardianPixelShader::CreateNewPixelShader(graphics, "../Guardian Engine/Shaders/ModelPixelShader.hlsl"));

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			Mesh->AddStaticApplicable(GuardianInputLayout::CreateNewInputLayout(graphics, vs, id, ARRAYSIZE(id)));

			Mesh->AddStaticApplicable(GuardianSampler::CreateNewSampler(graphics, GE_FILTER_MIN_MAG_MIP_LINEAR));
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Mesh->AddApplicable(this->LoadMaterialTexture(graphics, material, aiTextureType_DIFFUSE));

		Mesh->AddVertexBuffer(GuardianVertexBuffer::CreateNewVertexBuffer(graphics, (void*)Vertices.data(), (UINT)Vertices.size(), (UINT)sizeof(ModelVertex)));
		Mesh->AddIndexBuffer(GuardianIndexBuffer::CreateNewIndexBuffer(graphics, Indices));

		Mesh->AddTransformConstantBuffer(GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(graphics));

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
