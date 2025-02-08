#ifndef _GE_GUARDIANBOXCOLLIDER_H_
#define _GE_GUARDIANBOXCOLLIDER_H_
#include "GuardianCollider.h"

namespace guardian
{
	struct GUARDIAN_API GuardianBoxColliderProperties
	{
		GuardianBoxColliderProperties()
		{
			this->BoxHalfsize = GVector3(0.5f, 0.5f, 0.5f);
		}
		GuardianBoxColliderProperties(const GVector3& halfSize)
		{
			this->BoxHalfsize = halfSize;
		}

		GVector3 BoxHalfsize = GVector3(0.5f, 0.5f, 0.5f);
	};

	class GUARDIAN_API GuardianBoxCollider : public GuardianCollider
	{
	public:
		GuardianBoxCollider();
		GuardianBoxCollider(const GuardianBoxCollider& other);
		~GuardianBoxCollider();

		void SetColliderProperties(const GuardianBoxColliderProperties& properties) override;
		void SetColliderMaterial(const GuardianPhysicsMaterial& material) override;
		void InitializeBoxCollider();

		PxShape* GetColliderShape() noexcept override;
		const GuardianBoxColliderProperties& GetColliderProperties() const noexcept;
		const GuardianPhysicsMaterial& GetColliderMaterial() const noexcept;

	private:
		PxShape* ColliderBoxShape;

		GuardianBoxColliderProperties ColliderProperties;
		GuardianPhysicsMaterial ColliderMaterial;
	};
}

#endif