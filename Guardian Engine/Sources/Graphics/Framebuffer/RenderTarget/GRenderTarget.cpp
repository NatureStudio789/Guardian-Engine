#include "GRenderTarget.h"

namespace GE
{
	GRenderTarget::GRenderTarget()
	{
		this->RTVDescriptorHandle = null;
	}

	GRenderTarget::GRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		this->InitializeRenderTarget(graphicsContext);
	}

	GRenderTarget::GRenderTarget(const GRenderTarget& other)
	{
		this->RTVDescriptorHandle = other.RTVDescriptorHandle;
	}

	GRenderTarget::~GRenderTarget()
	{
		this->RTVDescriptorHandle.reset();
		this->RTVDescriptorHandle = null;
	}

	void GRenderTarget::InitializeRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
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

	void GRenderTarget::ResizeRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		auto RTV = this->RTVDescriptorHandle->CPUHandle;
		for (UINT i = 0; i < graphicsContext->GetGraphicsSwapChain()->GetBufferCount(); i++)
		{
			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
				graphicsContext->GetGraphicsSwapChain()->GetBufferList()[i].Get(), null, RTV);
			RTV.Offset(1, graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
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

	D3D12_CPU_DESCRIPTOR_HANDLE GRenderTarget::GetRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(this->RTVDescriptorHandle->CPUHandle,
			graphicsContext->GetGraphicsSwapChain()->GetCurrentBufferIndex(), 
			graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
}
