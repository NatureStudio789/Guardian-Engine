#include "GuardianSampler.h"

namespace GE
{
	GuardianSampler::GuardianSampler()
	{
		this->SamplerProperties = {};
		this->SamplerAppliedSlot = 0;
	}

	GuardianSampler::GuardianSampler(Properties properties, int index)
	{
		this->InitializeSampler(properties, index);
	}

	GuardianSampler::GuardianSampler(const GuardianSampler& other)
	{
		this->SamplerState = other.SamplerState;
		this->SamplerProperties = other.SamplerProperties;
		this->SamplerAppliedSlot = other.SamplerAppliedSlot;
	}

	void GuardianSampler::InitializeSampler(Properties properties, int index)
	{
		this->SamplerProperties = properties;
		this->SamplerAppliedSlot = index;

		D3D11_SAMPLER_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		SamplerDesc.Filter = (D3D11_FILTER)this->SamplerProperties.SamplerFilter;
		SamplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)this->SamplerProperties.AddressU;
		SamplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)this->SamplerProperties.AddressV;
		SamplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)this->SamplerProperties.AddressW;
		if (SamplerProperties.SamplerFilter == GE_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT)
		{
			SamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
			SamplerDesc.MaxAnisotropy = 1;
		}
		if (SamplerProperties.AddressU == GE_TEXTURE_ADDRESS_BORDER && 
			SamplerProperties.AddressV == GE_TEXTURE_ADDRESS_BORDER && 
			SamplerProperties.AddressW == GE_TEXTURE_ADDRESS_BORDER)
		{
			SamplerDesc.MinLOD = 0.0f;
			SamplerDesc.MaxLOD = 0.0f;
			SamplerDesc.BorderColor[0] = 0.0f;
			SamplerDesc.BorderColor[1] = 0.0f;
			SamplerDesc.BorderColor[2] = 0.0f;
			SamplerDesc.BorderColor[3] = 1.0f;
		}

		HRESULT hr = GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDevice()->CreateSamplerState(&SamplerDesc, this->SamplerState.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianSampler::Apply()
	{
		GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->PSSetSamplers(this->SamplerAppliedSlot, 1, this->SamplerState.GetAddressOf());
	}

	WRL::ComPtr<ID3D11SamplerState> GuardianSampler::GetSamplerState() noexcept
	{
		return this->SamplerState;
	}

	const GuardianSampler::Properties& GuardianSampler::GetSamplerProperties() const noexcept
	{
		return this->SamplerProperties;
	}

	const GuardianSampler::Filter& GuardianSampler::GetSamplerFilter() const noexcept
	{
		return this->SamplerProperties.SamplerFilter;
	}

	const GuardianSampler::AddressMode& GuardianSampler::GetSamplerAdressU() const noexcept
	{
		return this->SamplerProperties.AddressU;
	}

	const GuardianSampler::AddressMode& GuardianSampler::GetSamplerAdressV() const noexcept
	{
		return this->SamplerProperties.AddressV;
	}

	const GuardianSampler::AddressMode& GuardianSampler::GetSamplerAdressW() const noexcept
	{
		return this->SamplerProperties.AddressW;
	}

	std::shared_ptr<GuardianSampler> GuardianSampler::CreateNewSampler(Properties properties, int index)
	{
		return std::make_shared<GuardianSampler>(properties, index);
	}
}
