#include "GuardianRenderTarget.h"
#include "../Surface/GuardianSurface.h"
#include "../GuardianGraphics.h"

namespace GE
{
	GuardianRenderTarget::GuardianRenderTarget(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->InitializeRenderTarget(graphics);
	}

	GuardianRenderTarget::GuardianRenderTarget(std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		this->InitializeRenderTarget(graphics, width, height);
	}

	void GuardianRenderTarget::InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics)
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		graphics->GetGraphicsSwapChain()->GetDesc(&SwapChainDesc);
		this->RenderTargetWidth = SwapChainDesc.BufferDesc.Width;
		this->RenderTargetHeight = SwapChainDesc.BufferDesc.Height;

		HRESULT hr = graphics->GetGraphicsSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)this->RenderTargetTexture.GetAddressOf());
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

	void GuardianRenderTarget::InitializeRenderTarget(std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		this->RenderTargetWidth = width;
		this->RenderTargetHeight = height;

		D3D11_TEXTURE2D_DESC RenderTargetTextureDesc;
		ZeroMemory(&RenderTargetTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		RenderTargetTextureDesc.Width = this->RenderTargetWidth;
		RenderTargetTextureDesc.Height = this->RenderTargetHeight;
		RenderTargetTextureDesc.MipLevels = 1;
		RenderTargetTextureDesc.ArraySize = 1;
		RenderTargetTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		RenderTargetTextureDesc.SampleDesc.Count = 1;
		RenderTargetTextureDesc.SampleDesc.Quality = 0;
		RenderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		RenderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		HRESULT hr = graphics->GetGraphicsDevice()->CreateTexture2D(&RenderTargetTextureDesc,
			null, this->RenderTargetTexture.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		hr = graphics->GetGraphicsDevice()->CreateShaderResourceView(this->RenderTargetTexture.Get(),
			null, this->RenderTargetShaderResource.GetAddressOf());
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

		const float color[] = { clearColor.x, clearColor.y, clearColor.z, 1.0f };
		graphics->GetGraphicsDeviceContext()->ClearRenderTargetView(this->RenderTargetView.Get(), color);
	}

	WRL::ComPtr<ID3D11RenderTargetView> GuardianRenderTarget::GetRenderTargetView() noexcept
	{
		return this->RenderTargetView;
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianRenderTarget::GetRenderTargetShaderResource() noexcept
	{
		return this->RenderTargetShaderResource;
	}

	WRL::ComPtr<ID3D11Texture2D> GuardianRenderTarget::GetRenderTargetTexture() noexcept
	{
		return this->RenderTargetTexture;
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

	std::shared_ptr<GuardianRenderTarget> GuardianRenderTarget::CreateNewRenderTarget(
		std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		return std::make_shared<GuardianRenderTarget>(graphics, width, height);
	}
}
