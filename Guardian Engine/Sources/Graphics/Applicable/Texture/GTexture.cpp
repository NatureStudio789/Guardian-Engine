#include "GTexture.h"

namespace GE
{
	GTexture::GTexture() : GShaderView()
	{
		this->TextureRootSignature = null;
	}

	GTexture::GTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index)
	{
		this->InitializeTexture(rootSignature, surface, index);
	}

	GTexture::GTexture(const GTexture& other) : GShaderView(other)
	{
		this->TextureRootSignature = other.TextureRootSignature;
		this->TextureResource = other.TextureResource;
		this->TextureUploadHeap = other.TextureUploadHeap;
	}

	GTexture::~GTexture()
	{
		this->TextureRootSignature = null;
	}

	void GTexture::InitializeTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->TextureRootSignature = rootSignature;
		if (!this->TextureRootSignature)
		{
			this->ViewRootParameterIndex = 0;
		}
		else
		{
			this->ViewRootParameterIndex = this->TextureRootSignature->GetRootParameterIndex(
				GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, index));
		}

		this->AllocateDescriptor(1);

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, surface.GetSurfaceWidth(), surface.GetSurfaceHeight(), 1, 1),
				D3D12_RESOURCE_STATE_COPY_DEST, null, __uuidof(ID3D12Resource), (void**)this->TextureResource.GetAddressOf()));

		const auto UploadBufferSize = GetRequiredIntermediateSize(this->TextureResource.Get(), 0, 1);
		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->TextureUploadHeap.GetAddressOf()));

		D3D12_SUBRESOURCE_DATA TextureSubresourceData;
		GUARDIAN_CLEAR_MEMORY(TextureSubresourceData);
		TextureSubresourceData.pData = (const void*)surface.GetBufferData();
		TextureSubresourceData.RowPitch = (long long)(((surface.GetSurfaceWidth() * sizeof(GColor)) + 3) & ~3);
		TextureSubresourceData.SlicePitch = (long long)TextureSubresourceData.RowPitch * surface.GetSurfaceHeight();
		UpdateSubresources(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetInitializationCommandList()->GetCommandListObject().Get(),
			this->TextureResource.Get(), this->TextureUploadHeap.Get(), 0, 0, 1, &TextureSubresourceData);

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetInitializationCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->TextureResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->TextureResource.Get(), null, this->ViewDescriptorHandle->CPUHandle);

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ExecuteInitialization();
	}

	void GTexture::Apply()
	{
		if (!this->TextureRootSignature)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This texture doesn't support apply!");
		}

		this->ApplyShaderView();
	}

	WRL::ComPtr<ID3D12Resource> GTexture::GetTextureResource()
	{
		return this->TextureResource;
	}

	WRL::ComPtr<ID3D12Resource> GTexture::GetTextureUploadHeap()
	{
		return this->TextureUploadHeap;
	}
}
