#include "GuardianVertexBuffer.h"

namespace GE
{
	GuardianVertexBuffer::GuardianVertexBuffer()
	{
		this->VertexBufferData = null;
	}

	GuardianVertexBuffer::GuardianVertexBuffer(const GuardianVertexBuffer& other)
	{
		this->VertexBufferData = other.VertexBufferData;
		this->BufferDataSize = other.BufferDataSize;
		this->BufferDataTypeStride = other.BufferDataTypeStride;
		this->VertexBufferObject = other.VertexBufferObject;
	}

	GuardianVertexBuffer::GuardianVertexBuffer(
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		this->InitializeVertexBuffer(vertices, dataSize, dataTypeStride);
	}

	GuardianVertexBuffer::~GuardianVertexBuffer()
	{
		this->VertexBufferData = null;
	}

	void GuardianVertexBuffer::InitializeVertexBuffer(
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		this->VertexBufferData = vertices;
		this->BufferDataSize = dataSize;
		this->BufferDataTypeStride = dataTypeStride;

		D3D11_BUFFER_DESC VertexBufferDesc;
		ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		VertexBufferDesc.ByteWidth = (UINT)(this->BufferDataTypeStride * this->BufferDataSize);
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA VertexBufferResourceData;
		ZeroMemory(&VertexBufferResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		VertexBufferResourceData.pSysMem = this->VertexBufferData;

		HRESULT hr = GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDevice()->CreateBuffer(&VertexBufferDesc, &VertexBufferResourceData,
			this->VertexBufferObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianVertexBuffer::UpdateVertices(void* vertices)
	{
		this->VertexBufferData = vertices;

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT hr = GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->Map(this->VertexBufferObject.Get(),
			0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		CopyMemory(MappedResource.pData, this->VertexBufferData, this->BufferDataTypeStride * this->BufferDataSize);

		GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDeviceContext()->Unmap(this->VertexBufferObject.Get(), 0);
	}

	void GuardianVertexBuffer::Apply()
	{
		static UINT offset = 0;
		GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->IASetVertexBuffers(0, 1, this->VertexBufferObject.GetAddressOf(),
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
		void* vertices, UINT dataSize, UINT dataTypeStride)
	{
		return std::make_shared<GuardianVertexBuffer>(vertices, dataSize, dataTypeStride);
	}
}
