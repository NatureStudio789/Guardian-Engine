#ifndef _GE_GUARDIANSTATICRIGIDBODY_H_
#define _GE_GUARDIANSTATICRIGIDBODY_H_
#include "../Collider/GuardianBoxCollider.h"

namespace guardian
{
	class GUARDIAN_API GuardianStaticRigidBody
	{
	public:
		GuardianStaticRigidBody();
		GuardianStaticRigidBody(const GuardianTransform& transform);
		GuardianStaticRigidBody(const GuardianStaticRigidBody& other);
		~GuardianStaticRigidBody();

		void InitializeStaticRigidBody(const GuardianTransform& transform);
		void SetRigidBodyCollider(std::shared_ptr<GuardianCollider> collider);

		const GuardianTransform GetRigidBodyTransform() noexcept;
		PxRigidStatic* GetRigidBodyObject() noexcept;
		std::shared_ptr<GuardianCollider> GetRigidBodyCollider() noexcept;

	private:
		PxRigidStatic* RigidBodyObject;
		std::shared_ptr<GuardianCollider> RigidBodyCollider;
		GuardianTransform RigidBodyTransform;
	};
}

#endif