#include "GGraphicsContext.h"
#include "Framebuffer/GFramebuffer.h"

namespace GE
{
	GGraphicsContext::GGraphicsContext()
	{
		this->ContextWindowHandle = null;
		this->ContextEventProcesser = std::make_shared<GEventProcesser>();

		this->GraphicsFactory = null;
		this->GraphicsDevice = null;
		this->GraphicsFence = null;
		this->GraphicsCommandQueue = null;
		this->GraphicsCommandList = null;
		this->GraphicsSwapChain = null;
		this->GraphicsMainFramebuffer = null;
	}

	GGraphicsContext::GGraphicsContext(HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		this->InitializeGraphicsContext(windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other)
	{
		this->ContextId = other.ContextId;
		this->ContextWindowHandle = other.ContextWindowHandle;
		this->ContextEventProcesser = other.ContextEventProcesser;

		this->GraphicsFactory = other.GraphicsFactory;
		this->GraphicsDevice = other.GraphicsDevice;
		this->GraphicsFence = other.GraphicsFence;
		this->GraphicsCommandQueue = other.GraphicsCommandQueue;
		this->GraphicsCommandList = other.GraphicsCommandList;
		this->GraphicsSwapChain = other.GraphicsSwapChain;

		this->GraphicsFence = other.GraphicsFence;

		this->GraphicsMainFramebuffer = other.GraphicsMainFramebuffer;
	}

	GGraphicsContext::~GGraphicsContext()
	{
		this->ContextId = 0;
		this->ContextWindowHandle = null;
		this->ContextEventProcesser.reset();
		this->ContextEventProcesser = null;

		this->GraphicsFactory.reset();
		this->GraphicsFactory = null;

		this->GraphicsDevice.reset();
		this->GraphicsDevice = null;

		this->GraphicsFence.reset();
		this->GraphicsFence = null;

		this->GraphicsCommandQueue.reset();
		this->GraphicsCommandQueue = null;

		this->GraphicsCommandList.reset();
		this->GraphicsCommandList = null;
		
		this->GraphicsSwapChain.reset();
		this->GraphicsSwapChain = null;

		this->GraphicsMainFramebuffer.reset();
		this->GraphicsMainFramebuffer = null;
	}

	void GGraphicsContext::InitializeGraphicsContext(
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->ContextWindowHandle = windowHandle;

		this->ContextEventProcesser = std::make_shared<GEventProcesser>();
		this->ContextEventProcesser->OnEvent<GWindowResizeEvent>([this](const GWindowResizeEvent& event)
		{
			GUARDIAN_SETUP_AUTO_THROW();
		
			if (event.WindowHandle == this->ContextWindowHandle)
			{
				this->GraphicsFence->FlushFence(this->GraphicsCommandQueue);
				
				GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Reset(
					this->GraphicsCommandList->GetCommandListAllocator().Get(), null));
				
				this->GraphicsSwapChain->ResizeBuffer(event.ResizeWidth, event.ResizeHeight);
				
				this->GraphicsMainFramebuffer->ResizeFramebuffer(std::make_shared<GGraphicsContext>(*this), event.ResizeWidth, event.ResizeHeight);
				
				GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Close());
				ID3D12CommandList* CommandLists[] = { this->GraphicsCommandList->GetCommandListObject().Get() };
				this->GraphicsCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);
				
				this->GraphicsFence->FlushFence(this->GraphicsCommandQueue);
			}
		});

#ifdef GE_DEBUG
		WRL::ComPtr<ID3D12Debug> DebugController;
		GUARDIAN_AUTO_THROW(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)DebugController.GetAddressOf()))
		DebugController->EnableDebugLayer();
#endif

		this->GraphicsFactory = GGraphicsFactory::CreateNewGraphicsFactory(windowHandle);
		this->GraphicsDevice = GDevice::CreateNewDevice(this->GraphicsFactory);


		this->GraphicsFence = GFence::CreateNewFence(this->GraphicsDevice);

		this->GraphicsCommandQueue = GCommandQueue::CreateNewCommandQueue(this->GraphicsDevice);
		this->GraphicsCommandList = GCommandList::CreateNewCommandList(this->GraphicsDevice);

		this->GraphicsSwapChain = GSwapChain::CreateNewSwapChain(this->GraphicsFactory,
			this->GraphicsCommandQueue, 2, windowHandle, bufferWidth, bufferHeight, fullscreen);

		this->BeginInitializing();

		this->GraphicsMainFramebuffer = GFramebuffer::CreateNewFramebuffer(std::make_shared<GGraphicsContext>(*this));
		
		this->EndUpInitializing();
	}

	void GGraphicsContext::BeginRendering()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListAllocator()->Reset());

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Reset(
			this->GraphicsCommandList->GetCommandListAllocator().Get(), null));

		this->GraphicsCommandList->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	void GGraphicsContext::ApplyMainFramebuffer()
	{
		GUARDIAN_CHECK_POINTER(this->GraphicsMainFramebuffer);
		
		this->GraphicsMainFramebuffer->ApplyFramebuffer(std::make_shared<GGraphicsContext>(*this));
	}

	void GGraphicsContext::EndUpRendering(UINT syncInternal)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->GraphicsCommandList->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Close());

		ID3D12CommandList* CommandLists[] = { this->GraphicsCommandList->GetCommandListObject().Get() };
		this->GraphicsCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

		this->GraphicsSwapChain->Present(syncInternal);

		this->GraphicsFence->FlushFence(this->GraphicsCommandQueue);
	}

	void GGraphicsContext::BeginInitializing()
	{
		this->ResetCommandList();
	}

	void GGraphicsContext::EndUpInitializing()
	{
		this->ExecuteCommandList();
		this->GraphicsFence->FlushFence(this->GraphicsCommandQueue);
	}

	void GGraphicsContext::ResetCommandList()
	{
		GUARDIAN_SETUP_AUTO_THROW();
		
		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Reset(
			this->GraphicsCommandList->GetCommandListAllocator().Get(), null));
	}

	void GGraphicsContext::ExecuteCommandList()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(this->GraphicsCommandList->GetCommandListObject()->Close());
		ID3D12CommandList* CommandLists[] = { this->GraphicsCommandList->GetCommandListObject().Get() };
		this->GraphicsCommandQueue->ExecuteCommandLists(GUARDIAN_ARRAYSIZE(CommandLists), CommandLists);
	}

	const GUUID& GGraphicsContext::GetContextId() const noexcept
	{
		return this->ContextId;
	}

	std::shared_ptr<GGraphicsFactory> GGraphicsContext::GetGraphicsFactory()
	{
		return this->GraphicsFactory;
	}

	std::shared_ptr<GDevice> GGraphicsContext::GetGraphicsDevice()
	{
		return this->GraphicsDevice;
	}

	std::shared_ptr<GFence> GGraphicsContext::GetGraphicsFence()
	{
		return this->GraphicsFence;
	}

	std::shared_ptr<GCommandQueue> GGraphicsContext::GetGraphicsCommandQueue()
	{
		return this->GraphicsCommandQueue;
	}

	std::shared_ptr<GCommandList> GGraphicsContext::GetGraphicsCommandList()
	{
		return this->GraphicsCommandList;
	}

	std::shared_ptr<GSwapChain> GGraphicsContext::GetGraphicsSwapChain()
	{
		return this->GraphicsSwapChain;
	}
}
