#ifndef _GE_GUARDIANPHYSICSMATERIAL_H_
#define _GE_GUARDIANPHYSICSMATERIAL_H_
#include "../GuardianPhysicsEngine.h"

namespace guardian
{
	class GUARDIAN_API GuardianPhysicsMaterial : public GuardianSerializable
	{
	public:
		GuardianPhysicsMaterial();
		GuardianPhysicsMaterial(float staticFriction, float dynamicFriction, float restitution);
		GuardianPhysicsMaterial(const GuardianPhysicsMaterial& other);
		~GuardianPhysicsMaterial();

		void InitializePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution);
		void ResetStaticFriction(float staticFriction);
		void ResetDynamicFriction(float dynamicFriction);
		void ResetRestitution(float restituion);

		void Serialize(const GString& filePath) override;
		void Deserialize(const GString& filePath) override;

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