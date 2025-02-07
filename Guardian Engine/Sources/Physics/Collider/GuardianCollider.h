#ifndef _GE_GUARDIANCOLLIDER_H_
#define _GE_GUARDIANCOLLIDER_H_
#include "../PhysicsMaterial/GuardianPhysicsMaterial.h"

namespace guardian
{
	class GUARDIAN_API GuardianCollider
	{
	public:
		virtual PxShape* GetColliderShape() noexcept = 0;
	};
}

#endif