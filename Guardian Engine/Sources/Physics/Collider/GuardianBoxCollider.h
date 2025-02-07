#ifndef _GE_GUARDIANBOXCOLLIDER_H_
#define _GE_GUARDIANBOXCOLLIDER_H_
#include "GuardianCollider.h"

namespace guardian
{
	struct GUARDIAN_API GuardianBoxColliderProperties
	{
		GVector3 BoxHalfsize = GVector3(0.5f, 0.5f, 0.5f);
	};

	class GUARDIAN_API GuardianBoxCollider : public GuardianCollider
	{
	public:
		GuardianBoxCollider();
		GuardianBoxCollider(const GuardianBoxColliderProperties& properties, 
			const GuardianPhysicsMaterial& material);
		GuardianBoxCollider(const GuardianBoxCollider& other);
		~GuardianBoxCollider();

		void InitializeBoxCollider(const GuardianBoxColliderProperties& properties,
			const GuardianPhysicsMaterial& material);

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