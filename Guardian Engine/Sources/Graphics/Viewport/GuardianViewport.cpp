#include "GuardianViewport.h"
#include "../GuardianGraphics.h"

namespace guardian
{
	GuardianViewport::GuardianViewport(const GuardianViewport& other)
	{
		this->ViewportObject = other.ViewportObject;
	}

	GuardianViewport::GuardianViewport(const D3D11_VIEWPORT& viewport) noexcept
	{
		this->ViewportObject = viewport;
	}

	GuardianViewport::GuardianViewport(const GVector2& position, const GVector2& size) noexcept
	{
		this->InitializeViewport(position, size);
	}

	void GuardianViewport::InitializeViewport(const GVector2& position, const GVector2& size) noexcept
	{
		this->ViewportObject.TopLeftX = position.x;
		this->ViewportObject.TopLeftY = position.y;
		this->ViewportObject.Width = size.x;
		this->ViewportObject.Height = size.y;
		this->ViewportObject.MaxDepth = 1.0f;
		this->ViewportObject.MinDepth = 0.0f;
	}

	void GuardianViewport::ApplyViewport(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->RSSetViewports(1, &this->ViewportObject);
	}

	const D3D11_VIEWPORT& GuardianViewport::GetViewportObject() const noexcept
	{
		return this->ViewportObject;
	}
}
