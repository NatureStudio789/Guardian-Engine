#include "GPhysicsMaterial.h"

namespace GE
{
	GPhysicsMaterial::GPhysicsMaterial()
	{
		this->MaterialObject = null;

		this->StaticFriction = 0.5f;
		this->DynamicFriction = 0.5f;
		this->Restitution = 0.8f;
	}

	GPhysicsMaterial::GPhysicsMaterial(float staticFriction, float dynamicFriction, float resitution)
	{
		this->SetStaticFriction(staticFriction);
		this->SetDynamicFriction(dynamicFriction);
		this->SetRestitution(resitution);
	}

	GPhysicsMaterial::GPhysicsMaterial(const GPhysicsMaterial& other)
	{
		this->MaterialObject = other.MaterialObject;

		this->StaticFriction = other.StaticFriction;
		this->DynamicFriction = other.DynamicFriction;
		this->Restitution = other.Restitution;
	}

	GPhysicsMaterial::~GPhysicsMaterial()
	{
		this->MaterialObject->release();

		this->StaticFriction = 0.5f;
		this->DynamicFriction = 0.5f;
		this->Restitution = 0.8f;
	}

	void GPhysicsMaterial::SetStaticFriction(float staticFriction)
	{
		this->StaticFriction = staticFriction;
	}

	void GPhysicsMaterial::SetDynamicFriction(float dynamicFriction)
	{
		this->DynamicFriction = dynamicFriction;
	}

	void GPhysicsMaterial::SetRestitution(float restitution)
	{
		this->Restitution = restitution;
	}

	void GPhysicsMaterial::InitializeMaterial()
	{
		this->MaterialObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->
			createMaterial(this->StaticFriction, this->DynamicFriction, this->Restitution);
		if (!this->MaterialObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics material!");
		}
	}

	PxMaterial* GPhysicsMaterial::GetMaterialObject()
	{
		return this->MaterialObject;
	}

	float GPhysicsMaterial::GetStaticFriction() const noexcept
	{
		return this->StaticFriction;
	}

	float GPhysicsMaterial::GetDynamicFriction() const noexcept
	{
		return this->DynamicFriction;
	}

	float GPhysicsMaterial::GetRestitution() const noexcept
	{
		return this->Restitution;
	}
}
