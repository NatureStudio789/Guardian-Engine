#include "GRay.h"

namespace GE
{
	GRay::GRay()
	{
		this->Origin = {};
		this->Direction = {};
	}

	GRay::GRay(const GVector3& origin, const GVector3& direction)
	{
		this->SetOrigin(origin);
		this->SetDirection(direction);
	}

	GRay::GRay(const GCamera& camera, const GVector2& viewportResolution, const GVector2& mousePosition)
	{
		this->ScreenToRay(camera, viewportResolution, mousePosition);
	}

	GRay::GRay(const GRay& other)
	{
		this->Origin = other.Origin;
		this->Direction = other.Direction;
	}

	void GRay::SetOrigin(const GVector3& origin)
	{
		this->Origin = origin;
	}

	void GRay::SetDirection(const GVector3& direction)
	{
		XMFLOAT3 df = { direction.x, direction.y, direction.z };
		XMVECTOR vdirection = XMVector3Length(XMLoadFloat3(&df));
		XMVECTOR error = XMVectorAbs(vdirection - XMVectorSplatOne());
		assert(XMVector3Less(error, XMVectorReplicate(1e-5f)));

		XMStoreFloat3(&df, XMVector3Normalize(XMLoadFloat3(&df)));
		this->Direction = { df.x, df.y, df.z };
	}

	void GRay::ScreenToRay(const GCamera& camera, const GVector2& viewportResolution, const GVector2& mousePosition)
	{
		static const XMVECTORF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };
		XMVECTOR V = XMVectorSet(mousePosition.x, mousePosition.y, 0.0f, 1.0f);

		XMVECTOR Scale = XMVectorSet(viewportResolution.x * 0.5f, -viewportResolution.y * 0.5f, camera.Projection.FarZ - camera.Projection.NearZ, 1.0f);
		Scale = XMVectorReciprocal(Scale);

		XMVECTOR Offset = XMVectorSet(0.0f, 0.0f, -camera.Projection.NearZ, 0.0f);
		Offset = XMVectorMultiplyAdd(Scale, Offset, D.v);

		XMMATRIX Transform = XMMatrixMultiply(camera.GetViewMatrix(), camera.Projection.GetProjectionMatrix());
		Transform = XMMatrixInverse(nullptr, Transform);

		XMVECTOR Target = XMVectorMultiplyAdd(V, Scale, Offset);
		Target = XMVector3TransformCoord(Target, Transform);

		XMFLOAT3 direction;
		XMFLOAT3 CPF = { camera.Position.x, camera.Position.y, camera.Position.z };
		XMVECTOR CameraPosition = XMLoadFloat3(&CPF);
		XMStoreFloat3(&direction, XMVector3Normalize(Target - CameraPosition));

		this->SetOrigin(camera.Position);
		this->SetDirection({ direction.x, direction.y, direction.z });
	}

	bool GRay::Hit(const BoundingBox& box, float* distance, float maxDistance)
	{
		float dist;
		XMFLOAT3 origin = { this->Origin.x, this->Origin.y, this->Origin.z };
		XMFLOAT3 direction = { this->Direction.x, this->Direction.y, this->Direction.z };
		bool res = box.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), dist);
		if (distance)
			*distance = dist;
		return dist > maxDistance ? false : res;
	}

	const GVector3& GRay::GetOrigin() const noexcept
	{
		return this->Origin;
	}

	const GVector3& GRay::GetDirection() const noexcept
	{
		return this->Direction;
	}
}
