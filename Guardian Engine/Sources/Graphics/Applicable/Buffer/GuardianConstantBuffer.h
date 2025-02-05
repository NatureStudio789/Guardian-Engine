#ifndef _GE_GUARDIANCONSTANTBUFFER_H_
#define _GE_GUARDIANCONSTANTBUFFER_H_
#include "GuardianIndexBuffer.h"

namespace guardian
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

		void UpdateData(T data);
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
	inline void GuardianConstantBuffer<T>::UpdateData(T data)
	{
		this->ConstantBufferData = data;
	}

	template<typename T>
	inline void GuardianConstantBuffer<T>::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
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

	class GUARDIAN_API GuardianTransformConstantBuffer : public GuardianConstantBuffer<XMMATRIX>
	{ 
	public:
		GuardianTransformConstantBuffer() : GuardianConstantBuffer()
		{
			this->ConstantBufferData = XMMatrixIdentity();
		}
		GuardianTransformConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
			: GuardianConstantBuffer(graphics, GE_VERTEXSHADER_CONSTANTBUFFER, index)
		{
			this->ConstantBufferData = XMMatrixIdentity();
		}
		GuardianTransformConstantBuffer(const GuardianTransformConstantBuffer& other) : GuardianConstantBuffer(other)
		{

		}

		static std::shared_ptr<GuardianTransformConstantBuffer> CreateNewTransformConstantBuffer(
			std::shared_ptr<GuardianGraphics> graphics, int index = 0)
		{
			return std::make_shared<GuardianTransformConstantBuffer>(graphics, index);
		}
	};
}

#endif