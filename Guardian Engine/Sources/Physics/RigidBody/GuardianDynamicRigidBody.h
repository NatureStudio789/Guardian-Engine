#ifndef _GE_GUARDIANDYNAMICRIGIDBODY_H_
#define _GE_GUARDIANDYNAMICRIGIDBODY_H_
#include "GuardianStaticRigidBody.h"

namespace GE
{
	enum GuardianForceMode
	{
		GE_FORCE,
		GE_IMPULSE,
		GE_VELOCITY_CHANGE,
		GE_ACCELERATION
	};

	class GUARDIAN_API GuardianDynamicRigidBody
	{
	public:
		GuardianDynamicRigidBody();
		GuardianDynamicRigidBody(const GuardianDynamicRigidBody& other);
		~GuardianDynamicRigidBody();

		void SetRigidBodyTransform(const GuardianTransform& transform);
		void SetRigidBodyCollider(std::shared_ptr<GuardianCollider> collider);
		void SetRigidBodyDensity(float density);
		void InitializeDynamicRigidBody();

		void AddForce(const GVector3& force, const GuardianForceMode& mode = GE_FORCE);
		void AddTorque(const GVector3& torque, const GuardianForceMode& mode = GE_FORCE);

		const GuardianTransform& GetRigidBodyTransform() noexcept;
		PxRigidDynamic* GetRigidBodyObject() noexcept;
		std::shared_ptr<GuardianCollider> GetRigidBodyCollider() noexcept;
		const float GetRigidBodyDensity() const noexcept;

	private:
		PxRigidDynamic* RigidBodyObject;
		std::shared_ptr<GuardianCollider> RigidBodyCollider;
		GuardianTransform RigidBodyTransform;
		float RigidBodyDensity;
	};
}

#endif