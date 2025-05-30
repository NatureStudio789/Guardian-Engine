#include "GuardianStaticRigidBody.h"

namespace GE
{
	GuardianStaticRigidBody::GuardianStaticRigidBody()
	{
		this->RigidBodyObject = null;
		this->RigidBodyCollider = null;
		this->RigidBodyTransform = GuardianTransform();
	}

	GuardianStaticRigidBody::GuardianStaticRigidBody(const GuardianStaticRigidBody& other)
	{
		this->RigidBodyObject = other.RigidBodyObject;
		this->RigidBodyCollider = other.RigidBodyCollider;
	}

	GuardianStaticRigidBody::~GuardianStaticRigidBody()
	{
		if (this->RigidBodyObject)
		{
			this->RigidBodyObject->release();
		}
		this->RigidBodyObject = null;
	}

	void GuardianStaticRigidBody::InitializeStaticRigidBody()
	{
		this->RigidBodyObject = GuardianPhysicsEngine::GetPhysicsObject()->createRigidStatic(
			PxTransform(this->RigidBodyTransform.Position.x, 
				this->RigidBodyTransform.Position.y, this->RigidBodyTransform.Position.z,
				PxQuat(this->RigidBodyTransform.Quaternion.x, 
					this->RigidBodyTransform.Quaternion.y, 
					this->RigidBodyTransform.Quaternion.z, 
					this->RigidBodyTransform.Quaternion.w)));

		this->RigidBodyObject->attachShape(*this->RigidBodyCollider->GetColliderShape());
	}

	void GuardianStaticRigidBody::SetRigidBodyTransform(const GuardianTransform& transform)
	{
		this->RigidBodyTransform = transform;

		XMVECTOR translation, scale, quaternion;
		XMMatrixDecompose(&scale, &quaternion, &translation, this->RigidBodyTransform.GetTransformMatrix());
		XMFLOAT4 Quaternion;
		XMStoreFloat4(&Quaternion, quaternion);
		this->RigidBodyTransform.Rotation = GVector3(0.0f, 0.0f, 0.0f);
		this->RigidBodyTransform.Quaternion = GVector4(Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w);
	}

	void GuardianStaticRigidBody::SetRigidBodyCollider(std::shared_ptr<GuardianCollider> collider)
	{
		this->RigidBodyCollider = collider;
	}

	const GuardianTransform& GuardianStaticRigidBody::GetRigidBodyTransform() noexcept
	{
		PxTransform pxtransform = this->RigidBodyObject->getGlobalPose();
		this->RigidBodyTransform.Position = GVector3(pxtransform.p.x, pxtransform.p.y, pxtransform.p.z);
		this->RigidBodyTransform.Quaternion = GVector4(pxtransform.q.x, pxtransform.q.y, pxtransform.q.z, pxtransform.q.w);

		return this->RigidBodyTransform;
	}

	PxRigidStatic* GuardianStaticRigidBody::GetRigidBodyObject() noexcept
	{
		return this->RigidBodyObject;
	}

	std::shared_ptr<GuardianCollider> GuardianStaticRigidBody::GetRigidBodyCollider() noexcept
	{
		return this->RigidBodyCollider;
	}
}
