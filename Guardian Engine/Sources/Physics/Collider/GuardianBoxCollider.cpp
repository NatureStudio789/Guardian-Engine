#include "GuardianBoxCollider.h"

namespace guardian
{
	GuardianBoxCollider::GuardianBoxCollider()
	{
		this->ColliderBoxShape = null;
		this->ColliderProperties = GuardianBoxColliderProperties();
		this->ColliderMaterial = GuardianPhysicsMaterial();
	}

	GuardianBoxCollider::GuardianBoxCollider(
		const GuardianBoxColliderProperties& properties, const GuardianPhysicsMaterial& material)
	{
		this->InitializeBoxCollider(properties, material);
	}

	GuardianBoxCollider::GuardianBoxCollider(const GuardianBoxCollider& other)
	{
		this->ColliderBoxShape = other.ColliderBoxShape;
		this->ColliderMaterial = other.ColliderMaterial;
		this->ColliderProperties = other.ColliderProperties;
	}

	GuardianBoxCollider::~GuardianBoxCollider()
	{
		this->ColliderBoxShape->release();
		this->ColliderBoxShape = null;
	}

	void GuardianBoxCollider::InitializeBoxCollider(
		const GuardianBoxColliderProperties& properties, const GuardianPhysicsMaterial& material)
	{
		this->ColliderProperties = properties;
		this->ColliderMaterial = material;

		this->ColliderBoxShape = GuardianPhysicsEngine::GetPhysicsObject()->createShape(
			PxBoxGeometry({ this->ColliderProperties.BoxHalfsize.x, this->ColliderProperties.BoxHalfsize.y , this->ColliderProperties.BoxHalfsize.z }),
			*this->ColliderMaterial.GetMaterialObject(), true);
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
