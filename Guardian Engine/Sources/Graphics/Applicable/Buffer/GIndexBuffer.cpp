#include "GIndexBuffer.h"

namespace GE
{
	GIndexBuffer::GIndexBuffer() : GBuffer()
	{

	}

	GIndexBuffer::GIndexBuffer(UINT* indicesData, UINT dataSize) : 
		GBuffer((void*)indicesData, dataSize, sizeof(UINT))
	{

	}

	GIndexBuffer::GIndexBuffer(const GIndexBuffer& other) : GBuffer(other)
	{

	}

	void GIndexBuffer::Apply()
	{
		D3D12_INDEX_BUFFER_VIEW IndexBufferView;
		GUARDIAN_CLEAR_MEMORY(IndexBufferView);

		IndexBufferView.BufferLocation = this->GPUBuffer->GetGPUVirtualAddress();
		IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		IndexBufferView.SizeInBytes = this->DataSize * this->DataStride;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			IASetIndexBuffer(&IndexBufferView);
	}
}
