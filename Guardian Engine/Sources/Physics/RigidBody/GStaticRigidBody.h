#ifndef _GE_GSTATICRIGIDBODY_H_
#define _GE_GSTATICRIGIDBODY_H_
#include "../Collider/GCollider.h"

namespace GE
{
	class GUARDIAN_API GStaticRigidBody
	{
	public:
		GStaticRigidBody();
		GStaticRigidBody(const GStaticRigidBody& other);
		~GStaticRigidBody();

		void SetCollider(std::shared_ptr<GCollider> collider);
		void SetTransform(GTransform transform);

		void InitializeRigidBody();

		PxRigidStatic* GetRigidBodyObject();
		std::shared_ptr<GCollider> GetRigidBodyCollider();
		const GTransform& GetRigidBodyTransform();

	private:
		PxRigidStatic* StaticRigidBodyObject;
		std::shared_ptr<GCollider> RigidBodyCollider;

		GTransform RigidBodyTransform;
	};
}

#endif