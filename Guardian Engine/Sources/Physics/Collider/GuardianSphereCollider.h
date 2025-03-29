#ifndef _GE_GUARDIANSPHERECOLLIDER_H_
#define _GE_GUARDIANSPHERECOLLIDER_H_
#include "GuardianBoxCollider.h"

namespace GE
{
	struct GUARDIAN_API GuardianSphereColliderProperties
	{
		GuardianSphereColliderProperties()
		{
			this->Radius = 0.5f;
		}
		GuardianSphereColliderProperties(const float radius)
		{
			this->Radius = radius;
		}

		float Radius;
	};

	class GUARDIAN_API GuardianSphereCollider : public GuardianCollider
	{
	public:
		GuardianSphereCollider();
		GuardianSphereCollider(const GuardianSphereCollider& other);
		~GuardianSphereCollider();

		void SetColliderProperties(const GuardianSphereColliderProperties& properties);
		void SetColliderMaterial(const GuardianPhysicsMaterial& material);
		void InitializeSphereCollider();

		PxShape* GetColliderShape() noexcept override;
		const GuardianSphereColliderProperties& GetColliderProperties() const noexcept;
		const GuardianPhysicsMaterial& GetColliderMaterial() const noexcept;

	private:
		PxShape* ColliderShape;

		GuardianSphereColliderProperties ColliderProperties;
		GuardianPhysicsMaterial ColliderMaterial;
	};
}

#endif