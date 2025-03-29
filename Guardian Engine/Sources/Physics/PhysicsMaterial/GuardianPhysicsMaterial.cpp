#include "GuardianPhysicsMaterial.h"

namespace GE
{
	GuardianPhysicsMaterial::GuardianPhysicsMaterial()
	{
		this->MaterialObject = null;
		this->StaticFriction = 0.5f;
		this->DynamicFriction = 0.5f;
		this->Restitution = 0.6f;
	}

	GuardianPhysicsMaterial::GuardianPhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		this->SetStaticFriction(staticFriction);
		this->SetDynamicFriction(dynamicFriction);
		this->SetRestitution(restitution);
		this->MaterialObject = null;
	}

	GuardianPhysicsMaterial::GuardianPhysicsMaterial(const GuardianPhysicsMaterial& other)
	{
		this->MaterialObject = other.MaterialObject;
		this->StaticFriction = other.StaticFriction;
		this->DynamicFriction = other.DynamicFriction;
		this->Restitution = other.Restitution;
	}

	GuardianPhysicsMaterial::~GuardianPhysicsMaterial()
	{
	}

	void GuardianPhysicsMaterial::SetStaticFriction(float staticFriction)
	{
		this->StaticFriction = staticFriction;
	}

	void GuardianPhysicsMaterial::SetDynamicFriction(float dynamicFriction)
	{
		this->DynamicFriction = dynamicFriction;
	}

	void GuardianPhysicsMaterial::SetRestitution(float restituion)
	{
		this->Restitution = restituion;
	}

	void GuardianPhysicsMaterial::InitializePhysicsMaterial()
	{
		this->MaterialObject = GuardianPhysicsEngine::CreatePhysicsMaterial(
			this->StaticFriction, this->DynamicFriction, this->Restitution);
		if (!this->MaterialObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics material!");
		}
	}

	PxMaterial* GuardianPhysicsMaterial::GetMaterialObject() noexcept
	{
		return this->MaterialObject;
	}

	const float GuardianPhysicsMaterial::GetStaticFriction() const noexcept
	{
		return this->StaticFriction;
	}

	const float GuardianPhysicsMaterial::GetDynamicFriction() const noexcept
	{
		return this->DynamicFriction;
	}

	const float GuardianPhysicsMaterial::GetRestitution() const noexcept
	{
		return this->Restitution;
	}
}
