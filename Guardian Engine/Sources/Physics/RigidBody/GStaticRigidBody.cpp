#include "GStaticRigidBody.h"

namespace GE
{
	GStaticRigidBody::GStaticRigidBody()
	{
		this->StaticRigidBodyObject = null;
		
		this->RigidBodyCollider = null;
		this->RigidBodyTransform = {};
	}

	GStaticRigidBody::GStaticRigidBody(const GStaticRigidBody& other)
	{
		this->StaticRigidBodyObject = other.StaticRigidBodyObject;

		this->RigidBodyCollider = other.RigidBodyCollider;
		this->RigidBodyTransform = other.RigidBodyTransform;
	}
	
	GStaticRigidBody::~GStaticRigidBody()
	{
		this->StaticRigidBodyObject->release();

		this->RigidBodyCollider.reset();
		this->RigidBodyTransform = {};
	}

	void GStaticRigidBody::SetCollider(std::shared_ptr<GCollider> collider)
	{
		this->RigidBodyCollider = collider;
	}

	void GStaticRigidBody::SetTransform(GTransform transform)
	{
		if (this->StaticRigidBodyObject)
		{
			PxTransform pxtransform;
			pxtransform.p = PxVec3(transform.Position.x, transform.Position.y, transform.Position.z);
			auto& quaternion = GVector4::EulerToQuaternion(transform.Rotation);
			pxtransform.q = PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);

			this->StaticRigidBodyObject->setGlobalPose(pxtransform);
		}
		else
		{
			this->RigidBodyTransform = transform;
		}
	}

	void GStaticRigidBody::InitializeRigidBody()
	{
		auto& quaternion = GVector4::EulerToQuaternion(this->RigidBodyTransform.Rotation);
		PxTransform transform;
		transform.p = PxVec3(this->RigidBodyTransform.Position.x, this->RigidBodyTransform.Position.y, this->RigidBodyTransform.Position.z);
		transform.q = PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		this->StaticRigidBodyObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createRigidStatic(transform);
		if (!this->StaticRigidBodyObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create static rigid body!");
		}

		for (auto& shape : this->RigidBodyCollider->ColliderShapeList)
		{
			if (shape && shape->GetShapeObject())
			{
				this->StaticRigidBodyObject->attachShape(*shape->GetShapeObject());
			}
		}
	}

	PxRigidStatic* GStaticRigidBody::GetRigidBodyObject()
	{
		return this->StaticRigidBodyObject;
	}

	std::shared_ptr<GCollider> GStaticRigidBody::GetRigidBodyCollider() 
	{
		return this->RigidBodyCollider;
	}
	
	const GTransform& GStaticRigidBody::GetRigidBodyTransform()
	{
		return this->RigidBodyTransform;
	}
}
