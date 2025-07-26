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
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetConstantBufferCPUView();
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGPUView();

		WRL::ComPtr<ID3D12Resource> UploadBuffer;
		std::shared_ptr<GRootSignature> BufferRootSignature;
		UINT BufferIndex;
		UINT BufferRootParameterIndex;
		
		T BufferData;
		UINT DataSize;
		BYTE* MappedData;
	};

	template<typename T>
	inline GConstantBuffer<T>::GConstantBuffer()
	{
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
		this->BufferRootParameterIndex = this->BufferIndex;
		this->DataSize = (sizeof(T) + 255) & ~255;
		this->BufferRootSignature = rootSignature;

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
			CreateConstantBufferView(&ConstantBufferDesc, this->GetConstantBufferCPUView());
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
		this->BufferRootSignature->SetRootDescriptorTable(
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList(), this->BufferRootParameterIndex,
			this->GetConstantBufferGPUView());
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

	template<typename T>
	inline CD3DX12_CPU_DESCRIPTOR_HANDLE GConstantBuffer<T>::GetConstantBufferCPUView()
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(this->BufferRootSignature->GetConstantBufferDescriptorHeap()->GetFirstCPUDescriptorHandle()).Offset(
			this->BufferIndex, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDescriptorSize(
				D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}

	template<typename T>
	inline CD3DX12_GPU_DESCRIPTOR_HANDLE GConstantBuffer<T>::GetConstantBufferGPUView()
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(this->BufferRootSignature->GetConstantBufferDescriptorHeap()->GetFirstGPUDescriptorHandle()).Offset(
			this->BufferIndex, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDescriptorSize(
				D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
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