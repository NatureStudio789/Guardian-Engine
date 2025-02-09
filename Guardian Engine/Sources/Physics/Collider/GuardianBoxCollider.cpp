#include "GuardianBoxCollider.h"

namespace guardian
{
	GuardianBoxCollider::GuardianBoxCollider()
	{
		this->ColliderBoxShape = null;
		this->ColliderProperties = GuardianBoxColliderProperties();
		this->ColliderMaterial = GuardianPhysicsMaterial();
	}

	GuardianBoxCollider::GuardianBoxCollider(const GuardianBoxCollider& other)
	{
		this->ColliderBoxShape = other.ColliderBoxShape;
		this->ColliderMaterial = other.ColliderMaterial;
		this->ColliderProperties = other.ColliderProperties;
	}

	GuardianBoxCollider::~GuardianBoxCollider()
	{
	}

	void GuardianBoxCollider::SetColliderProperties(const GuardianBoxColliderProperties& properties)
	{
		this->ColliderProperties = properties;
	}

	void GuardianBoxCollider::SetColliderMaterial(const GuardianPhysicsMaterial& material)
	{
		this->ColliderMaterial = material;
	}

	void GuardianBoxCollider::InitializeBoxCollider()
	{
		if (!this->ColliderMaterial.GetMaterialObject())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The material of the collider is invalid!");
		}

		this->ColliderBoxShape = GuardianPhysicsEngine::GetPhysicsObject()->createShape(
			PxBoxGeometry({ this->ColliderProperties.BoxHalfsize.x, this->ColliderProperties.BoxHalfsize.y , this->ColliderProperties.BoxHalfsize.z }),
			*this->ColliderMaterial.GetMaterialObject(), true);
		if (!this->ColliderBoxShape)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create collider shape!");
		}
	}

	PxShape* GuardianBoxCollider::GetColliderShape() noexcept
	{
		return this->ColliderBoxShape;
	}

	const GuardianPhysicsMaterial& GuardianBoxCollider::GetColliderMaterial() const noexcept
	{
		return this->ColliderMaterial;
	}

	const GuardianBoxColliderProperties& GuardianBoxCollider::GetColliderProperties() const noexcept
	{
		return this->ColliderProperties;
	}
}
