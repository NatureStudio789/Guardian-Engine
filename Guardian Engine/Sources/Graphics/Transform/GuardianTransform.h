#ifndef _GE_GUARDIANTRANSFORM_H_
#define _GE_GUARDIANTRANSFORM_H_
#include "Vector/GuardianVector.h"

namespace guardian
{
	class GUARDIAN_API GuardianTransform
	{
	public:
		GuardianTransform()
		{
			this->Position = { 0.0f, 0.0f, 0.0f };
			this->Rotation = { 0.0f, 0.0f, 0.0f };
			this->QuaternionRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
			this->Scale = { 1.0f, 1.0f, 1.0f };
		}
		GuardianTransform(const GVector3& position, const GVector3& rotation,
			const GVector4& quaternion, const GVector3& scale)
		{
			this->Position = position;
			this->Rotation = rotation;
			this->QuaternionRotation = quaternion;
			this->Scale = scale;
		}
		GuardianTransform(const GuardianTransform& other)
		{
			this->Position = other.Position;
			this->Rotation = other.Rotation;
			this->QuaternionRotation = other.QuaternionRotation;
			this->Scale = other.Scale;
		}

		GVector3& Translate(const GVector3& translation) noexcept
		{
			this->Position += translation;
			return this->Position;
		}
		GVector3& Rotate(const GVector3& rotation) noexcept
		{
			this->Rotation += rotation;
			return this->Rotation;
		}
		GVector3& Scaling(const GVector3& scaling) noexcept
		{
			this->Scale += scaling;
			return this->Scale;
		}

		const XMMATRIX GetTransformMatrix() const noexcept
		{
			return XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z) *
				XMMatrixTranslation(this->Position.x, this->Position.y, this->Position.z) *
				XMMatrixScaling(this->Scale.x, this->Scale.y, this->Scale.z);
		}

		const XMVECTOR GetForwardVector() const noexcept
		{
			XMFLOAT3 Forward = { 0.0f, 0.0f, 1.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Forward), RotationMatrix);
		}

		const XMVECTOR GetBackwardVector() const noexcept
		{
			XMFLOAT3 Backward = { 0.0f, 0.0f, -1.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Backward), RotationMatrix);
		}

		const XMVECTOR GetRightVector() const noexcept
		{
			XMFLOAT3 Right = { 1.0f, 0.0f, 0.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Right), RotationMatrix);
		}

		const XMVECTOR GetLeftVector() const noexcept
		{
			XMFLOAT3 Left = { -1.0f, 0.0f, 0.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Left), RotationMatrix);
		}

		const XMVECTOR GetUpVector() const noexcept
		{
			XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Up), RotationMatrix);
		}

		const XMVECTOR GetDownVector() const noexcept
		{
			XMFLOAT3 Down = { 0.0f, -1.0f, 0.0f };

			XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(XMVectorSet(this->QuaternionRotation.x, this->QuaternionRotation.y,
				this->QuaternionRotation.z, this->QuaternionRotation.w)) *
				XMMatrixRotationRollPitchYaw(this->Rotation.x, this->Rotation.y, this->Rotation.z);

			return XMVector3TransformCoord(XMLoadFloat3(&Down), RotationMatrix);
		}

		GVector3 Position;
		GVector3 Rotation;
		GVector4 QuaternionRotation;
		GVector3 Scale;
	};
}

#endif