#include "GSwapChain.h"

namespace GE
{
	GSwapChain::GSwapChain()
	{
		this->BufferWidth = this->BufferHeight = 0;
		this->BufferCount = 2;
		this->CurrentBufferIndex = 0;
		this->BufferList.clear();
	}

	GSwapChain::GSwapChain(std::shared_ptr<GGraphicsFactory> factory,
		std::shared_ptr<GCommandQueue> commandQueue, UINT bufferCount,
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		this->InitializeSwapChain(factory, commandQueue, bufferCount, windowHandle, bufferWidth, bufferHeight, fullscreen);
	}

	GSwapChain::GSwapChain(const GSwapChain& other)
	{
		this->BufferWidth = other.BufferWidth;
		this->BufferHeight = other.BufferHeight;
		this->BufferCount = other.BufferCount;
		this->CurrentBufferIndex = other.CurrentBufferIndex;
		this->BufferList = other.BufferList;
		this->SwapChainObject = other.SwapChainObject;
	}

	GSwapChain::~GSwapChain()
	{
		this->BufferWidth = this->BufferHeight = 0;
		this->BufferCount = 0;
		this->CurrentBufferIndex = 0;

		this->ReleaseBuffers();
	}

	void GSwapChain::InitializeSwapChain(std::shared_ptr<GGraphicsFactory> factory,
		std::shared_ptr<GCommandQueue> commandQueue, UINT bufferCount,
		HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_CHECK_VALUE(bufferCount);
		GUARDIAN_CHECK_VALUE(bufferWidth);
		GUARDIAN_CHECK_VALUE(bufferHeight);
		this->BufferWidth = bufferWidth;
		this->BufferHeight = bufferHeight;
		this->BufferCount = bufferCount;
		this->CurrentBufferIndex = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		GUARDIAN_CLEAR_MEMORY(SwapChainDesc);

		SwapChainDesc.BufferDesc.Width = this->BufferWidth;
		SwapChainDesc.BufferDesc.Height = this->BufferHeight;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferCount = this->BufferCount;
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

		this->CreateBuffers();
	}

	void GSwapChain::CreateBuffers()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		for (UINT i = 0; i < this->BufferCount; i++)
		{
			WRL::ComPtr<ID3D12Resource> Buffer;
			GUARDIAN_AUTO_THROW(this->SwapChainObject->GetBuffer(i, __uuidof(ID3D12Resource), (void**)Buffer.GetAddressOf()));

			this->BufferList.push_back(Buffer);
		}
	}

	void GSwapChain::ResizeBuffer(int newWidth, int newHeight)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		GUARDIAN_CHECK_VALUE(newWidth);
		GUARDIAN_CHECK_VALUE(newHeight);

		this->ReleaseBuffers();

		GUARDIAN_AUTO_THROW(this->SwapChainObject->ResizeBuffers(this->BufferCount, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0));

		this->BufferWidth = newWidth;
		this->BufferHeight = newHeight;
		this->CurrentBufferIndex = 0;

		this->CreateBuffers();
	}

	void GSwapChain::ReleaseBuffers()
	{
		for (auto& buffer : this->BufferList)
		{
			buffer.Reset();
		}
		this->BufferList.clear();
	}

	void GSwapChain::Present(UINT syncInternal)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		GUARDIAN_AUTO_THROW(this->SwapChainObject->Present(syncInternal, 0));

		this->CurrentBufferIndex = (this->CurrentBufferIndex + 1) % this->BufferCount;
	}

	const int& GSwapChain::GetBufferWidth() const noexcept
	{
		return this->BufferWidth;
	}

	const int& GSwapChain::GetBufferHeight() const noexcept
	{
		return this->BufferHeight;
	}

	const UINT& GSwapChain::GetBufferCount() const noexcept
	{
		return this->BufferCount;
	}

	const UINT& GSwapChain::GetCurrentBufferIndex() const noexcept
	{
		return this->CurrentBufferIndex;
	}

	WRL::ComPtr<ID3D12Resource> GSwapChain::GetCurrentBuffer() const noexcept
	{
		return this->BufferList[this->CurrentBufferIndex];
	}

	const std::vector<WRL::ComPtr<ID3D12Resource>> GSwapChain::GetBufferList() const noexcept
	{
		return this->BufferList;
	}

	WRL::ComPtr<IDXGISwapChain> GSwapChain::GetSwapChainObject()
	{
		return this->SwapChainObject;
	}
}
