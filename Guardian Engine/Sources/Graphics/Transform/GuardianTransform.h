#ifndef _GE_GUARDIANTRANSFORM_H_
#define _GE_GUARDIANTRANSFORM_H_
#include "AABB/GuardianAABB.h"

namespace guardian
{
	class GUARDIAN_API GuardianTransform
	{
	public:
		GuardianTransform()
		{
			this->Position = { 0.0f, 0.0f, 0.0f };
			this->Rotation = { 0.0f, 0.0f, 0.0f };
			this->Quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
			this->Scale = { 1.0f, 1.0f, 1.0f };
		}
		GuardianTransform(const GVector3& position, const GVector3& rotation, 
			const GVector4& quaternion, const GVector3& scale)
		{
			this->Position = position;
			this->Rotation = rotation;
			this->Quaternion = quaternion;
			this->Scale = scale;
		}
		GuardianTransform(const GuardianTransform& other)
		{
			this->Position = other.Position;
			this->Rotation = other.Rotation;
			this->Quaternion = other.Quaternion;
			this->Scale = other.Scale;
		}

		void Translate(const GVector3& translation) noexcept
		{
			this->Position += translation;
		}
		void Rotate(const GVector3& rotation) noexcept
		{
			this->Rotation += rotation;
		}
		void Scaling(const GVector3& scaling) noexcept
		{
			this->Scale += scaling;
		}

		const XMMATRIX GetTransformMatrix() const noexcept
		{
			return XMMatrixScaling(this->Scale.x, this->Scale.y, this->Scale.z) *
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) * 
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI) *
				XMMatrixTranslation(this->Position.x, this->Position.y, this->Position.z);
		}

		const GVector3 GetForwardVector() const noexcept
		{
			XMFLOAT3 Forward = { 0.0f, 0.0f, 1.0f };

			XMMATRIX RotationMatrix = 
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR ForwardVector = XMVector3TransformCoord(XMLoadFloat3(&Forward), RotationMatrix);
			XMStoreFloat3(&Forward, ForwardVector);
			return GVector3(Forward.x, Forward.y, Forward.z);
		}

		const GVector3 GetBackwardVector() const noexcept
		{
			XMFLOAT3 Backward = { 0.0f, 0.0f, -1.0f };

			XMMATRIX RotationMatrix = 
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR BackwardVector = XMVector3TransformCoord(XMLoadFloat3(&Backward), RotationMatrix);
			XMStoreFloat3(&Backward, BackwardVector);
			return GVector3(Backward.x, Backward.y, Backward.z);
		}

		const GVector3 GetRightVector() const noexcept
		{
			XMFLOAT3 Right = { 1.0f, 0.0f, 0.0f };

			XMMATRIX RotationMatrix =
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR RightVector = XMVector3TransformCoord(XMLoadFloat3(&Right), RotationMatrix);
			XMStoreFloat3(&Right, RightVector);
			return GVector3(Right.x, Right.y, Right.z);
		}

		const GVector3 GetLeftVector() const noexcept
		{
			XMFLOAT3 Left = { -1.0f, 0.0f, 0.0f };

			XMMATRIX RotationMatrix =
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR LeftVector = XMVector3TransformCoord(XMLoadFloat3(&Left), RotationMatrix);
			XMStoreFloat3(&Left, LeftVector);
			return GVector3(Left.x, Left.y, Left.z);
		}

		const GVector3 GetUpVector() const noexcept
		{
			XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

			XMMATRIX RotationMatrix =
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR UpVector = XMVector3TransformCoord(XMLoadFloat3(&Up), RotationMatrix);
			XMStoreFloat3(&Up, UpVector);
			return GVector3(Up.x, Up.y, Up.z);
		}

		const GVector3 GetDownVector() const noexcept
		{
			XMFLOAT3 Down = { 0.0f, -1.0f, 0.0f };

			XMMATRIX RotationMatrix =
				XMMatrixRotationQuaternion(XMVectorSet(this->Quaternion.x, this->Quaternion.y, this->Quaternion.z, this->Quaternion.w)) *
				XMMatrixRotationX((this->Rotation.x / 360.0f) * XM_2PI) *
				XMMatrixRotationY((this->Rotation.y / 360.0f) * XM_2PI) *
				XMMatrixRotationZ((this->Rotation.z / 360.0f) * XM_2PI);

			XMVECTOR DownVector = XMVector3TransformCoord(XMLoadFloat3(&Down), RotationMatrix);
			XMStoreFloat3(&Down, DownVector);
			return GVector3(Down.x, Down.y, Down.z);
		}

		GVector3 Position;
		GVector3 Rotation;
		GVector4 Quaternion;
		GVector3 Scale;
	};
}

#endif