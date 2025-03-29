#ifndef _GE_GUARDIANCAPSULECOLLIDER_H_
#define _GE_GUARDIANCAPSULECOLLIDER_H_
#include "GuardianSphereCollider.h"

namespace GE
{
	struct GUARDIAN_API GuardianCapsuleColliderProperties
	{
		GuardianCapsuleColliderProperties()
		{
			this->Radius = 0.5f;
			this->HalfHeight = 1.0f;
		}
		GuardianCapsuleColliderProperties(const float radius, const float halfHeight)
		{
			this->Radius = radius;
			this->HalfHeight = halfHeight;
		}

		float Radius, HalfHeight;
	};

	class GUARDIAN_API GuardianCapsuleCollider : public GuardianCollider
	{
	public:
		GuardianCapsuleCollider();
		GuardianCapsuleCollider(const GuardianCapsuleCollider& other);
		~GuardianCapsuleCollider();

		void SetColliderMaterial(const GuardianPhysicsMaterial& material) override;
		void SetColliderProperties(const GuardianCapsuleColliderProperties& properties);
		void InitializeCapsuleCollider();

		PxShape* GetColliderShape() noexcept override;
		const GuardianCapsuleColliderProperties& GetColliderProperties() const noexcept;
		const GuardianPhysicsMaterial& GetColliderMaterial() const noexcept;

	private:
		PxShape* ColliderCapsuleShape;

		GuardianCapsuleColliderProperties ColliderProperties;
		GuardianPhysicsMaterial ColliderMaterial;
	};
}

#endif