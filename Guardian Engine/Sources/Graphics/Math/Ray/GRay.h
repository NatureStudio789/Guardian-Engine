#ifndef _GE_GRAY_H_
#define _GE_GRAY_H_
#include "../Camera/GCamera.h"

namespace GE
{
	class GUARDIAN_API GRay
	{
	public:
		GRay();
		GRay(const GVector3& origin, const GVector3& direction);
		GRay(const GCamera& camera, const GVector2& viewportResolution, const GVector2& mousePosition);
		GRay(const GRay& other);

		void SetOrigin(const GVector3& origin);
		void SetDirection(const GVector3& direction);
		void ScreenToRay(const GCamera& camera, const GVector2& viewportResolution, const GVector2& mousePosition);

		bool Hit(const BoundingBox& box, float* distance = null, float maxDistance = FLT_MAX);

		const GVector3& GetOrigin() const noexcept;
		const GVector3& GetDirection() const noexcept;

	private:
		GVector3 Origin;
		GVector3 Direction;
	};
}

#endif