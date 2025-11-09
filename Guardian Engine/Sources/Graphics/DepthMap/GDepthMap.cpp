#include "GDepthMap.h"

namespace GE
{
	GDepthMap::GDepthMap() : GShaderView()
	{
		this->MapWidth = 0;
		this->MapHeight = 0;

		this->DSVDescriptorHandle = null;
	}

	GDepthMap::GDepthMap(UINT width, UINT height)
	{
		this->InitializeDepthMap(width, height);
	}

	GDepthMap::GDepthMap(const GDepthMap& other) : GShaderView(other)
	{
		this->MapWidth = other.MapWidth;
		this->MapHeight = other.MapHeight;

		this->DepthMapBuffer = other.DepthMapBuffer;
		this->DSVDescriptorHandle = other.DSVDescriptorHandle;
	}

	GDepthMap::~GDepthMap()
	{
		this->MapWidth = this->MapHeight = 0;

		this->DSVDescriptorHandle.reset();
	}

	void GDepthMap::InitializeDepthMap(UINT width, UINT height)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->MapWidth = width;
		this->MapHeight = height;

		D3D12_RESOURCE_DESC	DepthMapBufferDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapBufferDesc);

		DepthMapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthMapBufferDesc.Alignment = 0;
		DepthMapBufferDesc.Width = this->MapWidth;
		DepthMapBufferDesc.Height = this->MapHeight;
		DepthMapBufferDesc.DepthOrArraySize = 1;
		DepthMapBufferDesc.MipLevels = 1;
		DepthMapBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthMapBufferDesc.SampleDesc.Count = 1;
		DepthMapBufferDesc.SampleDesc.Quality = 0;
		DepthMapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthMapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE BufferClearValue;
		GUARDIAN_CLEAR_MEMORY(BufferClearValue);
		BufferClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		BufferClearValue.DepthStencil.Depth = 1.0f;
		BufferClearValue.DepthStencil.Stencil = 0;

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, &DepthMapBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, &BufferClearValue,
				__uuidof(ID3D12Resource), (void**)this->DepthMapBuffer.GetAddressOf()));

		this->DSVDescriptorHandle = GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetDSVDescriptorHeap()->Allocate(1);
		if (!this->GetDescriptorAllocated())
		{
			this->AllocateDescriptor();
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC DepthMapDSVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapDSVDesc);
		DepthMapDSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		DepthMapDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthMapDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthMapDSVDesc.Texture2D.MipSlice = 0;
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateDepthStencilView(this->DepthMapBuffer.Get(), &DepthMapDSVDesc, this->DSVDescriptorHandle->CPUHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->ViewDescriptorHandle->CPUHandle);

		GViewport::Attribute DepthMapViewportAttribute;
		DepthMapViewportAttribute.PositionX = 0.0f;
		DepthMapViewportAttribute.PositionY = 0.0f;
		DepthMapViewportAttribute.Width = (float)this->MapWidth;
		DepthMapViewportAttribute.Height = (float)this->MapHeight;
		DepthMapViewportAttribute.MinDepth = 0.0f;
		DepthMapViewportAttribute.MaxDepth = 1.0f;
		this->DepthMapViewport = GViewport::CreateNewViewport(DepthMapViewportAttribute);
	}

	void GDepthMap::ResetDescriptor(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		GShaderView::ResetDescriptor(cpuHandle, gpuHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->ViewDescriptorHandle->CPUHandle);
	}

	void GDepthMap::ApplyDepthMap()
	{
		this->DepthMapViewport->ApplyViewport(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList());

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			OMSetRenderTargets(0, null, false, &this->DSVDescriptorHandle->CPUHandle);
	}

	void GDepthMap::ClearDepthMap()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ClearDepthStencilView(this->DSVDescriptorHandle->CPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	void GDepthMap::BeginRendering()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->DepthMapBuffer.Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	}

	void GDepthMap::EndUpRendering()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->DepthMapBuffer.Get(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
	}

	void GDepthMap::ResizeDepthMap(UINT newWidth, UINT newHeight)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->MapWidth = newWidth;
		this->MapHeight = newHeight;

		this->DepthMapBuffer.Reset();

		D3D12_RESOURCE_DESC	DepthMapBufferDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapBufferDesc);

		DepthMapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthMapBufferDesc.Alignment = 0;
		DepthMapBufferDesc.Width = this->MapWidth;
		DepthMapBufferDesc.Height = this->MapHeight;
		DepthMapBufferDesc.DepthOrArraySize = 1;
		DepthMapBufferDesc.MipLevels = 1;
		DepthMapBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthMapBufferDesc.SampleDesc.Count = 1;
		DepthMapBufferDesc.SampleDesc.Quality = 0;
		DepthMapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthMapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE BufferClearValue;
		GUARDIAN_CLEAR_MEMORY(BufferClearValue);
		BufferClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		BufferClearValue.DepthStencil.Depth = 1.0f;
		BufferClearValue.DepthStencil.Stencil = 0;

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, &DepthMapBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, &BufferClearValue,
				__uuidof(ID3D12Resource), (void**)this->DepthMapBuffer.GetAddressOf()));

		D3D12_DEPTH_STENCIL_VIEW_DESC DepthMapDSVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapDSVDesc);
		DepthMapDSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		DepthMapDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthMapDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthMapDSVDesc.Texture2D.MipSlice = 0;
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateDepthStencilView(this->DepthMapBuffer.Get(), &DepthMapDSVDesc, this->DSVDescriptorHandle->CPUHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->ViewDescriptorHandle->CPUHandle);

		GViewport::Attribute DepthMapViewportAttribute;
		DepthMapViewportAttribute.PositionX = 0.0f;
		DepthMapViewportAttribute.PositionY = 0.0f;
		DepthMapViewportAttribute.Width = (float)this->MapWidth;
		DepthMapViewportAttribute.Height = (float)this->MapHeight;
		DepthMapViewportAttribute.MinDepth = 0.0f;
		DepthMapViewportAttribute.MaxDepth = 1.0f;
		this->DepthMapViewport->SetViewportAttribute(DepthMapViewportAttribute);
	}

	const UINT& GDepthMap::GetDepthMapWidth() const noexcept
	{
		return this->MapWidth;
	}

	const UINT& GDepthMap::GetDepthMapHeight() const noexcept
	{
		return this->MapHeight;
	}

	WRL::ComPtr<ID3D12Resource> GDepthMap::GetDepthMapBuffer()
	{
		return this->DepthMapBuffer;
	}

	std::shared_ptr<GDescriptorHandle> GDepthMap::GetDepthMapDSVDescriptorHandle()
	{
		return this->DSVDescriptorHandle;
	}
}
