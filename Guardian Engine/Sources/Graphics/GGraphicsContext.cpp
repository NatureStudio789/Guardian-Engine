#include "GGraphicsContext.h"

namespace GE
{
	GGraphicsContext::GGraphicsContext()
	{
		this->GraphicsFactory = null;
		this->GraphicsDevice = null;
		this->GraphicsCommandQueue = null;
		this->GraphicsCommandList = null;
		this->GraphicsSwapChain = null;

		this->RTVDescriptorHeap = null;
		this->RTVDescriptorSize = -1;
		this->DSVDescriptorHeap = null;
		this->DSVDescriptorSize = -1;
	}

	GGraphicsContext::GGraphicsContext(
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		this->InitializeGraphicsContext(windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other)
	{
		this->ContextId = other.ContextId;

		this->GraphicsFactory = other.GraphicsFactory;
		this->GraphicsDevice = other.GraphicsDevice;
		this->GraphicsCommandQueue = other.GraphicsCommandQueue;
		this->GraphicsCommandList = other.GraphicsCommandList;
		this->GraphicsSwapChain = other.GraphicsSwapChain;

		this->GraphicsFence = other.GraphicsFence;

		this->RTVDescriptorHeap = other.RTVDescriptorHeap;
		this->RTVDescriptorSize = other.RTVDescriptorSize;
		this->DSVDescriptorHeap = other.DSVDescriptorHeap;
		this->DSVDescriptorSize = other.DSVDescriptorSize;
	}

	GGraphicsContext::~GGraphicsContext()
	{
		this->ContextId = 0;

		this->GraphicsFactory.reset();
		this->GraphicsFactory = null;

		this->GraphicsDevice.reset();
		this->GraphicsDevice = null;

		this->GraphicsCommandQueue.reset();
		this->GraphicsCommandQueue = null;

		this->GraphicsCommandList.reset();
		this->GraphicsCommandList = null;
		
		this->GraphicsSwapChain.reset();
		this->GraphicsSwapChain = null;

		this->RTVDescriptorHeap.reset();
		this->RTVDescriptorHeap = null;
		this->RTVDescriptorSize = -1;
		this->DSVDescriptorHeap.reset();
		this->DSVDescriptorHeap = null;
		this->DSVDescriptorSize = -1;
	}

	void GGraphicsContext::InitializeGraphicsContext(
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->GraphicsFactory = GGraphicsFactory::CreateNewGraphicsFactory(windowHandle);
		this->GraphicsDevice = GDevice::CreateNewDevice(this->GraphicsFactory);

		GUARDIAN_AUTO_THROW(this->GraphicsDevice->GetDeviceObject()->CreateFence(
			0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)this->GraphicsFence.GetAddressOf()));
		this->RTVDescriptorSize = this->GraphicsDevice->GetDeviceObject()->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		this->DSVDescriptorSize = this->GraphicsDevice->GetDeviceObject()->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		this->GraphicsCommandQueue = GCommandQueue::CreateNewCommandQueue(this->GraphicsDevice);
		this->GraphicsCommandList = GCommandList::CreateNewCommandList(this->GraphicsDevice);

		this->GraphicsSwapChain = GSwapChain::CreateNewSwapChain(this->GraphicsFactory,
			this->GraphicsCommandQueue, windowHandle, bufferWidth, bufferHeight, fullscreen);

		this->RTVDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(this->GraphicsDevice,
			2, GDescriptorHeap::GE_DESCRIPTOR_HEAP_RTV);
		this->DSVDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(this->GraphicsDevice,
			1, GDescriptorHeap::GE_DESCRIPTOR_HEAP_DSV);
	}

	void GGraphicsContext::BeginRendering()
	{
		GUARDIAN_SETUP_AUTO_THROW();
		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListAllocator()->Reset());

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Reset(
			this->GraphicsCommandList->GetCommandListAllocator().Get(), null));

		this->GraphicsCommandList->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetBackBuffer().Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	}

	void GGraphicsContext::EndUpRendering(UINT syncInternal)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->GraphicsCommandList->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetBackBuffer().Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Close());

		ID3D12CommandList* CommandLists[] = { this->GraphicsCommandList->GetCommandListObject().Get() };
		this->GraphicsCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

		this->GraphicsSwapChain->Present(syncInternal);
	}

	const GUUID& GGraphicsContext::GetContextId() const noexcept
	{
		return this->ContextId;
	}
}
