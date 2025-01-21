#include "GuardianRasterizerState.h"

namespace guardian
{
	GuardianRasterizerState::GuardianRasterizerState()
	{
		this->RasterizerStateFillMode = GE_FILL_SOLID;
		this->RasterizerStateCullMode = GE_CULL_NONE;
	}

	GuardianRasterizerState::GuardianRasterizerState(
		std::shared_ptr<GuardianGraphics> graphics, const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		this->InitializeRasterizerState(graphics, fillMode, cullMode);
	}

	GuardianRasterizerState::GuardianRasterizerState(const GuardianRasterizerState& other)
	{
		this->RasterizerStateObject = other.RasterizerStateObject;
		this->RasterizerStateFillMode = other.RasterizerStateFillMode;
		this->RasterizerStateCullMode = other.RasterizerStateCullMode;
	}

	void GuardianRasterizerState::InitializeRasterizerState(
		std::shared_ptr<GuardianGraphics> graphics, const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		this->RasterizerStateFillMode = fillMode;
		this->RasterizerStateCullMode = cullMode;

		D3D11_RASTERIZER_DESC RasterizerStateDesc;
		ZeroMemory(&RasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));

		RasterizerStateDesc.FillMode = (D3D11_FILL_MODE)this->RasterizerStateFillMode;
		RasterizerStateDesc.CullMode = (D3D11_CULL_MODE)this->RasterizerStateCullMode;
		HRESULT hr = graphics->GetGraphicsDevice()->CreateRasterizerState(
			&RasterizerStateDesc, this->RasterizerStateObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianRasterizerState::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->RSSetState(this->RasterizerStateObject.Get());
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
		std::shared_ptr<GuardianGraphics> graphics, const GuardianFillMode& fillMode, const GuardianCullMode& cullMode)
	{
		return std::make_shared<GuardianRasterizerState>(graphics, fillMode, cullMode);
	}
}
