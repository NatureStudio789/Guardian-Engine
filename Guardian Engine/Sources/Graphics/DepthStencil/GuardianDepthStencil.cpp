#include "GuardianDepthStencil.h"
#include "../GuardianGraphics.h"

namespace guardian
{
	GuardianDepthStencil::GuardianDepthStencil()
	{

	}

	GuardianDepthStencil::GuardianDepthStencil(std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		this->InitializeDepthStencil(graphics, width, height);
	}

	GuardianDepthStencil::GuardianDepthStencil(const GuardianDepthStencil& other)
	{
		this->DepthStencilState = other.DepthStencilState;
		this->DepthStencilView = other.DepthStencilView;
		this->DepthStencilShaderResource = other.DepthStencilShaderResource;
	}

	GuardianDepthStencil::~GuardianDepthStencil()
	{
	}

	void GuardianDepthStencil::InitializeDepthStencil(std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		D3D11_TEXTURE2D_DESC DepthStencilBufferDesc;
		ZeroMemory(&DepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
		DepthStencilBufferDesc.Width = width;
		DepthStencilBufferDesc.Height = height;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.ArraySize = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		DepthStencilBufferDesc.CPUAccessFlags = 0;
		DepthStencilBufferDesc.MiscFlags = 0;

		WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
		HRESULT hr = graphics->GetGraphicsDevice()->CreateTexture2D(&DepthStencilBufferDesc,
			null, DepthStencilBuffer.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC DepthStencilShaderResourceDesc;
		ZeroMemory(&DepthStencilShaderResourceDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		DepthStencilShaderResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthStencilShaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		DepthStencilShaderResourceDesc.Texture2D.MipLevels = 1;

		hr = graphics->GetGraphicsDevice()->CreateShaderResourceView(DepthStencilBuffer.Get(),
			&DepthStencilShaderResourceDesc, this->DepthStencilShaderResource.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
		ZeroMemory(&DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = graphics->GetGraphicsDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(),
			&DepthStencilViewDesc, this->DepthStencilView.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
		ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		DepthStencilDesc.DepthEnable = true;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = graphics->GetGraphicsDevice()->CreateDepthStencilState(&DepthStencilDesc,
			this->DepthStencilState.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	WRL::ComPtr<ID3D11DepthStencilState> GuardianDepthStencil::GetDepthStencilState() noexcept
	{
		return this->DepthStencilState;
	}

	WRL::ComPtr<ID3D11DepthStencilView> GuardianDepthStencil::GetDepthStencilView() noexcept
	{
		return this->DepthStencilView;
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianDepthStencil::GetDepthStencilShaderResource() noexcept
	{
		return this->DepthStencilShaderResource;
	}

	std::shared_ptr<GuardianDepthStencil> GuardianDepthStencil::CreateNewDepthStencil(
		std::shared_ptr<GuardianGraphics> graphics, int width, int height)
	{
		return std::make_shared<GuardianDepthStencil>(graphics, width, height);
	}
}
