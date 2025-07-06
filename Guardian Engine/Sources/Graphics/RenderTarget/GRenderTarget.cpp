#include "GRenderTarget.h"

namespace GE
{
	GRenderTarget::GRenderTarget()
	{
		GUARDIAN_CLEAR_MEMORY(this->RenderTargetView);
	}

	GRenderTarget::GRenderTarget(std::shared_ptr<GSwapChain> swapChain, 
		std::shared_ptr<GDevice> device, std::shared_ptr<GDescriptorHeap> rtvDescriptorHeap)
	{
		this->InitializeRenderTarget(swapChain, device, rtvDescriptorHeap);
	}

	GRenderTarget::GRenderTarget(const GRenderTarget& other)
	{
		this->RenderTargetView = other.RenderTargetView;
	}

	GRenderTarget::~GRenderTarget()
	{
		GUARDIAN_CLEAR_MEMORY(this->RenderTargetView);
	}

	void GRenderTarget::InitializeRenderTarget(std::shared_ptr<GSwapChain> swapChain, 
		std::shared_ptr<GDevice> device, std::shared_ptr<GDescriptorHeap> rtvDescriptorHeap)
	{
		this->RenderTargetView = rtvDescriptorHeap->GetFirstCPUDescriptorHandle();
		device->GetDeviceObject()->CreateRenderTargetView(swapChain->GetBackBuffer().Get(), null, this->RenderTargetView);
	}

	void GRenderTarget::ApplyRenderTarget(std::shared_ptr<GCommandList> commandList)
	{
		static float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		commandList->GetCommandListObject()->ClearRenderTargetView(this->RenderTargetView, color, 0, null);

		commandList->GetCommandListObject()->OMSetRenderTargets(1, &this->RenderTargetView, true, null);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GE::GRenderTarget::GetRenderTargetView()
	{
		return this->RenderTargetView;
	}
}
