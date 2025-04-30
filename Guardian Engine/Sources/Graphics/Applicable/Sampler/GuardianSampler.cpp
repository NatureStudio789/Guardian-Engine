#include "GuardianSampler.h"

namespace GE
{
	GuardianSampler::GuardianSampler()
	{
		this->SamplerFilter = GE_FILTER_MIN_MAG_MIP_POINT;
		this->SamplerAppliedSlot = 0;
	}

	GuardianSampler::GuardianSampler(GuardianSamplerFilter filter, int index)
	{
		this->InitializeSampler(filter, index);
	}

	GuardianSampler::GuardianSampler(const GuardianSampler& other)
	{
		this->SamplerState = other.SamplerState;
		this->SamplerFilter = other.SamplerFilter;
		this->SamplerAppliedSlot = other.SamplerAppliedSlot;
	}

	void GuardianSampler::InitializeSampler(GuardianSamplerFilter filter, int index)
	{
		this->SamplerFilter = filter;
		this->SamplerAppliedSlot = index;

		D3D11_SAMPLER_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		SamplerDesc.Filter = (D3D11_FILTER)this->SamplerFilter;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

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

	const GuardianSamplerFilter& GuardianSampler::GetSamplerFilter() const noexcept
	{
		return this->SamplerFilter;
	}

	std::shared_ptr<GuardianSampler> GuardianSampler::CreateNewSampler(GuardianSamplerFilter filter, int index)
	{
		return std::make_shared<GuardianSampler>(filter, index);
	}
}
