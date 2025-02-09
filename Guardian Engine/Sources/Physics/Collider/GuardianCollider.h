#ifndef _GE_GUARDIANCOLLIDER_H_
#define _GE_GUARDIANCOLLIDER_H_
#include "../PhysicsMaterial/GuardianPhysicsMaterial.h"

namespace guardian
{
	class GuardianPhysicsMaterial;

	class GUARDIAN_API GuardianCollider
	{
	public:
		virtual void SetColliderMaterial(const GuardianPhysicsMaterial& material) = 0;

		virtual PxShape* GetColliderShape() noexcept = 0;
	};
}

#endif