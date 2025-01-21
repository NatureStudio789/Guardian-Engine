#ifndef _GE_GUARDIANPROJECTION_H_
#define _GE_GUARDIANPROJECTION_H_
#include "../../Transform/GuardianTransform.h"

namespace guardian
{
	class GUARDIAN_API GuardianPerspectiveProjection
	{
	public:
		GuardianPerspectiveProjection()
		{
			this->FOV = 60.0f;
			this->Aspect = 16.0f / 9.0f;
			this->NearZ = 0.1f;
			this->FarZ = 1000.0f;
		}
		GuardianPerspectiveProjection(float fov, float aspect, float nearZ, float farZ)
		{
			this->FOV = fov;
			this->Aspect = aspect;
			this->NearZ = nearZ;
			this->FarZ = farZ;
		}
		GuardianPerspectiveProjection(const GuardianPerspectiveProjection& other)
		{
			this->FOV = other.FOV;
			this->Aspect = other.Aspect;
			this->NearZ = other.NearZ;
			this->FarZ = other.FarZ;
		}
		~GuardianPerspectiveProjection() = default;

		const XMMATRIX GetProjectionMatrix() const noexcept
		{
			return XMMatrixPerspectiveFovLH((this->FOV / 360.0f) * XM_2PI, this->Aspect, this->NearZ, this->FarZ);
		}

		float FOV;
		float Aspect;
		float NearZ;
		float FarZ;
	};

	class GUARDIAN_API GuardianOrthographicsProjection
	{
	public:
		GuardianOrthographicsProjection()
		{
			this->FrustumWidth = 1536.0f;
			this->FrustumHeight = 864.0f;
			this->NearZ = 0.1f;
			this->FarZ = 1000.0f;
		}
		GuardianOrthographicsProjection(float width, float height, float nearZ, float farZ)
		{
			this->FrustumWidth = width;
			this->FrustumHeight = height;
			this->NearZ = nearZ;
			this->FarZ = farZ;
		}
		GuardianOrthographicsProjection(const GuardianOrthographicsProjection& other)
		{
			this->FrustumWidth = other.FrustumWidth;
			this->FrustumHeight = other.FrustumHeight;
			this->NearZ = other.NearZ;
			this->FarZ = other.FarZ;
		}
		~GuardianOrthographicsProjection() = default;

		const XMMATRIX GetProjectionMatrix() const noexcept
		{
			return XMMatrixOrthographicOffCenterLH(0.0f, this->FrustumWidth, this->FrustumHeight, 0.0f,
				this->NearZ, this->FarZ);
		}

		float FrustumWidth;
		float FrustumHeight;
		float NearZ;
		float FarZ;
	};
}

#endif