#include "GuardianPickingRay.h"

namespace GE
{
	GuardianPickingRay::GuardianPickingRay()
	{
		this->RayOrigin = GVector3(0.0f, 0.0f, 0.0f);
		this->RayDirection = GVector3(0.0f, 0.0f, 0.0f);
	}

	GuardianPickingRay::GuardianPickingRay(const GVector2& mousePosition, const GVector3& cameraPosition,
		const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding)
	{
		this->UpdatePickingRay(mousePosition, cameraPosition, viewMatrix, projectionMatrix, viewportBounding);
	}

	GuardianPickingRay::GuardianPickingRay(const GuardianPickingRay& other)
	{
		this->RayOrigin = other.RayOrigin;
		this->RayDirection = other.RayDirection;
	}

	GuardianPickingRay::~GuardianPickingRay()
	{

	}

	void GuardianPickingRay::UpdatePickingRay(const GVector2& mousePosition, const GVector3& cameraPosition,
		const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding)
	{
		XMVECTOR MouseClipPosition = { (mousePosition.x / viewportBounding.Width) * 2.0f - 1.0f,
			((mousePosition.y / viewportBounding.Height) * 2.0f - 1.0f) * -1.0f, -1.0f, 1.0f };

		auto InverseProjection = XMMatrixInverse(null, projectionMatrix);
		auto InverseView = XMMatrixInverse(null, viewMatrix);

		XMVECTOR Ray = XMVector4Transform(MouseClipPosition, InverseProjection);
		XMVECTOR DirectionVector = XMVector3Transform(Ray, InverseView);
		XMFLOAT3 Dir;
		XMStoreFloat3(&Dir, DirectionVector);

		this->RayDirection = { Dir.x, Dir.y, Dir.z };
		this->RayOrigin = cameraPosition;
	}

	const GVector3& GuardianPickingRay::GetRayOrigin() const noexcept
	{
		return this->RayOrigin;
	}

	const GVector3& GuardianPickingRay::GetRayDirection() const noexcept
	{
		return this->RayDirection;
	}
}
