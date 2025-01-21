#include "GuardianVertexBuffer.h"

namespace guardian
{
	GuardianVertexBuffer::GuardianVertexBuffer()
	{
		this->VertexBufferData = null;
	}

	GuardianVertexBuffer::GuardianVertexBuffer(std::shared_ptr<GuardianGraphics> graphics,
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		this->InitializeVertexBuffer(graphics, vertices, dataSize, dataTypeStride);
	}

	GuardianVertexBuffer::~GuardianVertexBuffer()
	{
		this->VertexBufferData = null;
	}

	void GuardianVertexBuffer::InitializeVertexBuffer(std::shared_ptr<GuardianGraphics> graphics,
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		this->VertexBufferData = vertices;
		this->BufferDataSize = dataSize;
		this->BufferDataTypeStride = dataTypeStride;

		D3D11_BUFFER_DESC VertexBufferDesc;
		ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		VertexBufferDesc.ByteWidth = (UINT)(this->BufferDataTypeStride * this->BufferDataSize);
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA VertexBufferResourceData;
		ZeroMemory(&VertexBufferResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		VertexBufferResourceData.pSysMem = this->VertexBufferData;

		HRESULT hr = graphics->GetGraphicsDevice()->CreateBuffer(&VertexBufferDesc, &VertexBufferResourceData,
			this->VertexBufferObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianVertexBuffer::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		static UINT offset = 0;
		graphics->GetGraphicsDeviceContext()->IASetVertexBuffers(0, 1, this->VertexBufferObject.GetAddressOf(),
			&this->BufferDataTypeStride, &offset);
	}

	const void* GuardianVertexBuffer::GetVertexBufferData() const noexcept
	{
		return this->VertexBufferData;
	}

	const UINT GuardianVertexBuffer::GetVertexBufferDataSize() const noexcept
	{
		return this->BufferDataSize;
	}

	WRL::ComPtr<ID3D11Buffer> GuardianVertexBuffer::GetVertexBufferObject() noexcept
	{
		return this->VertexBufferObject;
	}

	std::shared_ptr<GuardianVertexBuffer> GuardianVertexBuffer::CreateNewVertexBuffer(
		std::shared_ptr<GuardianGraphics> graphics,
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		return std::make_shared<GuardianVertexBuffer>(graphics, vertices, dataSize, dataTypeStride);
	}
}
