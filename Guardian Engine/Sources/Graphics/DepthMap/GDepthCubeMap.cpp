#include "GDepthCubeMap.h"

namespace GE
{
	GDepthCubeMap::GDepthCubeMap() : GShaderView()
	{
		this->CubeMapFaceLength = 1024;

		this->DSVDescriptorHandleList.clear();
		this->CubeMapFaceViewport = null;
	}

	GDepthCubeMap::GDepthCubeMap(UINT edgeLength)
	{
		this->InitializeDepthCubeMap(edgeLength);
	}

	GDepthCubeMap::GDepthCubeMap(const GDepthCubeMap& other) : GShaderView(other)
	{
		this->CubeMapFaceLength = other.CubeMapFaceLength;
		
		this->DepthCubeMapBuffer = other.DepthCubeMapBuffer;
		this->DSVDescriptorHandleList = other.DSVDescriptorHandleList;

		this->CubeMapFaceViewport = other.CubeMapFaceViewport;
	}

	GDepthCubeMap::~GDepthCubeMap()
	{
		this->CubeMapFaceLength = 0;
		
		for (auto& descriptorHandle : this->DSVDescriptorHandleList)
		{
			descriptorHandle.reset();
			descriptorHandle = null;
		}
		this->DSVDescriptorHandleList.clear();

		this->CubeMapFaceViewport.reset();
	}

	void GDepthCubeMap::InitializeDepthCubeMap(UINT edgeLength)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->CubeMapFaceLength = edgeLength;

		D3D12_RESOURCE_DESC DepthCubeMapBufferDesc;
		GUARDIAN_CLEAR_MEMORY(DepthCubeMapBufferDesc);

		DepthCubeMapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthCubeMapBufferDesc.Alignment = 0;
		DepthCubeMapBufferDesc.Width = this->CubeMapFaceLength;
		DepthCubeMapBufferDesc.Height = this->CubeMapFaceLength;
		DepthCubeMapBufferDesc.DepthOrArraySize = 6;
		DepthCubeMapBufferDesc.MipLevels = 1;
		DepthCubeMapBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthCubeMapBufferDesc.SampleDesc.Count = 1;
		DepthCubeMapBufferDesc.SampleDesc.Quality = 0;
		DepthCubeMapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthCubeMapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE BufferClearValue;
		GUARDIAN_CLEAR_MEMORY(BufferClearValue);
		BufferClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		BufferClearValue.DepthStencil.Depth = 1.0f;
		BufferClearValue.DepthStencil.Stencil = 0;

		GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&DepthCubeMapBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, &BufferClearValue,
				__uuidof(ID3D12Resource), (void**)this->DepthCubeMapBuffer.GetAddressOf()));

		for (UINT i = 0; i < 6; i++)
		{
			std::shared_ptr<GDescriptorHandle> DescriptorHandle =
				GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetDSVDescriptorHeap()->Allocate(1);
			this->DSVDescriptorHandleList.push_back(DescriptorHandle);

			D3D12_DEPTH_STENCIL_VIEW_DESC DepthMapDSVDesc;
			GUARDIAN_CLEAR_MEMORY(DepthMapDSVDesc);
			DepthMapDSVDesc.Flags = D3D12_DSV_FLAG_NONE;
			DepthMapDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			DepthMapDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthMapDSVDesc.Texture2DArray.ArraySize = 1;
			DepthMapDSVDesc.Texture2DArray.MipSlice = 0;
			DepthMapDSVDesc.Texture2DArray.FirstArraySlice = i;
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
				CreateDepthStencilView(this->DepthCubeMapBuffer.Get(), &DepthMapDSVDesc, this->DSVDescriptorHandleList[i]->CPUHandle);
		}

		if (!this->GetDescriptorAllocated())
		{
			this->AllocateDescriptor();
		}

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthCubeMapSRVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthCubeMapSRVDesc);
		DepthCubeMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthCubeMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthCubeMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		DepthCubeMapSRVDesc.TextureCube.MipLevels = 1;
		DepthCubeMapSRVDesc.TextureCube.MostDetailedMip = 0;
		DepthCubeMapSRVDesc.TextureCube.ResourceMinLODClamp = 0.0f;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->DepthCubeMapBuffer.Get(), &DepthCubeMapSRVDesc, this->ViewDescriptorHandle->CPUHandle);

		GViewport::Attribute DepthMapViewportAttribute;
		DepthMapViewportAttribute.PositionX = 0.0f;
		DepthMapViewportAttribute.PositionY = 0.0f;
		DepthMapViewportAttribute.Width = (float)this->CubeMapFaceLength;
		DepthMapViewportAttribute.Height = (float)this->CubeMapFaceLength;
		DepthMapViewportAttribute.MinDepth = 0.0f;
		DepthMapViewportAttribute.MaxDepth = 1.0f;
		this->CubeMapFaceViewport = GViewport::CreateNewViewport(DepthMapViewportAttribute);
	}

	void GDepthCubeMap::ResetDescriptor(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		GShaderView::ResetDescriptor(cpuHandle, gpuHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthCubeMapSRVDesc;
		GUARDIAN_CLEAR_MEMORY(DepthCubeMapSRVDesc);
		DepthCubeMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthCubeMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthCubeMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		DepthCubeMapSRVDesc.TextureCube.MipLevels = 1;
		DepthCubeMapSRVDesc.TextureCube.MostDetailedMip = 0;
		DepthCubeMapSRVDesc.TextureCube.ResourceMinLODClamp = 0.0f;

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
			CreateShaderResourceView(this->DepthCubeMapBuffer.Get(), &DepthCubeMapSRVDesc, this->ViewDescriptorHandle->CPUHandle);
	}

	void GDepthCubeMap::ApplyDepthCubeMap(UINT faceIndex)
	{
		if (faceIndex >= (UINT)this->DSVDescriptorHandleList.size())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The face index of the depth cube map is out of range of 6 faces!");
		}

		this->CubeMapFaceViewport->ApplyViewport(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList());

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			OMSetRenderTargets(0, null, false, &this->DSVDescriptorHandleList[faceIndex]->CPUHandle);
	}

	void GDepthCubeMap::ClearDepthCubeMap(UINT faceIndex)
	{
		if (faceIndex >= (UINT)this->DSVDescriptorHandleList.size())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The face index of the depth cube map is out of range of 6 faces!");
		}

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ClearDepthStencilView(this->DSVDescriptorHandleList[faceIndex]->CPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	void GDepthCubeMap::BeginRendering()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->DepthCubeMapBuffer.Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	}
	
	void GDepthCubeMap::EndUpRendering()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->DepthCubeMapBuffer.Get(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
	}

	const UINT& GDepthCubeMap::GetDepthCubeMapFaceLength() const noexcept
	{
		return this->CubeMapFaceLength;
	}

	WRL::ComPtr<ID3D12Resource> GDepthCubeMap::GetDepthCubeMapBuffer()
	{
		return this->DepthCubeMapBuffer;
	}

	std::shared_ptr<GDescriptorHandle> GDepthCubeMap::GetDepthCubeMapDSVDescriptorHandle(UINT faceIndex)
	{
		if (faceIndex >= (UINT)this->DSVDescriptorHandleList.size())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The face index of the depth cube map is out of range of 6 faces!");
		}

		return this->DSVDescriptorHandleList[faceIndex];
	}
}
