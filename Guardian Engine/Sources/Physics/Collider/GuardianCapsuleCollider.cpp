#include "GuardianCapsuleCollider.h"

namespace guardian
{
	GuardianCapsuleCollider::GuardianCapsuleCollider()
	{
		this->ColliderCapsuleShape = null;
		this->ColliderProperties = GuardianCapsuleColliderProperties();
		this->ColliderMaterial = GuardianPhysicsMaterial();
	}

	GuardianCapsuleCollider::GuardianCapsuleCollider(const GuardianCapsuleCollider& other)
	{
		this->ColliderCapsuleShape = other.ColliderCapsuleShape;
		this->ColliderProperties = other.ColliderProperties;
		this->ColliderMaterial = other.ColliderMaterial;
	}

	GuardianCapsuleCollider::~GuardianCapsuleCollider()
	{

	}

	void GuardianCapsuleCollider::SetColliderMaterial(const GuardianPhysicsMaterial& material)
	{
		this->ColliderMaterial = material;
	}

	void GuardianCapsuleCollider::SetColliderProperties(const GuardianCapsuleColliderProperties& properties)
	{
		this->ColliderProperties = properties;
	}

	void GuardianCapsuleCollider::InitializeCapsuleCollider()
	{
		if (!this->ColliderMaterial.GetMaterialObject())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The material of the collider is invalid!");
		}

		PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
		this->ColliderCapsuleShape = GuardianPhysicsEngine::GetPhysicsObject()->createShape(
			PxCapsuleGeometry(this->ColliderProperties.Radius, this->ColliderProperties.HalfHeight),
			*this->ColliderMaterial.GetMaterialObject(), true);
		if (!this->ColliderCapsuleShape)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create collider shape!");
		}
		this->ColliderCapsuleShape->setLocalPose(relativePose);
	}

	PxShape* GuardianCapsuleCollider::GetColliderShape() noexcept
	{
		return this->ColliderCapsuleShape;
	}

	const GuardianCapsuleColliderProperties& GuardianCapsuleCollider::GetColliderProperties() const noexcept
	{
		return this->ColliderProperties;
	}

	const GuardianPhysicsMaterial& GuardianCapsuleCollider::GetColliderMaterial() const noexcept
	{
		return this->ColliderMaterial;
	}
}
