#ifndef _GE_GUARDIANCOLLIDER_H_
#define _GE_GUARDIANCOLLIDER_H_
#include "../PhysicsMaterial/GuardianPhysicsMaterial.h"

namespace guardian
{
	struct GuardianBoxColliderProperties;
	class GuardianPhysicsMaterial;

	class GUARDIAN_API GuardianCollider
	{
	public:
		virtual void SetColliderProperties(const GuardianBoxColliderProperties& properties) = 0;
		virtual void SetColliderMaterial(const GuardianPhysicsMaterial& material) = 0;

		virtual PxShape* GetColliderShape() noexcept = 0;
	};
}

#endif