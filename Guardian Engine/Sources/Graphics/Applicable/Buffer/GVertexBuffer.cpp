#include "GVertexBuffer.h"

namespace GE
{
	GVertexBuffer::GVertexBuffer() : GBuffer()
	{

	}

	GVertexBuffer::GVertexBuffer(void* verticesData, UINT dataSize, UINT dataStride) : GBuffer(verticesData, dataSize, dataStride)
	{
		
	}

	GVertexBuffer::GVertexBuffer(const GVertexBuffer& other) : GBuffer(other)
	{

	}

	void GVertexBuffer::Apply()
	{
		GUARDIAN_CHECK_VALUE((unsigned long long)this->BufferData);
		GUARDIAN_CHECK_VALUE(this->DataSize);
		GUARDIAN_CHECK_VALUE(this->DataStride);
		GUARDIAN_CHECK_VALUE((unsigned long long)this->GPUBuffer.Get());

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
		GUARDIAN_CLEAR_MEMORY(VertexBufferView);
		VertexBufferView.BufferLocation = this->GPUBuffer->GetGPUVirtualAddress();
		VertexBufferView.SizeInBytes = this->DataSize * this->DataStride;
		VertexBufferView.StrideInBytes = this->DataStride;
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			IASetVertexBuffers(0, 1, &VertexBufferView);
	}

	const UINT& GVertexBuffer::GetVerticesCount() const noexcept
	{
		return this->GetDataSize();
	}
}
