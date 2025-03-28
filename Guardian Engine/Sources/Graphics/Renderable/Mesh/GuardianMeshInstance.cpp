#include "GuardianMeshInstance.h"
#include "../../Renderer/GuardianRenderer.h"

namespace GE
{
	GuardianMeshInstance::GuardianMeshInstance() : GuardianRenderableBase<GuardianMeshInstance>()
	{
	}

	GuardianMeshInstance::GuardianMeshInstance(const GuardianMeshInstance& other) : GuardianRenderableBase<GuardianMeshInstance>(other)
	{
		this->RenderableId = other.RenderableId;
		this->InstanceData = other.InstanceData;
		this->RenderingBoundingBox = other.RenderingBoundingBox;
	}

	GuardianMeshInstance::~GuardianMeshInstance()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();

		this->InstanceData.VertexData.clear();
		this->InstanceData.IndexData.clear();
	}

	void GuardianMeshInstance::InitializeMeshInstance(std::shared_ptr<GuardianGraphics> graphics, const GuardianMeshInstance::Data& data)
	{
		this->InstanceData = data;

		this->CalculateBoundingBox();

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
			(void*)this->InstanceData.VertexData.data(), (UINT)this->InstanceData.VertexData.size(), (UINT)sizeof(Vertex)));
		this->AddIndexBuffer(GuardianIndexBuffer::CreateNewIndexBuffer(graphics, this->InstanceData.IndexData));

		this->AddTransformConstantBuffer(GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(graphics));
		this->AddLightConstantBuffer(GuardianLightConstantBuffer::CreateNewLightConstantBuffer(graphics));
	}

	void GuardianMeshInstance::SetMeshInstanceMaterial(const GuardianUUID& materialId)
	{
		this->InstanceData.MaterialId = materialId;
	}

	void GuardianMeshInstance::Update()
	{
		this->RenderingMaterial = GuardianMaterialSystem::GetMaterial(this->InstanceData.MaterialId);
	}

	void GuardianMeshInstance::UpdateMeshInstanceTransform(XMMATRIX worldMatrix)
	{
		this->RenderingTransformConstantBuffer->SetWorldTransform(worldMatrix);
	}

	void GuardianMeshInstance::UpdateMeshInstanceLighting(
		std::shared_ptr<GuardianGraphics> graphics, GuardianLightProperties properties)
	{
		this->RenderingLightConstantBuffer->UpdateData(graphics, properties);
	}

	const GuardianMeshInstance::Data& GuardianMeshInstance::GetMeshInstanceData() const noexcept
	{
		return this->InstanceData;
	}

	const std::vector<GVector3> GuardianMeshInstance::GetPositionVertices() const noexcept
	{
		std::vector<GVector3> PositionVertices;
		for (auto& Vertex : this->InstanceData.VertexData)
		{
			PositionVertices.push_back(Vertex.Position);
		}

		return PositionVertices;
	}

	const std::vector<UINT>& GuardianMeshInstance::GetIndices() const noexcept
	{
		return this->InstanceData.IndexData;
	}

	bool GuardianMeshInstance::operator==(const GuardianMeshInstance& other) const
	{
		return this->RenderableId == other.RenderableId;
	}

	void GuardianMeshInstance::CalculateBoundingBox()
	{
		auto& aabb = this->RenderingBoundingBox;
		aabb.Max = { -FLT_MAX, -FLT_MAX , -FLT_MAX };
		aabb.Min = { FLT_MAX, FLT_MAX , FLT_MAX };

		for (auto& v : this->InstanceData.VertexData)
		{
			auto& position = v.Position;

			aabb.Min.x = std::min(position.x, aabb.Min.x);
			aabb.Min.y = std::min(position.y, aabb.Min.y);
			aabb.Min.z = std::min(position.z, aabb.Min.z);

			aabb.Max.x = max(position.x, aabb.Max.x);
			aabb.Max.y = max(position.y, aabb.Max.y);
			aabb.Max.z = max(position.z, aabb.Max.z);
		}
	}
}
