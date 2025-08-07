#include "GDepthStencil.h"

namespace GE
{
	GDepthStencil::GDepthStencil()
	{
		this->DSVDescriptorHandle = null;
	}

	GDepthStencil::GDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		this->InitializeDepthStencil(graphicsContext);
	}

	GDepthStencil::GDepthStencil(const GDepthStencil& other)
	{
		this->DSVDescriptorHandle = other.DSVDescriptorHandle;
		this->DepthStencilBuffer = other.DepthStencilBuffer;
	}

	GDepthStencil::~GDepthStencil()
	{
		this->DSVDescriptorHandle.reset();
		this->DSVDescriptorHandle = null;
	}

	void GDepthStencil::InitializeDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->DSVDescriptorHandle = graphicsContext->GetDSVDescriptorHeap()->Allocate(1);

		D3D12_RESOURCE_DESC DepthStencilBufferDesc;
		GUARDIAN_CLEAR_MEMORY(DepthStencilBufferDesc);

		DepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilBufferDesc.Alignment = 0;
		DepthStencilBufferDesc.Width = graphicsContext->GetGraphicsSwapChain()->GetBufferWidth();
		DepthStencilBufferDesc.Height = graphicsContext->GetGraphicsSwapChain()->GetBufferHeight();
		DepthStencilBufferDesc.DepthOrArraySize = 1;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE OptimizedClear;
		GUARDIAN_CLEAR_MEMORY(OptimizedClear);
		OptimizedClear.Format = DepthStencilBufferDesc.Format;
		OptimizedClear.DepthStencil.Depth = 1.0f;
		OptimizedClear.DepthStencil.Stencil = 0;

		GUARDIAN_AUTO_THROW(graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
			&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBuffer.GetAddressOf()));

		graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateDepthStencilView(this->DepthStencilBuffer.Get(),
			null, this->DSVDescriptorHandle->CPUHandle);

		graphicsContext->GetInitializationCommandList()->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		graphicsContext->ExecuteInitialization();
	}

	void GDepthStencil::ResizeDepthStencilView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->DepthStencilBuffer.Reset();

		D3D12_RESOURCE_DESC DepthStencilBufferDesc;
		GUARDIAN_CLEAR_MEMORY(DepthStencilBufferDesc);

		DepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilBufferDesc.Alignment = 0;
		DepthStencilBufferDesc.Width = graphicsContext->GetGraphicsSwapChain()->GetBufferWidth();
		DepthStencilBufferDesc.Height = graphicsContext->GetGraphicsSwapChain()->GetBufferHeight();
		DepthStencilBufferDesc.DepthOrArraySize = 1;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE OptimizedClear;
		GUARDIAN_CLEAR_MEMORY(OptimizedClear);
		OptimizedClear.Format = DepthStencilBufferDesc.Format;
		OptimizedClear.DepthStencil.Depth = 1.0f;
		OptimizedClear.DepthStencil.Stencil = 0;

		GUARDIAN_AUTO_THROW(graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
			&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBuffer.GetAddressOf()));

		graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateDepthStencilView(this->DepthStencilBuffer.Get(),
			null, this->DSVDescriptorHandle->CPUHandle);

		graphicsContext->GetInitializationCommandList()->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		graphicsContext->ExecuteInitialization();
	}

	void GDepthStencil::ClearDepthStencil(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		graphicsContext->GetGraphicsCommandList()->GetCommandListObject()->ClearDepthStencilView(this->DSVDescriptorHandle->CPUHandle,
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	WRL::ComPtr<ID3D12Resource> GDepthStencil::GetDepthStencilBuffer()
	{
		return this->DepthStencilBuffer;
	}

	std::shared_ptr<GDescriptorHandle> GDepthStencil::GetDSVDescriptorHandle()
	{
		return this->DSVDescriptorHandle;
	}
}
