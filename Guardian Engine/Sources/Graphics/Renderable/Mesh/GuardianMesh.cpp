#include "GuardianMesh.h"

namespace guardian
{
	GuardianMesh::GuardianMesh()
	{
		
	}

	GuardianMesh::GuardianMesh(const GuardianMesh& other)
	{
		
	}

	GuardianMesh::~GuardianMesh()
	{
		for (auto& staticApplicableList : StaticApplicableList)
		{
			staticApplicableList.reset();
		}
		StaticApplicableList.clear();

		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();
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

	void GuardianMesh::Update()
	{
		
	}

	void GuardianMesh::UpdateMeshTransform(XMMATRIX transform)
	{
		this->RenderingTransformConstantBuffer->UpdateData(transform);
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

	std::shared_ptr<GuardianMaterial> GuardianMesh::GetMeshMaterial() noexcept
	{
		return this->RenderingMaterial;
	}
}
