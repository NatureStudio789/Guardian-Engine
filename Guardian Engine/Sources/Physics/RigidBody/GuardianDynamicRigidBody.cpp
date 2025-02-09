#include "GuardianDynamicRigidBody.h"

namespace guardian
{
	GuardianDynamicRigidBody::GuardianDynamicRigidBody()
	{
		this->RigidBodyObject = null;
		this->RigidBodyCollider = null;
		this->RigidBodyTransform = GuardianTransform();
		this->RigidBodyDensity = 1.0f;
	}

	GuardianDynamicRigidBody::GuardianDynamicRigidBody(const GuardianDynamicRigidBody& other)
	{
		this->RigidBodyObject = other.RigidBodyObject;
		this->RigidBodyCollider = other.RigidBodyCollider;
		this->RigidBodyTransform = other.RigidBodyTransform;
		this->RigidBodyDensity = other.RigidBodyDensity;
	}

	GuardianDynamicRigidBody::~GuardianDynamicRigidBody()
	{
		if (this->RigidBodyObject)
		{
			this->RigidBodyObject->release();
		}
		this->RigidBodyObject = null;
	}

	void GuardianDynamicRigidBody::InitializeDynamicRigidBody()
	{
		this->RigidBodyObject = GuardianPhysicsEngine::GetPhysicsObject()->createRigidDynamic(
			PxTransform(this->RigidBodyTransform.Position.x,
				this->RigidBodyTransform.Position.y, this->RigidBodyTransform.Position.z,
				PxQuat(this->RigidBodyTransform.Quaternion.x, 
					this->RigidBodyTransform.Quaternion.y, 
					this->RigidBodyTransform.Quaternion.z, 
					this->RigidBodyTransform.Quaternion.w)));
		if (!this->RigidBodyObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create dynamic rigid object!");
		}

		if (!this->RigidBodyCollider)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot attach invalid collider to rigid body!");
		}
		this->RigidBodyObject->attachShape(*this->RigidBodyCollider->GetColliderShape());
		PxRigidBodyExt::updateMassAndInertia(*this->RigidBodyObject, this->RigidBodyDensity);
	}

	void GuardianDynamicRigidBody::SetRigidBodyTransform(const GuardianTransform& transform)
	{
		this->RigidBodyTransform = transform;

		XMVECTOR translation, scale, quaternion;
		XMMatrixDecompose(&scale, &quaternion, &translation, this->RigidBodyTransform.GetTransformMatrix());
		XMFLOAT4 Quaternion;
		XMStoreFloat4(&Quaternion, quaternion);
		this->RigidBodyTransform.Rotation = GVector3(0.0f, 0.0f, 0.0f);
		this->RigidBodyTransform.Quaternion = GVector4(Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w);
	}

	void GuardianDynamicRigidBody::SetRigidBodyCollider(std::shared_ptr<GuardianCollider> collider)
	{
		this->RigidBodyCollider = collider;
	}

	void GuardianDynamicRigidBody::SetRigidBodyDensity(float density)
	{
		this->RigidBodyDensity = density;
	}

	const GuardianTransform& GuardianDynamicRigidBody::GetRigidBodyTransform() noexcept
	{
		PxTransform pxtransform = this->RigidBodyObject->getGlobalPose();
		this->RigidBodyTransform.Position = GVector3(pxtransform.p.x, pxtransform.p.y, pxtransform.p.z);
		this->RigidBodyTransform.Quaternion = GVector4(pxtransform.q.x, pxtransform.q.y, pxtransform.q.z, pxtransform.q.w);

		return this->RigidBodyTransform;
	}

	PxRigidDynamic* GuardianDynamicRigidBody::GetRigidBodyObject() noexcept
	{
		return this->RigidBodyObject;
	}

	std::shared_ptr<GuardianCollider> GuardianDynamicRigidBody::GetRigidBodyCollider() noexcept
	{
		return this->RigidBodyCollider;
	}

	const float GuardianDynamicRigidBody::GetRigidBodyDensity() const noexcept
	{
		return this->RigidBodyDensity;
	}
}
