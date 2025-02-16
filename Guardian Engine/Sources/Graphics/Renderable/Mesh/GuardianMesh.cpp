#include "GuardianMesh.h"
#include "../../Renderer/GuardianRenderer.h"

namespace guardian
{
	GuardianMesh::GuardianMesh() : GuardianRenderableBase<GuardianMesh>()
	{
		this->MeshFilePath.clear();
	}

	GuardianMesh::GuardianMesh(const GuardianMesh& other) : GuardianRenderableBase<GuardianMesh>(other)
	{
		this->VertexData = other.VertexData;
		this->IndexData = other.IndexData;
		this->MeshFilePath = other.MeshFilePath;
	}

	GuardianMesh::~GuardianMesh()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();

		this->VertexData.clear();
		this->IndexData.clear();
		this->MeshFilePath.clear();
	}

	void GuardianMesh::InitializeMesh(std::shared_ptr<GuardianGraphics> graphics, std::vector<Vertex> vertices, std::vector<UINT> indices)
	{
		this->VertexData = vertices;
		this->IndexData = indices;

		if (!this->IsStaticApplicablesInitialized())
		{
			this->AddStaticApplicable(GuardianRasterizerState::CreateNewRasterizerState(graphics, GE_FILL_SOLID, GE_CULL_BACK));
			this->AddStaticApplicable(GuardianTopology::CreateNewTopology(GE_TOPOLOGY_TRIANGLELIST));

			auto vs = GuardianVertexShader::CreateNewVertexShader(graphics, "../Guardian Engine/Shaders/MeshVertexShader.hlsl");
			this->AddStaticApplicable(vs);
			this->AddStaticApplicable(
				GuardianPixelShader::CreateNewPixelShader(graphics, "../Guardian Engine/Shaders/MeshPixelShader.hlsl"));

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			this->AddStaticApplicable(GuardianInputLayout::CreateNewInputLayout(graphics, vs, id, ARRAYSIZE(id)));

			this->AddStaticApplicable(GuardianSampler::CreateNewSampler(graphics, GE_FILTER_MIN_MAG_MIP_LINEAR));
		}

		this->AddVertexBuffer(GuardianVertexBuffer::CreateNewVertexBuffer(graphics, 
			(void*)vertices.data(), (UINT)vertices.size(), (UINT)sizeof(Vertex)));
		this->AddIndexBuffer(GuardianIndexBuffer::CreateNewIndexBuffer(graphics, indices));

		this->AddTransformConstantBuffer(GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(graphics));
	}

	void GuardianMesh::InitializeMesh(std::shared_ptr<GuardianGraphics> graphics, const GString& meshFilePath)
	{
		this->Deserialize(meshFilePath);

		if (!this->IsStaticApplicablesInitialized())
		{
			this->AddStaticApplicable(GuardianRasterizerState::CreateNewRasterizerState(graphics, GE_FILL_SOLID, GE_CULL_BACK));
			this->AddStaticApplicable(GuardianTopology::CreateNewTopology(GE_TOPOLOGY_TRIANGLELIST));

			auto vs = GuardianVertexShader::CreateNewVertexShader(graphics, "../Guardian Engine/Shaders/MeshVertexShader.hlsl");
			this->AddStaticApplicable(vs);
			this->AddStaticApplicable(
				GuardianPixelShader::CreateNewPixelShader(graphics, "../Guardian Engine/Shaders/MeshPixelShader.hlsl"));

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			this->AddStaticApplicable(GuardianInputLayout::CreateNewInputLayout(graphics, vs, id, ARRAYSIZE(id)));

			this->AddStaticApplicable(GuardianSampler::CreateNewSampler(graphics, GE_FILTER_MIN_MAG_MIP_LINEAR));
		}

		this->AddVertexBuffer(GuardianVertexBuffer::CreateNewVertexBuffer(graphics,
			(void*)this->VertexData.data(), (UINT)this->VertexData.size(), (UINT)sizeof(Vertex)));
		this->AddIndexBuffer(GuardianIndexBuffer::CreateNewIndexBuffer(graphics, this->IndexData));

		this->AddTransformConstantBuffer(GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(graphics));
	}

	void GuardianMesh::SetMeshMaterial(std::shared_ptr<GuardianMaterial> material)
	{
		this->RenderingMaterial = material;
	}

	void GuardianMesh::Update()
	{
		
	}

	void GuardianMesh::UpdateMeshTransform(XMMATRIX transform)
	{
		this->RenderingTransformConstantBuffer->UpdateData(transform);
	}

	void GuardianMesh::Serialize(const GString& filePath)
	{
		YAML::Emitter MeshOutput;
		MeshOutput << YAML::BeginMap;
		MeshOutput << YAML::Key << "Mesh";

		MeshOutput << YAML::BeginMap;
		MeshOutput << YAML::Key << "Vertices" << YAML::Value << YAML::BeginSeq;

		for (auto& vertex : this->VertexData)
		{
			MeshOutput << YAML::BeginMap;

			MeshOutput << YAML::Key << "Position" << YAML::Value << vertex.Position;
			MeshOutput << YAML::Key << "TextureCoord" << YAML::Value << vertex.TextureCoord;
			MeshOutput << YAML::Key << "Normal" << YAML::Value << vertex.Normal;

			MeshOutput << YAML::EndMap;
		}

		MeshOutput << YAML::EndSeq;

		MeshOutput << YAML::Key << "Indices" << YAML::Value << YAML::BeginSeq;

		for (int i = 0; i < (int)this->IndexData.size(); i += 3)
		{
			MeshOutput << YAML::BeginMap;

			MeshOutput << YAML::Value << GVector3((float)this->IndexData[i],
				(float)this->IndexData[i + 1], (float)this->IndexData[i + 2]);

			MeshOutput << YAML::EndMap;
		}

		MeshOutput << YAML::EndSeq;
		MeshOutput << YAML::EndMap;

		MeshOutput << YAML::EndMap;

		this->MeshFilePath = filePath;
		std::ofstream MeshOutputFile(this->MeshFilePath);
		MeshOutputFile << MeshOutput.c_str();
	}

	void GuardianMesh::Deserialize(const GString& filePath)
	{
		this->MeshFilePath = filePath;

		std::ifstream SceneFile(this->MeshFilePath);
		std::stringstream SceneFileStringStream;
		SceneFileStringStream << SceneFile.rdbuf();
		if (SceneFileStringStream.str().empty())
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to load mesh : '" + filePath + "' !");
		}

		YAML::Node MeshData = YAML::Load(SceneFileStringStream.str())["Mesh"];

		auto Vertices = MeshData["Vertices"];
		this->VertexData.clear();
		for (auto& vertex : Vertices)
		{
			Vertex v;
			v.Position = vertex["Position"].as<GVector3>();
			v.TextureCoord = vertex["TextureCoord"].as<GVector2>();
			v.Normal = vertex["Normal"].as<GVector3>();

			this->VertexData.push_back(v);
		}

		auto Indices = MeshData["Indices"];
		this->IndexData.clear();
		for (auto& index : Indices)
		{
			GVector3& face = index.as<GVector3>();

			this->IndexData.push_back((UINT)face.x);
			this->IndexData.push_back((UINT)face.y);
			this->IndexData.push_back((UINT)face.z);
		}
	}

	const std::vector<GVector3> GuardianMesh::GetPositionVertices() const noexcept
	{
		std::vector<GVector3> PositionVertices;
		for (auto& Vertex : this->VertexData)
		{
			PositionVertices.push_back(Vertex.Position);
		}

		return PositionVertices;
	}

	const std::vector<UINT>& GuardianMesh::GetIndices() const noexcept
	{
		return this->IndexData;
	}

	bool GuardianMesh::operator==(const GuardianMesh& other) const
	{
		return this->RenderableId == other.RenderableId;
	}
}
