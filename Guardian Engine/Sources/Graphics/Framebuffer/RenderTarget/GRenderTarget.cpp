#include "GRenderTarget.h"

namespace GE
{
	GRenderTarget::GRenderTarget()
	{
		this->RenderTargetViewDescriptorHeap = null;
	}

	GRenderTarget::GRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		this->InitializeRenderTarget(graphicsContext);
	}

	GRenderTarget::GRenderTarget(const GRenderTarget& other)
	{
		this->RenderTargetViewDescriptorHeap = other.RenderTargetViewDescriptorHeap;
	}

	GRenderTarget::~GRenderTarget()
	{
		this->RenderTargetViewDescriptorHeap.reset();
	}

	void GRenderTarget::InitializeRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		this->RenderTargetViewDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(graphicsContext->GetGraphicsDevice(),
			graphicsContext->GetGraphicsSwapChain()->GetBufferCount(), GDescriptorHeap::GE_DESCRIPTOR_HEAP_RTV);

		auto RenderTargetView = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->RenderTargetViewDescriptorHeap->GetFirstCPUDescriptorHandle());
		
		for (UINT i = 0; i < graphicsContext->GetGraphicsSwapChain()->GetBufferCount(); i++)
		{
			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
				graphicsContext->GetGraphicsSwapChain()->GetBufferList()[i].Get(), null, RenderTargetView);
			RenderTargetView.Offset(1, graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}
	}

	void GRenderTarget::ResizeRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		auto RenderTargetView = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->RenderTargetViewDescriptorHeap->GetFirstCPUDescriptorHandle());

		for (UINT i = 0; i < graphicsContext->GetGraphicsSwapChain()->GetBufferCount(); i++)
		{
			graphicsContext->GetGraphicsDevice()->GetDeviceObject()->CreateRenderTargetView(
				graphicsContext->GetGraphicsSwapChain()->GetBufferList()[i].Get(), null, RenderTargetView);
			RenderTargetView.Offset(1, graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}
	}

	void GRenderTarget::ClearRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		static float color[] = { 0.001f, 0.001f, 0.001f, 1.0f };
		graphicsContext->GetGraphicsCommandList()->GetCommandListObject()->ClearRenderTargetView(
			this->GetRenderTargetView(graphicsContext), color, 0, null);
	}

	void GRenderTarget::ApplyRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext, 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView)
	{
		graphicsContext->GetGraphicsCommandList()->GetCommandListObject()->OMSetRenderTargets(1, 
			&this->GetRenderTargetView(graphicsContext), true, null);
	}

	std::shared_ptr<GDescriptorHeap> GRenderTarget::GetRenderTargetViewDescriptorHeap()
	{
		return this->RenderTargetViewDescriptorHeap;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GRenderTarget::GetRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(this->RenderTargetViewDescriptorHeap->GetFirstCPUDescriptorHandle(), 
			graphicsContext->GetGraphicsSwapChain()->GetCurrentBufferIndex(), 
			graphicsContext->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
}
