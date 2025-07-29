#include "GTexture.h"

namespace GE
{
	GTexture::GTexture()
	{
		this->TextureRootSignature = null;
		this->TextureIndex = 0;
		this->TextureRootParameterIndex = 0;
	}

	GTexture::GTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index)
	{
		this->InitializeTexture(rootSignature, surface, index);
	}

	GTexture::GTexture(const GTexture& other)
	{
		this->TextureRootSignature = other.TextureRootSignature;
		this->TextureResource = other.TextureResource;
		this->TextureUploadHeap = other.TextureUploadHeap;
		this->TextureIndex = other.TextureIndex;
		this->TextureRootParameterIndex = other.TextureRootParameterIndex;
	}

	GTexture::~GTexture()
	{
		this->TextureRootSignature = null;
		this->TextureIndex = 0;
		this->TextureRootParameterIndex = 0;
	}

	void GTexture::InitializeTexture(std::shared_ptr<GRootSignature> rootSignature, const GSurface& surface, int index)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_CHECK_POINTER(rootSignature);

		this->TextureRootSignature = rootSignature;
		this->TextureIndex = index;
		this->TextureRootParameterIndex = this->TextureRootSignature->GetRootParameterIndex(
			GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, this->TextureIndex));

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
		UpdateSubresources(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject().Get(),
			this->TextureResource.Get(), this->TextureUploadHeap.Get(), 0, 0, 1, &TextureSubresourceData);

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->TextureResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->TextureResource.Get(), null, this->GetShaderResourceCPUView());
	}

	void GTexture::Apply()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			SetGraphicsRootDescriptorTable(this->TextureRootParameterIndex, this->GetShaderResourceGPUView());
	}

	WRL::ComPtr<ID3D12Resource> GTexture::GetTextureResource()
	{
		return this->TextureResource;
	}

	WRL::ComPtr<ID3D12Resource> GTexture::GetTextureUploadHeap()
	{
		return this->TextureUploadHeap;
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE GTexture::GetShaderResourceView()
	{
		return this->GetShaderResourceGPUView();
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE GTexture::GetShaderResourceCPUView()
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			this->TextureRootSignature->GetShaderResourceDescriptorHeap()->GetFirstCPUDescriptorHandle()).Offset(
			this->TextureIndex, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->
			GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE GTexture::GetShaderResourceGPUView()
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(
			this->TextureRootSignature->GetShaderResourceDescriptorHeap()->GetFirstGPUDescriptorHandle()).Offset(
			this->TextureIndex, GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->
				GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}
}
