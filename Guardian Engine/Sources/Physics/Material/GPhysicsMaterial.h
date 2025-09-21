#ifndef _GE_PHYSICSMATERIAL_H_
#define _GE_PHYSICSMATERIAL_H_
#include "../Context/GPhysicsContextRegistry.h"

namespace GE
{
	class GUARDIAN_API GPhysicsMaterial
	{
	public:
		GPhysicsMaterial();
		GPhysicsMaterial(float staticFriction, float dynamicFriction, float resitution);
		GPhysicsMaterial(const GPhysicsMaterial& other);
		~GPhysicsMaterial();

		void SetStaticFriction(float staticFriction);
		void SetDynamicFriction(float dynamicFriction);
		void SetRestitution(float restitution);

		void InitializeMaterial();

		PxMaterial* GetMaterialObject();

		float GetStaticFriction() const noexcept;
		float GetDynamicFriction() const noexcept;
		float GetRestitution() const noexcept;

	private:
		PxMaterial* MaterialObject;

		float StaticFriction;
		float DynamicFriction;
		float Restitution;
	};
}

#endif