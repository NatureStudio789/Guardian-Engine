#ifndef _GE_GUARDIANCONSTANTBUFFER_H_
#define _GE_GUARDIANCONSTANTBUFFER_H_
#include "GuardianIndexBuffer.h"

namespace GE
{
	enum GUARDIAN_API GuardianConstantBufferCategory
	{
		GE_VERTEXSHADER_CONSTANTBUFFER,
		GE_PIXELSHADER_CONSTANTBUFFER,
	};

	template<typename T>
	class GuardianConstantBuffer : public GuardianApplicable
	{
	public:
		GuardianConstantBuffer();
		GuardianConstantBuffer(std::shared_ptr<GuardianGraphics> graphics,
			GuardianConstantBufferCategory category, int index = 0);
		GuardianConstantBuffer(const GuardianConstantBuffer& other);
		virtual ~GuardianConstantBuffer() override = default;

		void InitializeConstantBuffer(std::shared_ptr<GuardianGraphics> graphics, 
			GuardianConstantBufferCategory category, int index = 0);

		void UpdateData(std::shared_ptr<GuardianGraphics> graphics, T data);
		virtual void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

		WRL::ComPtr<ID3D11Buffer> GetConstantBufferObject();
		const T& GetConstantBufferData() const;

	protected:
		WRL::ComPtr<ID3D11Buffer> ConstantBufferObject;
		T ConstantBufferData;
		UINT ConstantBufferAppliedSlot;
		GuardianConstantBufferCategory ConstantBufferCategory;
	};

	template<typename T>
	inline GuardianConstantBuffer<T>::GuardianConstantBuffer()
	{
		ConstantBufferData = T();
		this->ConstantBufferCategory = GE_VERTEXSHADER_CONSTANTBUFFER;
		this->ConstantBufferAppliedSlot = 0;
	}

	template<typename T>
	inline GuardianConstantBuffer<T>::GuardianConstantBuffer(
		std::shared_ptr<GuardianGraphics> graphics, GuardianConstantBufferCategory category, int index)
	{
		this->InitializeConstantBuffer(graphics, category, index);
	}

	template<typename T>
	inline GuardianConstantBuffer<T>::GuardianConstantBuffer(const GuardianConstantBuffer& other)
	{
		this->ConstantBufferObject = other.ConstantBufferObject;
		this->ConstantBufferData = other.ConstantBufferData;
		this->ConstantBufferCategory = other.ConstantBufferCategory;
		this->ConstantBufferAppliedSlot = other.ConstantBufferAppliedSlot;
	}

