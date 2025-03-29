#ifndef _GE_GUARDIANPHYSICSMATERIAL_H_
#define _GE_GUARDIANPHYSICSMATERIAL_H_
#include "../GuardianPhysicsEngine.h"

namespace GE
{
	class GUARDIAN_API GuardianPhysicsMaterial
	{
	public:
		GuardianPhysicsMaterial();
		GuardianPhysicsMaterial(
			float staticFriction, float dynamicFriction, float restitution);
		GuardianPhysicsMaterial(const GuardianPhysicsMaterial& other);
		~GuardianPhysicsMaterial();

		void SetStaticFriction(float staticFriction);
		void SetDynamicFriction(float dynamicFriction);
		void SetRestitution(float restituion);
		void InitializePhysicsMaterial();

		PxMaterial* GetMaterialObject() noexcept;

		const float GetStaticFriction() const noexcept;
		const float GetDynamicFriction() const noexcept;
		const float GetRestitution() const noexcept;

	private:
		PxMaterial* MaterialObject;
		float StaticFriction;
		float DynamicFriction;
		float Restitution;
	};
}

#endif