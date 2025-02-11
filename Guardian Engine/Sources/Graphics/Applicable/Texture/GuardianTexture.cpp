#include "GuardianTexture.h"

namespace guardian
{
	GuardianTexture::GuardianTexture()
	{
		this->TextureAppliedSlot = 0;
	}

	GuardianTexture::GuardianTexture(
		std::shared_ptr<GuardianGraphics> graphics, const GuardianSurface& surface, int index)
	{
		this->InitializeTexture(graphics, surface, index);
	}

	GuardianTexture::GuardianTexture(const GuardianTexture& other)
	{
		this->TextureObject = other.TextureObject;
		this->TextureResource = other.TextureResource;
		this->TextureAppliedSlot = other.TextureAppliedSlot;
	}

	GuardianTexture::~GuardianTexture()
	{
		this->TextureAppliedSlot = 0;
	}

	void GuardianTexture::SetTextureAppliedSlot(int index)
	{
		this->TextureAppliedSlot = index;
	}

	void GuardianTexture::InitializeTexture(
		std::shared_ptr<GuardianGraphics> graphics, const GuardianSurface& surface, int index)
	{
		this->SetTextureAppliedSlot(index);

		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = surface.GetSurfaceWidth();
		TextureDesc.Height = surface.GetSurfaceHeight();
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA TextureResourceData;
		ZeroMemory(&TextureResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		TextureResourceData.pSysMem = surface.GetBufferData();
		TextureResourceData.SysMemPitch = surface.GetSurfaceWidth() * sizeof(GuardianColor);

		HRESULT hr = graphics->GetGraphicsDevice()->CreateTexture2D(
			&TextureDesc, &TextureResourceData, &this->TextureObject);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView;
		ZeroMemory(&ShaderResourceView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		ShaderResourceView.Format = TextureDesc.Format;
		ShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		ShaderResourceView.Texture2D.MipLevels = 1;
		ShaderResourceView.Texture2D.MostDetailedMip = 0;

		hr = graphics->GetGraphicsDevice()->CreateShaderResourceView(
			this->TextureObject.Get(), &ShaderResourceView, this->TextureResource.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianTexture::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->PSSetShaderResources(this->TextureAppliedSlot, 1, this->TextureResource.GetAddressOf());
	}

	WRL::ComPtr<ID3D11Texture2D> GuardianTexture::GetTextureObject()
	{
		return this->TextureObject;
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianTexture::GetTextureResource()
	{
		return this->TextureResource;
	}

	std::shared_ptr<GuardianTexture> GuardianTexture::CreateNewTexture(std::shared_ptr<GuardianGraphics> graphics, 
		const GuardianSurface& surface, int index)
	{
		return std::make_shared<GuardianTexture>(graphics, surface, index);
	}
}
