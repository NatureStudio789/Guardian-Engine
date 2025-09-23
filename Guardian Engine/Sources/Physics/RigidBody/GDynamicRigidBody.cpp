#include "GDynamicRigidBody.h"

namespace GE
{
	GDynamicRigidBody::GDynamicRigidBody()
	{
		this->DynamicRigidBodyObject = null;
		this->RigidBodyCollider = std::make_shared<GCollider>();

		this->RigidBodyTransform = {};
		this->RigidBodyMass = 0.0f;
		this->IsKinematic = false;
	}

	GDynamicRigidBody::GDynamicRigidBody(const GDynamicRigidBody& other)
	{
		this->DynamicRigidBodyObject = other.DynamicRigidBodyObject;
		this->RigidBodyCollider = other.RigidBodyCollider;

		this->RigidBodyTransform = other.RigidBodyTransform;
		this->RigidBodyMass = other.RigidBodyMass;
		this->IsKinematic = other.IsKinematic;
	}

	GDynamicRigidBody::~GDynamicRigidBody()
	{
		this->DynamicRigidBodyObject->release();
		this->RigidBodyCollider.reset();

		this->RigidBodyTransform = {};
		this->RigidBodyMass = 0.0f;
		this->IsKinematic = false;
	}

	void GDynamicRigidBody::SetCollider(std::shared_ptr<GCollider> collider)
	{
		this->RigidBodyCollider = collider;
	}

	void GDynamicRigidBody::SetTransform(GTransform transform)
	{
		if (this->DynamicRigidBodyObject)
		{
			PxTransform pxtransform;
			pxtransform.p = PxVec3(transform.Position.x, transform.Position.y, transform.Position.z);
			auto& quaternion = GVector4::EulerToQuaternion(transform.Rotation);
			pxtransform.q = PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);

			if (this->IsKinematic)
			{
				this->DynamicRigidBodyObject->setKinematicTarget(pxtransform);
			}
			else
			{
				this->DynamicRigidBodyObject->setGlobalPose(pxtransform);
			}
		}
		else
		{
			this->RigidBodyTransform = transform;
		}
	}

	void GDynamicRigidBody::SetMass(float mass)
	{
		this->RigidBodyMass = mass;

		if (this->DynamicRigidBodyObject)
		{
			this->DynamicRigidBodyObject->setMass(mass);
		}
	}

	void GDynamicRigidBody::SetKinematic(bool kinematic)
	{
		this->IsKinematic = kinematic;

		if (this->DynamicRigidBodyObject)
		{
			this->DynamicRigidBodyObject->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, this->IsKinematic);
		}
	}

	void GDynamicRigidBody::AddForce(const GVector3& force, ForceType type)
	{
		if (this->DynamicRigidBodyObject)
		{
			this->DynamicRigidBodyObject->addForce(PxVec3(force.x, force.y, force.z), (PxForceMode::Enum)type);
		}
	}

	void GDynamicRigidBody::AddTorque(const GVector3& torque, ForceType type)
	{
		if (this->DynamicRigidBodyObject)
		{
			this->DynamicRigidBodyObject->addTorque(PxVec3(torque.x, torque.y, torque.z), (PxForceMode::Enum)type);
		}
	}

	void GDynamicRigidBody::InitializeRigidBody()
	{
		auto& quaternion = GVector4::EulerToQuaternion(this->RigidBodyTransform.Rotation);
		PxTransform transform;
		transform.p = PxVec3(this->RigidBodyTransform.Position.x, this->RigidBodyTransform.Position.y, this->RigidBodyTransform.Position.z);
		transform.q = PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		this->DynamicRigidBodyObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createRigidDynamic(transform);
		if (!this->DynamicRigidBodyObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create dynamic rigid body!");
		}

		for (auto& shape : this->RigidBodyCollider->ColliderShapeList)
		{
			if (shape && shape->GetShapeObject())
			{
				this->DynamicRigidBodyObject->attachShape(*shape->GetShapeObject());
			}
		}

		this->DynamicRigidBodyObject->setMass(this->RigidBodyMass);
		this->DynamicRigidBodyObject->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, this->IsKinematic);
	}

	PxRigidDynamic* GDynamicRigidBody::GetRigidBodyObject()
	{
		return this->DynamicRigidBodyObject;
	}

	std::shared_ptr<GCollider> GDynamicRigidBody::GetRigidBodyCollider()
	{
		return this->RigidBodyCollider;
	}

	const GTransform& GDynamicRigidBody::GetRigidBodyTransform()
	{
		if (this->DynamicRigidBodyObject)
		{
			PxTransform transform = this->DynamicRigidBodyObject->getGlobalPose();

			this->RigidBodyTransform.Position = GVector3(transform.p.x, transform.p.y, transform.p.z);
			auto& eular = GVector4::QuaternionToEuler(GVector4(transform.q.x, transform.q.y, transform.q.z, transform.q.w));
			this->RigidBodyTransform.Rotation = eular;
		}

		return this->RigidBodyTransform;
	}

	float GDynamicRigidBody::GetRigidBodyMax() const noexcept
	{
		return this->RigidBodyMass;
	}

	bool GDynamicRigidBody::GetKinematic() const noexcept
	{
		return this->IsKinematic;
	}
}
