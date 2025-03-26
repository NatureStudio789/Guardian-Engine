#ifndef _GE_GUARDIANPICKINGRAY_H_
#define _GE_GUARDIANPICKINGRAY_H_
#include "../Camera/GuardianCamera.h"

namespace guardian
{
	class GUARDIAN_API GuardianPickingRay
	{
	public:
		GuardianPickingRay();
		GuardianPickingRay(const GVector2& mousePosition, const GVector3& cameraPosition, 
			const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding);
		GuardianPickingRay(const GuardianPickingRay& other);
		~GuardianPickingRay();

		void UpdatePickingRay(const GVector2& mousePosition, const GVector3& cameraPosition, 
			const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const D3D11_VIEWPORT& viewportBounding);

		const GVector3& GetRayOrigin() const noexcept;
		const GVector3& GetRayDirection() const noexcept;

	private:
		GVector3 RayOrigin;
		GVector3 RayDirection;
	};
}

#endif