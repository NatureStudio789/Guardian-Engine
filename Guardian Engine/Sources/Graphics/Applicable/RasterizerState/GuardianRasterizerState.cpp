#include "GuardianRasterizerState.h"

namespace GE
{
	GuardianRasterizerState::GuardianRasterizerState()
	{
		this->RasterizerStateFillMode = GE_FILL_SOLID;
		this->RasterizerStateCullMode = GE_CULL_NONE;
	}

	GuardianRasterizerState::GuardianRasterizerState(const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		this->InitializeRasterizerState(fillMode, cullMode);
	}

	GuardianRasterizerState::GuardianRasterizerState(const GuardianRasterizerState& other)
	{
		this->RasterizerStateObject = other.RasterizerStateObject;
		this->RasterizerStateFillMode = other.RasterizerStateFillMode;
		this->RasterizerStateCullMode = other.RasterizerStateCullMode;
	}

	void GuardianRasterizerState::InitializeRasterizerState(const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		this->RasterizerStateFillMode = fillMode;
		this->RasterizerStateCullMode = cullMode;

		D3D11_RASTERIZER_DESC RasterizerStateDesc;
		ZeroMemory(&RasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));

		RasterizerStateDesc.FillMode = (D3D11_FILL_MODE)this->RasterizerStateFillMode;
		RasterizerStateDesc.CullMode = (D3D11_CULL_MODE)this->RasterizerStateCullMode;
		HRESULT hr = GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDevice()->CreateRasterizerState(
			&RasterizerStateDesc, this->RasterizerStateObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianRasterizerState::Apply()
	{
		GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->RSSetState(this->RasterizerStateObject.Get());
	}

	WRL::ComPtr<ID3D11RasterizerState> GuardianRasterizerState::GetRasterizerStateObject() noexcept
	{
		return this->RasterizerStateObject;
	}

	const GuardianFillMode& GuardianRasterizerState::GetRasterizerStateFillMode() const noexcept
	{
		return this->RasterizerStateFillMode;
	}

	const GuardianCullMode& GuardianRasterizerState::GetRasterizerStateCullMode() const noexcept
	{
		return this->RasterizerStateCullMode;
	}

	std::shared_ptr<GuardianRasterizerState> GuardianRasterizerState::CreateNewRasterizerState(
		const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		return std::make_shared<GuardianRasterizerState>(fillMode, cullMode);
	}
}
