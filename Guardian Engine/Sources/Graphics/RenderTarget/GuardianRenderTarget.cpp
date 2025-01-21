#include "GuardianRenderTarget.h"
#include "../Surface/GuardianSurface.h"
#include "../GuardianGraphics.h"

namespace guardian
{
	GuardianRenderTarget::GuardianRenderTarget(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->InitializeRenderTarget(graphics);
	}

	void GuardianRenderTarget::InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics)
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		graphics->GetGraphicsSwapChain()->GetDesc(&SwapChainDesc);
		this->RenderTargetWidth = SwapChainDesc.BufferDesc.Width;
		this->RenderTargetHeight = SwapChainDesc.BufferDesc.Height;

		WRL::ComPtr<ID3D11Texture2D> RenderTargetTexture;
		HRESULT hr = graphics->GetGraphicsSwapChain()->GetBuffer(
			0, __uuidof(ID3D11Texture2D), (void**)RenderTargetTexture.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		hr = graphics->GetGraphicsDevice()->CreateRenderTargetView(
			RenderTargetTexture.Get(), null, this->RenderTargetView.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianRenderTarget::ApplyRenderTarget(std::shared_ptr<GuardianGraphics> graphics,
		std::shared_ptr<GuardianDepthStencil> depthStencil, const GVector3& clearColor)
	{
		graphics->GetGraphicsDeviceContext()->OMSetRenderTargets(1, 
			this->RenderTargetView.GetAddressOf(), depthStencil->GetDepthStencilView().Get());

		static const float color[] = { clearColor.x, clearColor.y, clearColor.z, 1.0f };
		graphics->GetGraphicsDeviceContext()->ClearRenderTargetView(this->RenderTargetView.Get(), color);
	}

	WRL::ComPtr<ID3D11RenderTargetView> GuardianRenderTarget::GetRenderTargetView() noexcept
	{
		return this->RenderTargetView;
	}

	GuardianSurface GuardianRenderTarget::ConvertToSurface(std::shared_ptr<GuardianGraphics> graphics)
	{
		D3D11_RENDER_TARGET_VIEW_DESC srcViewDesc{};
		this->RenderTargetView->GetDesc(&srcViewDesc);

		WRL::ComPtr<ID3D11Resource> pResSource;
		this->RenderTargetView->GetResource(&pResSource);

		WRL::ComPtr<ID3D11Texture2D> pTexSource;
		pResSource.As(&pTexSource);

		D3D11_TEXTURE2D_DESC srcTextureDesc{};
		pTexSource->GetDesc(&srcTextureDesc);

		D3D11_TEXTURE2D_DESC tmpTextureDesc = srcTextureDesc;
		tmpTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tmpTextureDesc.Usage = D3D11_USAGE_STAGING;
		tmpTextureDesc.BindFlags = 0;
		tmpTextureDesc.MiscFlags = 0;
		tmpTextureDesc.ArraySize = 1;

		WRL::ComPtr<ID3D11Texture2D> pTexTemp;
		HRESULT hr = graphics->GetGraphicsDevice()->CreateTexture2D(
			&tmpTextureDesc, nullptr, &pTexTemp);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
			return GuardianSurface();
		}

		if (srcViewDesc.ViewDimension == D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2DARRAY)
		{
			graphics->GetGraphicsDeviceContext()->CopySubresourceRegion(
				pTexTemp.Get(), 0, 0, 0, 0, pTexSource.Get(), srcViewDesc.Texture2DArray.FirstArraySlice, nullptr);
		}
		else
		{
			graphics->GetGraphicsDeviceContext()->CopyResource(pTexTemp.Get(), pTexSource.Get());
		}

		GuardianSurface surface = GuardianSurface(
			this->RenderTargetWidth, this->RenderTargetHeight);

		D3D11_MAPPED_SUBRESOURCE msr = {};
		hr = graphics->GetGraphicsDeviceContext()->Map(pTexTemp.Get(), 0, D3D11_MAP_READ, 0, &msr);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
			return GuardianSurface();
		}

		auto SrcBytes = (const char*)(msr.pData);
		for (unsigned int y = 0; y < (UINT)this->RenderTargetHeight; y++)
		{
			auto SrcRow = reinterpret_cast<const GuardianColor*>(SrcBytes + msr.RowPitch * size_t(y));
			for (unsigned int x = 0; x < (UINT)this->RenderTargetWidth; x++)
			{
				surface.ReplacePixel(x, y, *(SrcRow + x));
			}
		}

		graphics->GetGraphicsDeviceContext()->Unmap(pTexTemp.Get(), 0);

		return surface;
	}

	std::shared_ptr<GuardianRenderTarget> GuardianRenderTarget::CreateNewRenderTarget(
		std::shared_ptr<GuardianGraphics> graphics)
	{
		return std::make_shared<GuardianRenderTarget>(graphics);
	}
}
