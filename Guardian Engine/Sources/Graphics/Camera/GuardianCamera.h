#ifndef _GE_GUARDIANCAMERA_H_
#define _GE_GUARDIANCAMERA_H_
#include "Projection/GuardianProjection.h"

namespace guardian
{
	class GUARDIAN_API GuardianCamera
	{
	public:
		GuardianCamera()
		{
			this->Position = { 0.0f, 0.0f, 0.0f };
			this->Direction = { 0.0f, 0.0f, 0.0f };
			this->Projection = GuardianPerspectiveProjection();
		}
		GuardianCamera(const GVector3& position, const GVector3& direction,
			const GuardianPerspectiveProjection& projection)
		{
			this->Position = position;
			this->Direction = direction;
			this->Projection = projection;
		}
		GuardianCamera(const GuardianCamera& other)
		{
			this->Position = other.Position;
			this->Direction = other.Direction;
			this->Projection = other.Projection;
		}
		~GuardianCamera() = default;

		const XMMATRIX GetViewMatrix() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x, 
				this->Direction.y, this->Direction.z);

			XMVECTOR CameraTarget = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), RotationMatrix);
			XMFLOAT3 CameraPosition = XMFLOAT3(this->Position.x, this->Position.y, this->Position.z);
			CameraTarget += XMLoadFloat3(&CameraPosition);

			XMVECTOR UpDirection = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), RotationMatrix);

			return XMMatrixLookAtLH(XMLoadFloat3(&CameraPosition), CameraTarget, UpDirection);
		}

		const XMMATRIX GetProjectionMatrix() const noexcept
		{
			return this->Projection.GetProjectionMatrix();
		}

		const XMVECTOR GetForwardVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Forward = { 0.0f, 0.0f, 1.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Forward), RotationMatrix);
		}

		const XMVECTOR GetBackwardVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Backward = { 0.0f, 0.0f, -1.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Backward), RotationMatrix);
		}

		const XMVECTOR GetLeftVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Left = { -1.0f, 0.0f, 0.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Left), RotationMatrix);
		}

		const XMVECTOR GetRightVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Right = { 1.0f, 0.0f, 0.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Right), RotationMatrix);
		}

		const XMVECTOR GetUpVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Up), RotationMatrix);
		}

		const XMVECTOR GetDownVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Down = { 0.0f, -1.0f, 0.0f };

			return XMVector3TransformCoord(XMLoadFloat3(&Down), RotationMatrix);
		}

		GVector3 Position;
		GVector3 Direction;
		GuardianPerspectiveProjection Projection;
	};
}

#endif