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

		void Translate(const GVector3& translation)
		{
			this->Position += translation;
		}

		void Rotate(const GVector3& rotation)
		{
			this->Direction += rotation;
		}

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

		const GVector3 GetForwardVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Forward = { 0.0f, 0.0f, 1.0f };
			XMVECTOR ForwardVector = XMVector3TransformCoord(XMLoadFloat3(&Forward), RotationMatrix);
			XMStoreFloat3(&Forward, ForwardVector);

			return GVector3(Forward.x, Forward.y, Forward.z);
		}

		const GVector3 GetBackwardVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Backward = { 0.0f, 0.0f, -1.0f };
			XMVECTOR BackwardVector = XMVector3TransformCoord(XMLoadFloat3(&Backward), RotationMatrix);
			XMStoreFloat3(&Backward, BackwardVector);

			return GVector3(Backward.x, Backward.y, Backward.z);
		}

		const GVector3 GetLeftVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Left = { -1.0f, 0.0f, 0.0f };
			XMVECTOR LeftVector = XMVector3TransformCoord(XMLoadFloat3(&Left), RotationMatrix);
			XMStoreFloat3(&Left, LeftVector);

			return GVector3(Left.x, Left.y, Left.z);
		}

		const GVector3 GetRightVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Right = { 1.0f, 0.0f, 0.0f };
			XMVECTOR RightVector = XMVector3TransformCoord(XMLoadFloat3(&Right), RotationMatrix);
			XMStoreFloat3(&Right, RightVector);

			return GVector3(Right.x, Right.y, Right.z);
		}

		const GVector3 GetUpVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

			XMVECTOR UpVector = XMVector3TransformCoord(XMLoadFloat3(&Up), RotationMatrix);
			XMStoreFloat3(&Up, UpVector);

			return GVector3(Up.x, Up.y, Up.z);
		}

		const GVector3 GetDownVector() const noexcept
		{
			XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(this->Direction.x,
				this->Direction.y, this->Direction.z);

			XMFLOAT3 Down = { 0.0f, -1.0f, 0.0f };
			XMVECTOR DownVector = XMVector3TransformCoord(XMLoadFloat3(&Down), RotationMatrix);
			XMStoreFloat3(&Down, DownVector);

			return GVector3(Down.x, Down.y, Down.z);
		}

		GVector3 Position;
		GVector3 Direction;
		GuardianPerspectiveProjection Projection;
	};
}

#endif