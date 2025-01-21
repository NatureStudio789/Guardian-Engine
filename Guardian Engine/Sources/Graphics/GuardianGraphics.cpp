#include "GuardianGraphics.h"

namespace guardian
{
	GuardianGraphics::GuardianGraphics()
	{
		this->GraphicsRenderTarget = std::make_shared<GuardianRenderTarget>();
		this->GraphicsDepthStencil = std::make_shared<GuardianDepthStencil>();
		this->GraphicsMainViewport = std::make_shared<GuardianViewport>();
	}

	GuardianGraphics::GuardianGraphics(GWindowHandle renderWindow,
		int windowWidth, int windowHeight, bool isFullscreenWindow)
	{
		this->GraphicsMainViewport = std::make_shared<GuardianViewport>();
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

		this->GraphicsRenderTarget = GuardianRenderTarget::CreateNewRenderTarget(std::make_shared<GuardianGraphics>(*this));
		this->GraphicsDepthStencil = GuardianDepthStencil::CreateNewDepthStencil(std::make_shared<GuardianGraphics>(*this), 
			windowWidth, windowHeight);

		this->GraphicsMainViewport->InitializeViewport(GVector2(0.0f, 0.0f), GVector2((float)windowWidth, (float)windowHeight));
	}

	void GuardianGraphics::BeginRendering(const GVector3& clearColor)
	{
		this->GraphicsRenderTarget->ApplyRenderTarget(
			std::make_shared<GuardianGraphics>(*this), this->GraphicsDepthStencil, clearColor);
		this->GraphicsDeviceContext->ClearDepthStencilView(this->GraphicsDepthStencil->GetDepthStencilView().Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		this->GraphicsMainViewport->ApplyViewport(std::make_shared<GuardianGraphics>(*this));
	}

	void GuardianGraphics::EndUpRendering(int syncInterval)
	{
		HRESULT hr = this->GraphicsSwapChain->Present(syncInterval, 0);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianGraphics::UpdateGraphicsResolution(int newWidth, int newHeight)
	{
		this->GraphicsDeviceContext->OMSetRenderTargets(0, null, null);
		
		this->GraphicsRenderTarget->GetRenderTargetView()->Release();
		this->GraphicsDepthStencil->GetDepthStencilState()->Release();
		this->GraphicsDepthStencil->GetDepthStencilView()->Release();
		this->GraphicsDepthStencil->GetDepthStencilShaderResource()->Release();

		this->GraphicsDeviceContext->Flush();

		this->GraphicsSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);

		this->GraphicsRenderTarget->InitializeRenderTarget(std::make_shared<GuardianGraphics>(*this));
		this->GraphicsDepthStencil->InitializeDepthStencil(std::make_shared<GuardianGraphics>(*this), newWidth, newHeight);
		this->GraphicsMainViewport->InitializeViewport(GVector2(0.0f, 0.0f), GVector2((float)newWidth, (float)newHeight));
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

	std::shared_ptr<GuardianRenderTarget> GuardianGraphics::GetGraphicsRenderTarget() noexcept
	{
		return this->GraphicsRenderTarget;
	}

	std::shared_ptr<GuardianDepthStencil> GuardianGraphics::GetGraphicsDepthStencil() noexcept
	{
		return this->GraphicsDepthStencil;
	}

	std::shared_ptr<GuardianGraphics> GuardianGraphics::CreateNewGraphics(
		GWindowHandle renderWindow, int windowWidth, int windowHeight, bool isFullscreenWindow)
	{
		return std::make_shared<GuardianGraphics>(renderWindow, windowWidth, windowHeight, isFullscreenWindow);
	}
}
