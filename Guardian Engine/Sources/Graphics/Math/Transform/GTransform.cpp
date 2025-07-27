#include "GTransform.h"

namespace GE
{
	GTransform::GTransform()
	{
		this->Position = {};
		this->Rotation = {};
		this->Scale = { 1.0f, 1.0f, 1.0f };
	}

	GTransform::GTransform(const GVector3& position, const GVector3& rotation, const GVector3& scale)
	{
		this->Position = position;
		this->Rotation = rotation;
		this->Scale = scale;
	}

	GTransform::GTransform(const GTransform& other)
	{
		this->Position = other.Position;
		this->Rotation = other.Rotation;
		this->Scale = other.Scale;
	}

	void GTransform::SetPosition(const GVector3& position)
	{
		this->Position = position;
	}

	void GTransform::SetPosition(float x, float y, float z)
	{
		this->Position = { x, y, z };
	}

	void GTransform::SetRotation(const GVector3& rotation)
	{
		this->Rotation = rotation;
	}

	void GTransform::SetRotation(float x, float y, float z)
	{
		this->Rotation = { x, y, z };
	}

	void GTransform::SetScale(const GVector3& scale)
	{
		this->Scale = scale;
	}

	void GTransform::SetScale(float x, float y, float z)
	{
		this->Scale = { x, y, z };
	}

	void GTransform::Translate(const GVector3& translation)
	{
		this->Position += translation;
	}

	void GTransform::Translate(float tx, float ty, float tz)
	{
		this->Position += {tx, ty, tz};
	}

	void GTransform::Rotate(const GVector3& rotation)
	{
		this->Rotation += rotation;
	}

	void GTransform::Rotate(float rx, float ry, float rz)
	{
		this->Rotation += { rx, ry, rz };
	}

	void GTransform::Scaling(const GVector3& scale)
	{
		this->Scale += scale;
	}

	void GTransform::Scaling(float sx, float sy, float sz)
	{
		this->Scale += { sx, sy, sz };
	}

	const GMatrix GTransform::GetTransformMatrix() const noexcept
	{
		return GMatrix::ScalingMatrix(this->Scale) *
			this->GetRotationMatrix() * GMatrix::TranslationMatrix(this->Position);
	}

	const GVector3 GTransform::GetForwardVector() const noexcept
	{
		XMFLOAT3 Forward = { 0.0f, 0.0f, 1.0f };

		XMVECTOR ForwardVector = XMVector3TransformCoord(XMLoadFloat3(&Forward), this->GetRotationMatrix());
		XMStoreFloat3(&Forward, ForwardVector);
		return { Forward.x, Forward.y, Forward.z };
	}

	const GVector3 GTransform::GetBackwardVector() const noexcept
	{
		XMFLOAT3 Backward = { 0.0f, 0.0f, -1.0f };

		XMVECTOR BackwardVector = XMVector3TransformCoord(XMLoadFloat3(&Backward), this->GetRotationMatrix());
		XMStoreFloat3(&Backward, BackwardVector);
		return { Backward.x, Backward.y, Backward.z };
	}

	const GVector3 GTransform::GetRightVector() const noexcept
	{
		XMFLOAT3 Right = { 1.0f, 0.0f, 0.0f };

		XMVECTOR RightVector = XMVector3TransformCoord(XMLoadFloat3(&Right), this->GetRotationMatrix());
		XMStoreFloat3(&Right, RightVector);
		return { Right.x, Right.y, Right.z };
	}

	const GVector3 GTransform::GetLeftVector() const noexcept
	{
		XMFLOAT3 Left = { -1.0f, 0.0f, 0.0f };

		XMVECTOR LeftVector = XMVector3TransformCoord(XMLoadFloat3(&Left), this->GetRotationMatrix());
		XMStoreFloat3(&Left, LeftVector);
		return { Left.x, Left.y, Left.z };
	}

	const GVector3 GTransform::GetUpVector() const noexcept
	{
		XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

		XMVECTOR UpVector = XMVector3TransformCoord(XMLoadFloat3(&Up), this->GetRotationMatrix());
		XMStoreFloat3(&Up, UpVector);
		return { Up.x, Up.y, Up.z };
	}

	const GVector3 GTransform::GetDownVector() const noexcept
	{
		XMFLOAT3 Down = { 0.0f, -1.0f, 0.0f };

		XMVECTOR DownVector = XMVector3TransformCoord(XMLoadFloat3(&Down), this->GetRotationMatrix());
		XMStoreFloat3(&Down, DownVector);
		return { Down.x, Down.y, Down.z };
	}
	
	const GMatrix GTransform::GetRotationMatrix() const noexcept
	{
		return GMatrix::RotationXMatrix(this->Rotation.x) *
			GMatrix::RotationYMatrix(this->Rotation.y) *
			GMatrix::RotationZMatrix(this->Rotation.z);
	}
}
