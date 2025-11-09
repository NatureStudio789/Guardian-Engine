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
		virtual void Apply() override;

		WRL::ComPtr<ID3D12Resource> GetUploadBuffer();
		const UINT& GetBufferIndex() const noexcept;

		const UINT& GetRealDataSize() const noexcept;
		const T& GetBufferData() const noexcept;

	protected:
		D3D12_GPU_VIRTUAL_ADDRESS GetConstantBufferGPUVirtualAddress();

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
		this->DataSize = sizeof(T) + (16 - (sizeof(T) % 16));
		this->BufferRootSignature = rootSignature;
		this->BufferRootParameterIndex = this->BufferRootSignature->GetRootParameterIndex(
			GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_CBV, this->BufferIndex));

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(this->DataSize), D3D12_RESOURCE_STATE_GENERIC_READ, null,
				__uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		GUARDIAN_AUTO_THROW(this->UploadBuffer->Map(0, null, (void**)&this->MappedData));
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
		this->BufferRootSignature->SetRootConstantBufferView(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList(),
			this->BufferRootParameterIndex, this->GetConstantBufferGPUVirtualAddress());
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
	inline D3D12_GPU_VIRTUAL_ADDRESS GConstantBuffer<T>::GetConstantBufferGPUVirtualAddress()
	{
		return this->UploadBuffer->GetGPUVirtualAddress();
	}

	struct GTransformCBData
	{
		GTransformCBData()
		{
			this->WorldMatrix = GMatrix::IdentityMatrix();
		}
		GTransformCBData(GMatrix worldMatrix)
		{
			this->WorldMatrix = worldMatrix;
		}

		GMatrix WorldMatrix;
	};
	class GUARDIAN_API GTransformCBuffer : public GConstantBuffer<GTransformCBData>
	{
	public:
		GTransformCBuffer() : GConstantBuffer<GTransformCBData>()
		{
			this->BufferData = {};
			this->Parent = null;
		}
		GTransformCBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 0) : 
			GConstantBuffer<GTransformCBData>(rootSignature, index)
		{
			this->Parent = null;
		}
		GTransformCBuffer(const GTransformCBuffer& other) : GConstantBuffer<GTransformCBData>(other)
		{
			this->Parent = other.Parent;
		}

		void SetParent(const GRenderable& renderable) override;

		void Apply() override;

		static std::shared_ptr<GTransformCBuffer> CreateNewTransformCBuffer(
			std::shared_ptr<GRootSignature> rootSignature, UINT index = 0)
		{
			return std::make_shared<GTransformCBuffer>(rootSignature, index);
		}

	private:
		const GRenderable* Parent;
	};

	struct GCameraCBData
	{
		GCameraCBData()
		{
			this->CameraMatrix = GMatrix::IdentityMatrix();
			this->CameraPosition = {};
		}
		GCameraCBData(GMatrix cameraMatrix, GVector3 cameraPosition)
		{
			this->CameraMatrix = cameraMatrix;
			this->CameraPosition = cameraPosition;
		}

		GMatrix CameraMatrix;
		GVector3 CameraPosition;
	};
	class GUARDIAN_API GCameraCBuffer : public GConstantBuffer<GCameraCBData>
	{
	public:
		GCameraCBuffer() : GConstantBuffer<GCameraCBData>()
		{
			this->BufferData = {};
		}
		GCameraCBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 1) :
			GConstantBuffer<GCameraCBData>(rootSignature, index)
		{
		}
		GCameraCBuffer(const GCameraCBuffer& other) : GConstantBuffer<GCameraCBData>(other)
		{
		}

		static std::shared_ptr<GCameraCBuffer> CreateNewCameraCBuffer(
			std::shared_ptr<GRootSignature> rootSignature, UINT index = 1)
		{
			return std::make_shared<GCameraCBuffer>(rootSignature, index);
		}

	private:
	};

	struct GMaterialCBData
	{
	public:
		GMaterialCBData()
		{
			this->AlbedoValue = {};
			this->RoughnessValue = 0.8f;
			this->MetallicValue = 0.0f;
			this->AoValue = 0.8f;

			this->EnableAlbedoMap = false;
			this->EnableRoughnessMap = false;
			this->EnableMetallicMap = false;
			this->EnableAoMap = false;
			this->EnableNormalMap = false;
		}

		GVector3 AlbedoValue;
		float RoughnessValue;
		float MetallicValue;
		float AoValue;

		int EnableAlbedoMap;
		int EnableRoughnessMap;
		int EnableMetallicMap;
		int EnableAoMap;
		int EnableNormalMap;
	};
	class GUARDIAN_API GMaterialCBuffer : public GConstantBuffer<GMaterialCBData>
	{
	public:
		GMaterialCBuffer() : GConstantBuffer<GMaterialCBData>()
		{
			this->BufferData = {};
		}
		GMaterialCBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 2) :
			GConstantBuffer<GMaterialCBData>(rootSignature, index)
		{
		}
		GMaterialCBuffer(const GMaterialCBuffer& other) : GConstantBuffer<GMaterialCBData>(other)
		{
		}

		static std::shared_ptr<GMaterialCBuffer> CreateNewMaterialCBuffer(
			std::shared_ptr<GRootSignature> rootSignature, UINT index = 2)
		{
			return std::make_shared<GMaterialCBuffer>(rootSignature, index);
		}

	private:
	};

	struct GLightCBData
	{
	public:
		GLightCBData()
		{
			this->PointLightCount = 0;
		}

		GPointLight::Data PointLightList[GLightRegistry::MaxLightCount];
	public:
		int PointLightCount;
	};
	class GUARDIAN_API GLightCBuffer : public GConstantBuffer<GLightCBData>
	{
	public:
		GLightCBuffer() : GConstantBuffer< GLightCBData>()
		{
			this->BufferData = {};
		}
		GLightCBuffer(std::shared_ptr<GRootSignature> rootSignature, UINT index = 3) :
			GConstantBuffer<GLightCBData>(rootSignature, index)
		{
		}
		GLightCBuffer(const GLightCBuffer& other) : GConstantBuffer<GLightCBData>(other)
		{

		}

		static std::shared_ptr<GLightCBuffer> CreateNewLightCBuffer(
			std::shared_ptr<GRootSignature> rootSignature, UINT index = 3)
		{
			return std::make_shared<GLightCBuffer>(rootSignature, index);
		}
	};
}

#endif