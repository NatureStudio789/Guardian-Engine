#include "GuardianSphereCollider.h"

namespace GE
{
	GuardianSphereCollider::GuardianSphereCollider()
	{
		this->ColliderShape = null;
		this->ColliderProperties = GuardianSphereColliderProperties();
		this->ColliderMaterial = GuardianPhysicsMaterial();
	}

	GuardianSphereCollider::GuardianSphereCollider(const GuardianSphereCollider& other)
	{
		this->ColliderShape = other.ColliderShape;
		this->ColliderProperties = other.ColliderProperties;
		this->ColliderMaterial = other.ColliderMaterial;
	}

	GuardianSphereCollider::~GuardianSphereCollider()
	{
	}

	void GuardianSphereCollider::SetColliderProperties(const GuardianSphereColliderProperties& properties)
	{
		this->ColliderProperties = properties;
	}

	void GuardianSphereCollider::SetColliderMaterial(const GuardianPhysicsMaterial& material)
	{
		this->ColliderMaterial = material;
		if (!this->ColliderMaterial.GetMaterialObject())
		{
			this->ColliderMaterial.InitializePhysicsMaterial();
		}
	}

	void GuardianSphereCollider::InitializeSphereCollider()
	{
		if (!this->ColliderMaterial.GetMaterialObject())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The material of this collider is invalid!");
		}

		this->ColliderShape = GuardianPhysicsEngine::GetPhysicsObject()->createShape(
			PxSphereGeometry(this->ColliderProperties.Radius), *this->ColliderMaterial.GetMaterialObject(), true);
		if (!this->ColliderShape)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create collider shape!");
		}
	}

	PxShape* GuardianSphereCollider::GetColliderShape() noexcept
	{
		return this->ColliderShape;
	}

	const GuardianSphereColliderProperties& GuardianSphereCollider::GetColliderProperties() const noexcept
	{
		return this->ColliderProperties;
	}

	const GuardianPhysicsMaterial& GuardianSphereCollider::GetColliderMaterial() const noexcept
	{
		return this->ColliderMaterial;
	}
}
