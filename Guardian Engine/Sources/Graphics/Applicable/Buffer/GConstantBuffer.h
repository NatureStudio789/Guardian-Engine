#ifndef _GE_GCONSTANTBUFFER_H_
#define _GE_GCONSTANTBUFFER_H_
#include "GIndexBuffer.h"

namespace GE
{
	template<typename T>
	class GUARDIAN_API GConstantBuffer : public GApplicable
	{
	public:
		GConstantBuffer();
		GConstantBuffer(UINT index);
		GConstantBuffer(const GConstantBuffer& other);
		~GConstantBuffer();

		void InitializeConstantBuffer(UINT index);

		void UpdateBufferData(T data);
		void Apply();

		WRL::ComPtr<ID3D12Resource> GetUploadBuffer();
		const UINT& GetBufferIndex() const noexcept;

		const UINT& GetRealDataSize() const noexcept;
		const T& GetBufferData() const noexcept;

	private:
		WRL::ComPtr<ID3D12Resource> UploadBuffer;
		std::shared_ptr<GDescriptorHeap> ConstantBufferDescriptorHeap;
		UINT BufferIndex;
		UINT BufferRootParameterIndex;
		
		T BufferData;
		UINT DataSize;
		BYTE* MappedData;
	};

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer()
	{
		this->ConstantBufferDescriptorHeap = null;

		this->BufferIndex = 0;
		this->BufferRootParameterIndex = 0;
		this->BufferData = T();
		this->DataSize = 0;
		this->MappedData = null;
	}

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer(UINT index)
	{
		this->InitializeConstantBuffer(index);
	}

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer(const GConstantBuffer& other)
	{
		this->UploadBuffer = other.UploadBuffer;
		this->ConstantBufferDescriptorHeap = other.ConstantBufferDescriptorHeap;
		this->BufferIndex = other.BufferIndex;
		this->BufferRootParameterIndex = other.BufferRootParameterIndex;

		this->BufferData = other.BufferData;
		this->DataSize = other.DataSize;
		this->MappedData = other.MappedData;
	}

	template<typename T>
	inline GConstantBuffer<T>::~GConstantBuffer()
	{
		if (this->UploadBuffer)
		{
			this->UploadBuffer->Unmap(0, null);
		}
		this->ConstantBufferDescriptorHeap.reset();

		this->BufferIndex = 0;
		this->BufferRootParameterIndex = 0;
		this->BufferData = T();
		this->DataSize = 0;
		this->MappedData = null;
	}

	template<typename T>
	inline void GConstantBuffer<T>::InitializeConstantBuffer(UINT index)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->BufferIndex = index;
		this->DataSize = (sizeof(T) + 255) & ~255;

		this->ConstantBufferDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice(), 1,
			GDescriptorHeap::GE_DESCRIPTOR_HEAP_CBVSRVUAV, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(this->BufferData), D3D12_RESOURCE_STATE_GENERIC_READ, null,
				__uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		GUARDIAN_AUTO_THROW(this->UploadBuffer->Map(0, null, (void**)&this->MappedData));

		D3D12_CONSTANT_BUFFER_VIEW_DESC ConstantBufferDesc;
		GUARDIAN_CLEAR_MEMORY(ConstantBufferDesc);
		ConstantBufferDesc.BufferLocation = this->UploadBuffer->GetGPUVirtualAddress();
		ConstantBufferDesc.SizeInBytes = this->DataSize;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateConstantBufferView(&ConstantBufferDesc, this->ConstantBufferDescriptorHeap->GetFirstCPUDescriptorHandle());

		CD3DX12_ROOT_PARAMETER parameter;
		parameter.InitAsConstantBufferView(this->BufferIndex);

		this->BufferRootParameterIndex = 
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsRootSignature()->AddRootSignatureParameter(parameter);
	}

	template<typename T>
	inline void GConstantBuffer<T>::UpdateBufferData(T data)
	{
		this->BufferData = data;

		memcpy((void*)this->MappedData, &this->BufferData, sizeof(T));
	}

	template<typename T>
	inline void GConstantBuffer<T>::Apply()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsRootSignature()->SetRootConstantBufferView(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList(), this->BufferRootParameterIndex,
			this->UploadBuffer->GetGPUVirtualAddress());
	}

	template<typename T>
	inline WRL::ComPtr<ID3D12Resource> GConstantBuffer<T>::GetUploadBuffer()
	{
		return this->UploadBuffer;
	}

	template<typename T>
	inline const UINT& GConstantBuffer<T>::GetBufferIndex() const noexcept
	{
		return this->BufferIndex;
	}

	template<typename T>
	inline const UINT& GConstantBuffer<T>::GetRealDataSize() const noexcept
	{
		return this->DataSize;
	}

	template<typename T>
	inline const T& GConstantBuffer<T>::GetBufferData() const noexcept
	{
		return this->BufferData
	}
}

#endif