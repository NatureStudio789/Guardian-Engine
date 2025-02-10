#include "GuardianPickingRay.h"

namespace guardian
{
	GuardianPickingRay::GuardianPickingRay()
	{
		this->RayOrigin = GVector3(0.0f, 0.0f, 0.0f);
		this->RayDirection = GVector3(0.0f, 0.0f, 0.0f);
	}

	GuardianPickingRay::GuardianPickingRay(
		const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding)
	{
		this->UpdatePickingRay(viewMatrix, projectionMatrix, viewportBounding);
	}

	GuardianPickingRay::GuardianPickingRay(const GuardianPickingRay& other)
	{
		this->RayOrigin = other.RayOrigin;
		this->RayDirection = other.RayDirection;
	}

	GuardianPickingRay::~GuardianPickingRay()
	{

	}

	void GuardianPickingRay::UpdatePickingRay(
		const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding)
	{
		int mouseX, mouseY;
		GuardianInput::GetMousePosition(mouseX, mouseY);

		XMVECTOR ScreenPosition = XMVectorSet((float)mouseX, (float)mouseY, 0.0f, 1.0f);

		XMVECTOR NDCPosition = XMVectorSet((ScreenPosition.m128_f32[0] - viewportBounding.TopLeftX) / viewportBounding.Width * 2.0f - 1.0f,
			(ScreenPosition.m128_f32[1] - viewportBounding.TopLeftY) / viewportBounding.Height * 2.0f - 1.0f, 0.0f, 1.0f);

		XMMATRIX InverseProjection = XMMatrixInverse(null, projectionMatrix);
		XMVECTOR ViewPosition = XMVector3TransformCoord(NDCPosition, InverseProjection);
		XMMATRIX InverseView = XMMatrixInverse(null, viewMatrix);

		XMVECTOR Origin = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), InverseView);
		XMFLOAT3 FOrigin;
		XMStoreFloat3(&FOrigin, Origin);
		this->RayOrigin = GVector3(FOrigin.x, FOrigin.y, FOrigin.z);
		XMVECTOR Direction = XMVector3TransformNormal(ViewPosition, InverseView);
		XMFLOAT3 FDirection;
		XMStoreFloat3(&FDirection, Direction);
		this->RayDirection = GVector3(FDirection.x, FDirection.y, FDirection.z);
	}

	const GVector3& GuardianPickingRay::GetRayOrigin() const noexcept
	{
		return this->RayDirection;
	}

	const GVector3& GuardianPickingRay::GetRayDirection() const noexcept
	{
		return this->RayDirection;
	}
}
