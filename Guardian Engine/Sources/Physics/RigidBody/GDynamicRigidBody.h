#ifndef _GE_GDYNAMICRIGIDBODY_H_
#define _GE_GDYNAMICRIGIDBODY_H_
#include "../Collider/GCollider.h"

namespace GE
{
	class GUARDIAN_API GDynamicRigidBody
	{
	public:
		enum ForceType
		{
			GE_FORCE_DEFAULT = PxForceMode::eFORCE,
			GE_FORCE_IMPULSE = PxForceMode::eIMPULSE,
			GE_FORCE_VELOCITY = PxForceMode::eVELOCITY_CHANGE,
			GE_FORCE_ACCELERATION = PxForceMode::eACCELERATION
		};

	public:
		GDynamicRigidBody();
		GDynamicRigidBody(const GDynamicRigidBody& other);
		~GDynamicRigidBody();

		void SetCollider(std::shared_ptr<GCollider> collider);
		void SetTransform(GTransform transform);
		void SetMass(float mass);
		void SetKinematic(bool kinematic);

		void AddForce(const GVector3& force, ForceType type);
		void AddTorque(const GVector3& torque, ForceType type);

		void InitializeRigidBody();

		PxRigidDynamic* GetRigidBodyObject();
		std::shared_ptr<GCollider> GetRigidBodyCollider();
		const GTransform& GetRigidBodyTransform();
		float GetRigidBodyMax() const noexcept;
		bool GetKinematic() const noexcept;

	private:
		PxRigidDynamic* DynamicRigidBodyObject;
		std::shared_ptr<GCollider> RigidBodyCollider;

		GTransform RigidBodyTransform;
		float RigidBodyMass;
		bool IsKinematic;
	};
}

#endif