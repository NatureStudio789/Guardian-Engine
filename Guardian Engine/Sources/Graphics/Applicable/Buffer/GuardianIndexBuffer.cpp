#include "GuardianIndexBuffer.h"

namespace guardian
{
	GuardianIndexBuffer::GuardianIndexBuffer()
	{
		this->IndexBufferData.clear();
	}

	GuardianIndexBuffer::GuardianIndexBuffer(const GuardianIndexBuffer& other)
	{
		this->IndexBufferObject = other.IndexBufferObject;
		this->IndexBufferData = other.IndexBufferData;
	}

	GuardianIndexBuffer::~GuardianIndexBuffer()
	{
		this->IndexBufferData.clear();
	}

	GuardianIndexBuffer::GuardianIndexBuffer(std::shared_ptr<GuardianGraphics> graphics, std::vector<UINT> indices)
	{
		this->InitializeIndexBuffer(graphics, indices);
	}

	void GuardianIndexBuffer::InitializeIndexBuffer(std::shared_ptr<GuardianGraphics> graphics, std::vector<UINT> indices)
	{
		this->IndexBufferData = indices;

		D3D11_BUFFER_DESC IndexBufferDesc;
		ZeroMemory(&IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		IndexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * this->IndexBufferData.size());
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA IndexBufferResourceData;
		ZeroMemory(&IndexBufferResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		IndexBufferResourceData.pSysMem = (const void*)this->IndexBufferData.data();

		HRESULT hr = graphics->GetGraphicsDevice()->CreateBuffer(&IndexBufferDesc, &IndexBufferResourceData,
			this->IndexBufferObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianIndexBuffer::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->IASetIndexBuffer(this->IndexBufferObject.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	WRL::ComPtr<ID3D11Buffer> GuardianIndexBuffer::GetIndexBufferObject() noexcept
	{
		return this->IndexBufferObject;
	}

	const std::vector<UINT>& GuardianIndexBuffer::GetIndexBufferData() const noexcept
	{
		return this->IndexBufferData;
	}

	std::shared_ptr<GuardianIndexBuffer> GuardianIndexBuffer::CreateNewIndexBuffer(
		std::shared_ptr<GuardianGraphics> graphics, std::vector<UINT> indices)
	{
		return std::make_shared<GuardianIndexBuffer>(graphics, indices);
	}
}
