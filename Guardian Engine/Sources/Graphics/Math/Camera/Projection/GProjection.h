#ifndef _GE_GPROJECTION_H_
#define _GE_GPROJECTION_H_
#include "../../Transform/GTransform.h"

namespace GE
{
	class GUARDIAN_API GPerspectiveProjection
	{
	public:
		GPerspectiveProjection()
		{
			this->FOV = 60.0f;
			this->Aspect = 16.0f / 9.0f;
			this->NearZ = 0.1f;
			this->FarZ = 1000.0f;
		}
		GPerspectiveProjection(float fov, float aspect, float nearZ, float farZ)
		{
			this->FOV = fov;
			this->Aspect = aspect;
			this->NearZ = nearZ;
			this->FarZ = farZ;
		}
		GPerspectiveProjection(const GPerspectiveProjection& other)
		{
			this->FOV = other.FOV;
			this->Aspect = other.Aspect;
			this->NearZ = other.NearZ;
			this->FarZ = other.FarZ;
		}

		const GMatrix GetProjectionMatrix() const noexcept
		{
			return GMatrix::PerspectiveMatrix((this->FOV / 360.0f) * XM_2PI, this->Aspect, this->NearZ, this->FarZ);
		}

		float FOV;
		float Aspect;
		float NearZ;
		float FarZ;
	};

	class GUARDIAN_API GOrthographicsProjection
	{
	public:
		GOrthographicsProjection()
		{
			this->ViewWidth = 50.0f;
			this->ViewHeight = 50.0f;
			this->NearZ = 0.1f;
			this->FarZ = 20.0f;
		}
		GOrthographicsProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
		{
			this->ViewWidth = viewWidth;
			this->ViewHeight = viewHeight;
			this->NearZ = nearZ;
			this->FarZ = farZ;
		}
		GOrthographicsProjection(const GOrthographicsProjection& other)
		{
			this->ViewWidth = other.ViewWidth;
			this->ViewHeight = other.ViewHeight;
			this->NearZ = other.NearZ;
			this->FarZ = other.FarZ;
		}

		const GMatrix GetProjectionMatrix() const noexcept
		{
			return GMatrix::OrthographicsMatrix(this->ViewWidth, this->ViewHeight, this->NearZ, this->FarZ);
		}

		float ViewWidth;
		float ViewHeight;
		float NearZ;
		float FarZ;
	};
}

#endif