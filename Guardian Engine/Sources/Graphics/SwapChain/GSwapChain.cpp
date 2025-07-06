#include "GSwapChain.h"

namespace GE
{
	GSwapChain::GSwapChain()
	{

	}

	GSwapChain::GSwapChain(std::shared_ptr<GGraphicsFactory> factory,
		std::shared_ptr<GCommandQueue> commandQueue,
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		this->InitializeSwapChain(factory, commandQueue, windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	GSwapChain::GSwapChain(const GSwapChain& other)
	{
		this->SwapChainObject = other.SwapChainObject;
	}

	GSwapChain::~GSwapChain()
	{
	}

	void GSwapChain::InitializeSwapChain(std::shared_ptr<GGraphicsFactory> factory,
		std::shared_ptr<GCommandQueue> commandQueue,
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		GUARDIAN_CLEAR_MEMORY(SwapChainDesc);

		SwapChainDesc.BufferDesc.Width = bufferWidth;
		SwapChainDesc.BufferDesc.Height = bufferHeight;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.OutputWindow = windowHandle;
		SwapChainDesc.Windowed = !fullscreen;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		GUARDIAN_AUTO_THROW(factory->GetFactoryObject()->CreateSwapChain(commandQueue->GetCommandQueueObject().Get(), 
			&SwapChainDesc, this->SwapChainObject.GetAddressOf()));
	}

	void GSwapChain::Present(UINT syncInternal)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		GUARDIAN_AUTO_THROW(this->SwapChainObject->Present(syncInternal, 0));
	}

	WRL::ComPtr<ID3D12Resource> GSwapChain::GetBackBuffer()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		WRL::ComPtr<ID3D12Resource> BackBuffer;
		GUARDIAN_AUTO_THROW(this->SwapChainObject->GetBuffer(0, __uuidof(ID3D12Resource), (void**)BackBuffer.GetAddressOf()));

		return BackBuffer;
	}

	WRL::ComPtr<IDXGISwapChain> GSwapChain::GetSwapChainObject()
	{
		return this->SwapChainObject;
	}
}
