#ifndef _GE_GCONSTANTBUFFER_H_
#define _GE_GCONSTANTBUFFER_H_
#include "GIndexBuffer.h"

namespace GE
{
	template<typename T>
	class GConstantBuffer : public GApplicable
	{
	public:
		GConstantBuffer();
		GConstantBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 0);
		GConstantBuffer(const GConstantBuffer& other);
		~GConstantBuffer();

		void InitializeConstantBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 0);

		void UpdateBufferData(T data);
		void Apply();

		WRL::ComPtr<ID3D12Resource> GetUploadBuffer();
		const UINT& GetBufferIndex() const noexcept;

		const UINT& GetRealDataSize() const noexcept;
		const T& GetBufferData() const noexcept;

	protected:
		WRL::ComPtr<ID3D12Resource> UploadBuffer;
		std::shared_ptr<GRootSignature> BufferRootSignature;
		std::shared_ptr<GDescriptorHeap> BufferDescriptorHeap;
		UINT BufferIndex;
		UINT BufferRootParameterIndex;
		
		T BufferData;
		UINT DataSize;
		BYTE* MappedData;
	};

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer()
	{
		this->BufferDescriptorHeap = null;
		this->BufferRootSignature = null;

		this->BufferIndex = 0;
		this->BufferRootParameterIndex = 0;
		this->BufferData = T();
		this->DataSize = 0;
		this->MappedData = null;
	}

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index)
	{
		this->InitializeConstantBuffer(rootSignature, index);
	}

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer(const GConstantBuffer& other)
	{
		this->UploadBuffer = other.UploadBuffer;
		this->BufferDescriptorHeap = other.BufferDescriptorHeap;
		this->BufferRootSignature = other.BufferRootSignature;
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
		this->BufferDescriptorHeap.reset();

		this->BufferRootSignature = null;

		this->BufferIndex = 0;
		this->BufferRootParameterIndex = 0;
		this->BufferData = T();
		this->DataSize = 0;
		this->MappedData = null;
	}

	template<typename T>
	inline void GConstantBuffer<T>::InitializeConstantBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->BufferIndex = index;
		this->DataSize = (sizeof(T) + 255) & ~255;
		this->BufferRootSignature = rootSignature;

		this->BufferDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice(), 1,
			GDescriptorHeap::GE_DESCRIPTOR_HEAP_CBVSRVUAV, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(this->DataSize), D3D12_RESOURCE_STATE_GENERIC_READ, null,
				__uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		GUARDIAN_AUTO_THROW(this->UploadBuffer->Map(0, null, (void**)&this->MappedData));

		D3D12_CONSTANT_BUFFER_VIEW_DESC ConstantBufferDesc;
		GUARDIAN_CLEAR_MEMORY(ConstantBufferDesc);
		ConstantBufferDesc.BufferLocation = this->UploadBuffer->GetGPUVirtualAddress();
		ConstantBufferDesc.SizeInBytes = this->DataSize;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateConstantBufferView(&ConstantBufferDesc, this->BufferDescriptorHeap->GetFirstCPUDescriptorHandle());

		GRootSignature::RootParameter Parameter;
		Parameter.Type = GRootSignature::GE_PARAMETER_CBV;
		Parameter.ShaderRegisterIndex = this->BufferIndex;

		this->BufferRootParameterIndex = this->BufferRootSignature->AddRootParameter(Parameter);
		this->BufferRootSignature->AddDescriptorHeap(this->BufferDescriptorHeap);
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
		this->BufferRootSignature->SetDescriptorHeapList(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList());

		this->BufferRootSignature->SetRootDescriptorTable(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList(), this->BufferRootParameterIndex,
			this->BufferDescriptorHeap->GetFirstGPUDescriptorHandle());
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
		return this->BufferData;
	}

	struct GTransformCBData
	{
		GTransformCBData()
		{
			this->Offset = 0.5f;
		}
		GTransformCBData(XMMATRIX transformMatrix)
		{
			this->Offset = 0.5f;
		}

		float Offset;
	};
	class GUARDIAN_API GTransformCBuffer : public GConstantBuffer<GTransformCBData>
	{
	public:
		GTransformCBuffer() : GConstantBuffer<GTransformCBData>()
		{
			this->BufferData = {};
		}
		GTransformCBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 0) : 
			GConstantBuffer<GTransformCBData>(rootSignature, index)
		{

		}
		GTransformCBuffer(const GTransformCBuffer& other) : GConstantBuffer<GTransformCBData>(other)
		{

		}

		static std::shared_ptr<GTransformCBuffer> CreateNewTransformCBuffer(
			std::shared_ptr<GRootSignature> rootSignature, UINT index = 0)
		{
			return std::make_shared<GTransformCBuffer>(rootSignature, index);
		}
	};
}

#endif