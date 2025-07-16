#include "GMeshInstance.h"

namespace GE
{
	GMeshInstance::GMeshInstance() : GRenderableBase<GMeshInstance>()
	{
		this->InstanceData = {};
	}

	GMeshInstance::GMeshInstance(const Data& data)
	{
		this->InitializeMeshInstance(data);
	}

	GMeshInstance::GMeshInstance(const GMeshInstance& other) : GRenderableBase<GMeshInstance>(other)
	{
		this->InstanceData = other.InstanceData;
	}

	GMeshInstance::~GMeshInstance()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();
		
		this->RenderableId = 0;
		this->VertexBuffer.reset();
		this->VertexBuffer = null;
		this->IndexBuffer.reset();
		this->IndexBuffer = null;

		this->InstanceData = {};
	}

	void GMeshInstance::InitializeMeshInstance(const Data& data)
	{
		this->InstanceData = data;

		this->SetRenderableShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Default.gvs"));
		this->SetRenderableShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Default.gps"));

		std::vector<D3D12_INPUT_ELEMENT_DESC> InputElements =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};
		this->SetRenderableInputLayout(GInputLayout::CreateNewInputLayout(InputElements.data(), (UINT)InputElements.size()));

		this->SetRenderableTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

		this->AddVertexBuffer(GVertexBuffer::CreateNewVertexBuffer(
			(void*)this->InstanceData.VerticesData.data(), (UINT)this->InstanceData.VerticesData.size(), (UINT)sizeof(Vertex)));
		this->AddIndexBuffer(GIndexBuffer::CreateNewIndexBuffer(this->InstanceData.IndicesData.data(), (UINT)this->InstanceData.IndicesData.size()));

		this->AddTransformCBuffer(GTransformCBuffer::CreateNewTransformCBuffer(
			this->GetRenderablePipelineState()->GetPipelineRootSignature()));

		this->EndUpInitializing();
	}

	void GMeshInstance::Update()
	{

	}

	const GMeshInstance::Data& GMeshInstance::GetMeshInstanceData() const noexcept
	{
		return this->InstanceData;
	}
}