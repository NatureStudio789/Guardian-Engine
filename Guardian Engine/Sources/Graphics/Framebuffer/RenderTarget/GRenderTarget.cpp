#include "GRenderTarget.h"

namespace GE
{
	GRenderTarget::GRenderTarget()
	{
		this->EnableRTT = false;
		this->RTVDescriptorHandle = null;
	}

	GRenderTarget::GRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT)
	{
		this->InitializeRenderTarget(graphicsContext, enableRTT);
	}

	GRenderTarget::GRenderTarget(const GRenderTarget& other)
	{
		this->EnableRTT = other.EnableRTT;
		this->RTTBuffer = other.RTTBuffer;

		this->RTVTextureDescriptorHandle = other.RTVTextureDescriptorHandle;
		this->RTVDescriptorHandle = other.RTVDescriptorHandle;
	}

	GRenderTarget::~GRenderTarget()
	{
		this->RTVDescriptorHandle.reset();
		this->RTVDescriptorHandle = null;
	}

	void GRenderTarget::InitializeRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->EnableRTT = enableRTT;

		if (this->EnableRTT)
		{
			this->RTVDescriptorHandle = graphicsContext->GetRTVDescriptorHeap()->Allocate(1);
			this->RTVTextureDescriptorHandle = graphicsContext->GetSRVDescriptorHeap()->Allocate(1);

			DXGI_SWAP_CHAIN_DESC desc;
			graphicsContext->GetGraphicsSwapChain()->GetSwapChainObject()->GetDesc(&desc);

			D3D12_CLEAR_VALUE ClearValue;
			GUARDIAN_CLEAR_MEMORY(ClearValue);
			ClearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			ClearValue.Color[0] = 0.1f;
			ClearValue.Color[1] = 0.1f;
			ClearValue.Color[2] = 0.1f;
			ClearValue.Color[3] = 1.0f;

			GUARDIAN_AUTO_THROW(graphicsContext->GetGraphicsDevice()->GetDeviceObject()->
				CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
						desc.BufferDesc.Width, desc.BufferDesc.Height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, 
					&ClearValue, __uuidof(ID3D12Resource), (void**)this->RTTBuffer.GetAddressOf()));

			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
				this->RTTBuffer.Get(), null, this->RTVDescriptorHandle->CPUHandle);

			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->
				CreateShaderResourceView(this->RTTBuffer.Get(), null, this->RTVTextureDescriptorHandle->CPUHandle);
		}
		else
		{
			this->RTVDescriptorHandle = graphicsContext->GetRTVDescriptorHeap()->Allocate(
				graphicsContext->GetGraphicsSwapChain()->GetBufferCount());

			auto RTV = this->RTVDescriptorHandle->CPUHandle;
			for (UINT i = 0; i < graphicsContext->GetGraphicsSwapChain()->GetBufferCount(); i++)
			{
				graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
					graphicsContext->GetGraphicsSwapChain()->GetBufferList()[i].Get(), null, RTV);
				RTV.Offset(1, graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			}
		}
	}

	void GRenderTarget::ResizeRenderTargetView(
		std::shared_ptr<GGraphicsContext> graphicsContext, int newWidth, int newHeight)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		if (this->EnableRTT)
		{
			if (this->RTTBuffer)
			{
				this->RTTBuffer.Reset();
			}

			D3D12_CLEAR_VALUE ClearValue;
			GUARDIAN_CLEAR_MEMORY(ClearValue);
			ClearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			ClearValue.Color[0] = 0.1f;
			ClearValue.Color[1] = 0.1f;
			ClearValue.Color[2] = 0.1f;
			ClearValue.Color[3] = 1.0f;

			GUARDIAN_AUTO_THROW(graphicsContext->GetGraphicsDevice()->GetDeviceObject()->
				CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
						newWidth, newHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
					&ClearValue, __uuidof(ID3D12Resource), (void**)this->RTTBuffer.GetAddressOf()));

			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
				this->RTTBuffer.Get(), null, this->RTVDescriptorHandle->CPUHandle);

			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->
				CreateShaderResourceView(this->RTTBuffer.Get(), null, this->RTVTextureDescriptorHandle->CPUHandle);
		}
		else
		{
			auto RTV = this->RTVDescriptorHandle->CPUHandle;
			for (UINT i = 0; i < graphicsContext->GetGraphicsSwapChain()->GetBufferCount(); i++)
			{
				graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
					graphicsContext->GetGraphicsSwapChain()->GetBufferList()[i].Get(), null, RTV);
				RTV.Offset(1, graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			}
		}
	}

	void GRenderTarget::ClearRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		static float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		graphicsContext->GetGraphicsCommandList()->GetCommandListObject()->ClearRenderTargetView(
			this->GetRenderTargetView(graphicsContext), color, 0, null);
	}

	void GRenderTarget::ApplyRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView)
	{
		graphicsContext->GetGraphicsCommandList()->GetCommandListObject()->OMSetRenderTargets(1, 
			&this->GetRenderTargetView(graphicsContext), true, &depthStencilView);
	}

	WRL::ComPtr<ID3D12Resource> GRenderTarget::GetRTTBuffer()
	{
		return this->RTTBuffer;
	}

	std::shared_ptr<GDescriptorHandle> GRenderTarget::GetTextureDescriptorHandle()
	{
		return this->RTVTextureDescriptorHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GRenderTarget::GetRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		if (this->EnableRTT)
		{
			return this->RTVDescriptorHandle->CPUHandle;
		}
		else
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(this->RTVDescriptorHandle->CPUHandle,
				graphicsContext->GetGraphicsSwapChain()->GetCurrentBufferIndex(),
				graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}
	}
}
