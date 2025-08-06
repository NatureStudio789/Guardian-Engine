#include "GBuffer.h"

namespace GE
{
	GBuffer::GBuffer()
	{
		this->BufferData = null;
		this->DataSize = 0;
		this->DataStride = 0;
	}

	GBuffer::GBuffer(void* bufferData, UINT dataSize, UINT dataStride)
	{
		this->InitializeBuffer(bufferData, dataSize, dataStride);
	}

	GBuffer::GBuffer(const GBuffer& other)
	{
		this->CPUBuffer = other.CPUBuffer;
		this->GPUBuffer = other.GPUBuffer;
		this->UploadBuffer = other.UploadBuffer;

		this->BufferData = other.BufferData;
		this->DataSize = other.DataSize;
		this->DataStride = other.DataStride;
	}

	GBuffer::~GBuffer()
	{
		this->BufferData = null;
		this->DataSize = 0;
		this->DataStride = 0;
	}

	void GBuffer::InitializeBuffer(void* bufferData, UINT dataSize, UINT dataStride)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		GUARDIAN_CHECK_VALUE((unsigned long long)this->BufferData);
		GUARDIAN_CHECK_VALUE(this->DataSize);
		GUARDIAN_CHECK_VALUE(this->DataStride);
		this->BufferData = bufferData;
		this->DataSize = dataSize;
		this->DataStride = dataStride;
	
		GUARDIAN_AUTO_THROW(D3DCreateBlob(this->DataSize * this->DataStride, this->CPUBuffer.GetAddressOf()));
		CopyMemory(this->CPUBuffer->GetBufferPointer(), bufferData, this->DataSize * this->DataStride);

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer((UINT64)this->DataSize * this->DataStride), D3D12_RESOURCE_STATE_COMMON, null,
				__uuidof(ID3D12Resource), (void**)this->GPUBuffer.GetAddressOf()));

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer((UINT64)this->DataSize * this->DataStride), D3D12_RESOURCE_STATE_GENERIC_READ, null,
				__uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		D3D12_SUBRESOURCE_DATA BufferSubresourceData;
		GUARDIAN_CLEAR_MEMORY(BufferSubresourceData);
		BufferSubresourceData.pData = this->BufferData;
		BufferSubresourceData.RowPitch = LONG_PTR(this->DataSize * this->DataStride);
		BufferSubresourceData.SlicePitch = BufferSubresourceData.RowPitch;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetInitializationCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->GPUBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetInitializationCommandList()->GetCommandListObject().Get(),
			this->GPUBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &BufferSubresourceData);
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetInitializationCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->GPUBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ExecuteInitialization();
	}

	WRL::ComPtr<ID3DBlob> GBuffer::GetCPUBuffer()
	{
		return this->CPUBuffer;
	}

	WRL::ComPtr<ID3D12Resource> GBuffer::GetGPUBuffer()
	{
		return this->GPUBuffer;
	}

	WRL::ComPtr<ID3D12Resource> GBuffer::GetUploadBuffer()
	{
		return this->UploadBuffer;
	}

	const void* GBuffer::GetBufferData() const noexcept
	{
		return this->BufferData;
	}

	const UINT& GBuffer::GetDataSize() const noexcept
	{
		return this->DataSize;
	}

	const UINT& GBuffer::GetDataStride() const noexcept
	{
		return this->DataStride;
	}
}
