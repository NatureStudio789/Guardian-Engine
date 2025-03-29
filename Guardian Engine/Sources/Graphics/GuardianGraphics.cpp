#include "GuardianGraphics.h"

namespace GE
{
	GuardianGraphics::GuardianGraphics()
	{
		this->GraphicsMainFramebuffer = std::make_shared<GuardianFramebuffer>();
	}

	GuardianGraphics::GuardianGraphics(GWindowHandle renderWindow,
		int windowWidth, int windowHeight, bool isFullscreenWindow)
	{
		this->GraphicsMainFramebuffer = std::make_shared<GuardianFramebuffer>();

		this->InitializeGraphics(renderWindow, windowWidth, windowHeight, isFullscreenWindow);
	}

	void GuardianGraphics::InitializeGraphics(GWindowHandle renderWindow,
		int windowWidth, int windowHeight, bool isFullscreenWindow)
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		SwapChainDesc.BufferDesc.Width = 0;
		SwapChainDesc.BufferDesc.Height = 0;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = renderWindow;
		SwapChainDesc.Windowed = !isFullscreenWindow;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(null,
			D3D_DRIVER_TYPE_HARDWARE,
			null,
			0,
			null,
			0,
			D3D11_SDK_VERSION,
			&SwapChainDesc,
			this->GraphicsSwapChain.GetAddressOf(),
			this->GraphicsDevice.GetAddressOf(),
			null,
			this->GraphicsDeviceContext.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		this->GraphicsMainFramebuffer->InitializeFramebuffer(std::make_shared<GuardianGraphics>(*this));
	}

	void GuardianGraphics::EndUpRendering(int syncInterval)
	{
		HRESULT hr = this->GraphicsSwapChain->Present(syncInterval, 0);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianGraphics::UpdateGUIGraphicsResolution(int newWidth, int newHeight)
	{
		this->GraphicsDeviceContext->OMSetRenderTargets(0, null, null);
		
		this->GraphicsMainFramebuffer->GetFramebufferRenderTarget()->GetRenderTargetView()->Release();
		this->GraphicsMainFramebuffer->GetFramebufferRenderTarget()->GetRenderTargetTexture()->Release();
		this->GraphicsMainFramebuffer->GetFramebufferDepthStencil()->GetDepthStencilView()->Release();
		this->GraphicsMainFramebuffer->GetFramebufferDepthStencil()->GetDepthStencilState()->Release();
		this->GraphicsMainFramebuffer->GetFramebufferDepthStencil()->GetDepthStencilShaderResource()->Release();

		this->GraphicsDeviceContext->Flush();

		this->GraphicsSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);

		this->GraphicsMainFramebuffer->InitializeFramebuffer(std::make_shared<GuardianGraphics>(*this));
	}

	WRL::ComPtr<ID3D11Device> GuardianGraphics::GetGraphicsDevice() noexcept
	{
		return this->GraphicsDevice;
	}

	WRL::ComPtr<ID3D11DeviceContext> GuardianGraphics::GetGraphicsDeviceContext() noexcept
	{
		return this->GraphicsDeviceContext;
	}

	WRL::ComPtr<IDXGISwapChain> GuardianGraphics::GetGraphicsSwapChain() noexcept
	{
		return this->GraphicsSwapChain;
	}

	std::shared_ptr<GuardianFramebuffer> GuardianGraphics::GetGraphicsMainFramebuffer() noexcept
	{
		return this->GraphicsMainFramebuffer;
	}

	std::shared_ptr<GuardianGraphics> GuardianGraphics::CreateNewGraphics(
		GWindowHandle renderWindow, int windowWidth, int windowHeight, bool isFullscreenWindow)
	{
		return std::make_shared<GuardianGraphics>(renderWindow, windowWidth, windowHeight, isFullscreenWindow);
	}
}