	template<typename T>
	inline void GuardianConstantBuffer<T>::InitializeConstantBuffer(
		std::shared_ptr<GuardianGraphics> graphics, GuardianConstantBufferCategory category, int index)
	{
		this->ConstantBufferCategory = category;
		this->ConstantBufferAppliedSlot = index;

		D3D11_BUFFER_DESC ConstantBufferDesc;
		ZeroMemory(&ConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

		ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ConstantBufferDesc.MiscFlags = 0;
		ConstantBufferDesc.ByteWidth = (UINT)(sizeof(T) + (16 - (sizeof(T) % 16)));
		ConstantBufferDesc.StructureByteStride = 0;

		HRESULT hr = graphics->GetGraphicsDevice()->CreateBuffer(
			&ConstantBufferDesc, null, this->ConstantBufferObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	template<typename T>
	inline void GuardianConstantBuffer<T>::UpdateData(
		std::shared_ptr<GuardianGraphics> graphics, T data)
	{
		this->ConstantBufferData = data;

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT hr = graphics->GetGraphicsDeviceContext()->Map(this->ConstantBufferObject.Get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		CopyMemory(MappedResource.pData, &this->ConstantBufferData, sizeof(T));

		graphics->GetGraphicsDeviceContext()->Unmap(this->ConstantBufferObject.Get(), 0);
	}

	template<typename T>
	inline void GuardianConstantBuffer<T>::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		switch (this->ConstantBufferCategory)
		{
			case GE_VERTEXSHADER_CONSTANTBUFFER:
			{
				graphics->GetGraphicsDeviceContext()->VSSetConstantBuffers(
					this->ConstantBufferAppliedSlot, 1, this->ConstantBufferObject.GetAddressOf());
				break;
			}

			case GE_PIXELSHADER_CONSTANTBUFFER:
			{
				graphics->GetGraphicsDeviceContext()->PSSetConstantBuffers(
					this->ConstantBufferAppliedSlot, 1, this->ConstantBufferObject.GetAddressOf());
				break;
			}
		}
		
	}

	template<typename T>
	inline WRL::ComPtr<ID3D11Buffer> GuardianConstantBuffer<T>::GetConstantBufferObject()
	{
		return this->ConstantBufferObject;
	}

	template<typename T>
	inline const T& GuardianConstantBuffer<T>::GetConstantBufferData() const
	{
		return this->ConstantBufferData;
	}

	struct GUARDIAN_API GuardianTransformProperties
	{
		GuardianTransformProperties()
		{
			this->TransformMatrix = XMMatrixIdentity();
			this->WorldTransformMatrix = XMMatrixIdentity();
		}

		GuardianTransformProperties(XMMATRIX worldTransformMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
		{
			this->TransformMatrix = worldTransformMatrix * viewMatrix * projectionMatrix;
			this->WorldTransformMatrix = worldTransformMatrix;
		}

		XMMATRIX TransformMatrix;
		XMMATRIX WorldTransformMatrix;
	};

	class GUARDIAN_API GuardianTransformConstantBuffer : public GuardianConstantBuffer<GuardianTransformProperties>
	{ 
	public:
		GuardianTransformConstantBuffer() : GuardianConstantBuffer()
		{
			this->ConstantBufferData = GuardianTransformProperties();
		}
		GuardianTransformConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
			: GuardianConstantBuffer(graphics, GE_VERTEXSHADER_CONSTANTBUFFER, index)
		{
			this->ConstantBufferData = GuardianTransformProperties();
		}
		GuardianTransformConstantBuffer(const GuardianTransformConstantBuffer& other) : GuardianConstantBuffer(other)
		{

		}

		void SetWorldTransform(XMMATRIX worldMatrix)
		{
			this->ConstantBufferData.WorldTransformMatrix = worldMatrix;
		}

		static std::shared_ptr<GuardianTransformConstantBuffer> CreateNewTransformConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
		{
			return std::make_shared<GuardianTransformConstantBuffer>(graphics, index);
		}
	};

	struct GUARDIAN_API GuardianMaterialProperties
	{
	public:
		GuardianMaterialProperties()
		{

		}

		GVector3 AlbedoColor;
		float MetallicColor;
		float RoughnessColor;
		float AoColor;
		int UsingAlbedoMap;
		int UsingMetallicMap;
		int UsingRoughnessMap;
		int UsingNormalMap;
		int UsingAoMap;
	};

	class GUARDIAN_API GuardianMaterialConstantBuffer : public GuardianConstantBuffer<GuardianMaterialProperties>
	{
	public:
		GuardianMaterialConstantBuffer() : GuardianConstantBuffer()
		{
			this->ConstantBufferData = GuardianMaterialProperties();
		}
		GuardianMaterialConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
			: GuardianConstantBuffer(graphics, GE_PIXELSHADER_CONSTANTBUFFER, index)
		{
			this->ConstantBufferData = GuardianMaterialProperties();
		}
		GuardianMaterialConstantBuffer(const GuardianMaterialConstantBuffer& other) :
			GuardianConstantBuffer(other)
		{

		}

		static std::shared_ptr<GuardianMaterialConstantBuffer> CreateNewMaterialConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
		{
			return std::make_shared<GuardianMaterialConstantBuffer>(graphics, index);
		}
	};

	struct GUARDIAN_API GuardianLightProperties
	{
	public:
		GuardianLightProperties()
		{
			this->CameraPosition = GVector3(0.0f);
			this->LightNumber = 0;
		}
		GuardianLightProperties(const GuardianLightProperties& other)
		{
			this->CameraPosition = other.CameraPosition;
			this->LightNumber = other.LightNumber;

			for (int i = 0; i < this->LightNumber; i++)
			{
				this->PointLightList[i] = other.PointLightList[i];
			}
		}
		~GuardianLightProperties()
		{
		}

		GVector3 CameraPosition;
		int LightNumber;
		GuardianPointLightProperties PointLightList[50];
	};

	class GUARDIAN_API GuardianLightConstantBuffer : public GuardianConstantBuffer<GuardianLightProperties>
	{
	public:
		GuardianLightConstantBuffer() : GuardianConstantBuffer()
		{
			this->ConstantBufferData = GuardianLightProperties();
		}
		GuardianLightConstantBuffer(std::shared_ptr<GuardianGraphics> graphics, int index = 1) : 
			GuardianConstantBuffer(graphics, GE_PIXELSHADER_CONSTANTBUFFER, index)
		{
			this->ConstantBufferData = GuardianLightProperties();
		}
		GuardianLightConstantBuffer(const GuardianLightConstantBuffer& other) : GuardianConstantBuffer(other)
		{

		}

		static std::shared_ptr<GuardianLightConstantBuffer> CreateNewLightConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 1)
		{
			return std::make_shared<GuardianLightConstantBuffer>(graphics, index);
		}
	};
}

#endif