#include "GGraphicsContext.h"
#include "Framebuffer/GFramebuffer.h"

namespace GE
{
	GGraphicsContext::GGraphicsContext()
	{
		this->ContextWindowHandle = null;

		this->GraphicsFactory = null;
		this->GraphicsDevice = null;
		this->GraphicsFence = null;
		this->GraphicsCommandQueue = null;
		this->CurrentCommandListName = "";
		GraphicsCommandListBatch.clear();
		this->GraphicsSwapChain = null;
	}

	GGraphicsContext::GGraphicsContext(HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		this->InitializeGraphicsContext(windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other)
	{
		this->ContextId = other.ContextId;
		this->ContextWindowHandle = other.ContextWindowHandle;

		this->GraphicsFactory = other.GraphicsFactory;
		this->GraphicsDevice = other.GraphicsDevice;
		this->GraphicsFence = other.GraphicsFence;
		this->GraphicsCommandQueue = other.GraphicsCommandQueue;
		GraphicsCommandListBatch = other.GraphicsCommandListBatch;
		this->GraphicsSwapChain = other.GraphicsSwapChain;

		this->GraphicsFence = other.GraphicsFence;
	}

	GGraphicsContext::~GGraphicsContext()
	{
		this->ContextId = 0;
		this->ContextWindowHandle = null;

		this->GraphicsFactory.reset();
		this->GraphicsFactory = null;

		this->GraphicsDevice.reset();
		this->GraphicsDevice = null;

		this->GraphicsFence.reset();
		this->GraphicsFence = null;

		this->GraphicsCommandQueue.reset();
		this->GraphicsCommandQueue = null;

		for (auto& commandList : GraphicsCommandListBatch)
		{
			commandList.second.reset();
			commandList.second = null;
		}
		GraphicsCommandListBatch.clear();
		
		this->GraphicsSwapChain.reset();
		this->GraphicsSwapChain = null;
	}

	void GGraphicsContext::InitializeGraphicsContext(
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->ContextWindowHandle = windowHandle;

#ifdef GE_DEBUG
		WRL::ComPtr<ID3D12Debug> DebugController;
		GUARDIAN_AUTO_THROW(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)DebugController.GetAddressOf()))
		DebugController->EnableDebugLayer();
#endif

		this->GraphicsFactory = GGraphicsFactory::CreateNewGraphicsFactory(windowHandle);
		this->GraphicsDevice = GDevice::CreateNewDevice(this->GraphicsFactory);


		this->GraphicsFence = GFence::CreateNewFence(this->GraphicsDevice);

		this->GraphicsCommandQueue = GCommandQueue::CreateNewCommandQueue(this->GraphicsDevice);
			
		this->GraphicsSwapChain = GSwapChain::CreateNewSwapChain(this->GraphicsFactory,
			this->GraphicsCommandQueue, 2, windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	void GGraphicsContext::RegisterGraphicsCommandList(const std::string& name)
	{
		if (this->GraphicsCommandListBatch.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION("The command list named '{}' already exists in graphics context.");
		}

		this->GraphicsCommandListBatch[name] = GCommandList::CreateNewCommandList(this->GraphicsDevice);
	}

	void GGraphicsContext::SetCurrentCommandList(const std::string& name)
	{
		if (!this->GraphicsCommandListBatch.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION("The command list named '{}' DOESN'T already exist in graphics context.");
		}

		this->CurrentCommandListName = name;
	}

	void GGraphicsContext::BeginRendering()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(this->GetGraphicsCommandList()->GetCommandListAllocator()->Reset());

		GUARDIAN_AUTO_THROW(this->GetGraphicsCommandList()->GetCommandListObject()->Reset(
			this->GetGraphicsCommandList()->GetCommandListAllocator().Get(), null));

		this->GetGraphicsCommandList()->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	void GGraphicsContext::EndUpRendering()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->GetGraphicsCommandList()->GetCommandListObject()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GraphicsSwapChain->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		GUARDIAN_AUTO_THROW(this->GetGraphicsCommandList()->GetCommandListObject()->Close());
	}

	void GGraphicsContext::PresentRenderingResult(UINT syncInternal)
	{
		this->GraphicsSwapChain->Present(syncInternal);

		this->FlushCommandQueue();
	}

	void GGraphicsContext::BeginInitializing()
	{
		this->ResetCommandList();
	}

	void GGraphicsContext::EndUpInitializing()
	{
		this->ExecuteCommandList();
		this->FlushCommandQueue();
	}

	void GGraphicsContext::ResetCommandList()
	{
		GUARDIAN_SETUP_AUTO_THROW();
		
		GUARDIAN_AUTO_THROW(this->GetGraphicsCommandList()->GetCommandListObject()->Reset(
			this->GetGraphicsCommandList()->GetCommandListAllocator().Get(), null));
	}

	void GGraphicsContext::ExecuteCommandList()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(this->GetGraphicsCommandList()->GetCommandListObject()->Close());
		ID3D12CommandList* CommandLists[] = { this->GetGraphicsCommandList()->GetCommandListObject().Get() };
		this->GraphicsCommandQueue->ExecuteCommandLists(GUARDIAN_ARRAYSIZE(CommandLists), CommandLists);
	}

	void GGraphicsContext::ExecuteCommandListBatch()
	{
		std::vector<ID3D12CommandList*> CommandLists;

		for (auto& commandList : this->GraphicsCommandListBatch)
		{
			CommandLists.push_back(commandList.second->GetCommandListObject().Get());
		}
		this->GraphicsCommandQueue->ExecuteCommandLists((UINT)CommandLists.size(), CommandLists.data());
	}

	void GGraphicsContext::FlushCommandQueue()
	{
		this->GraphicsFence->FlushFence(this->GraphicsCommandQueue);
	}

	const GUUID& GGraphicsContext::GetContextId() const noexcept
	{
		return this->ContextId;
	}

	HWND GGraphicsContext::GetContextWindowHandle() noexcept
	{
		return this->ContextWindowHandle;
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
		return this->GraphicsCommandListBatch[this->CurrentCommandListName];
	}

	std::map<std::string, std::shared_ptr<GCommandList>> GGraphicsContext::GetGraphicsCommandListBatch()
	{
		return this->GraphicsCommandListBatch;
	}

	std::shared_ptr<GSwapChain> GGraphicsContext::GetGraphicsSwapChain()
	{
		return this->GraphicsSwapChain;
	}
}
