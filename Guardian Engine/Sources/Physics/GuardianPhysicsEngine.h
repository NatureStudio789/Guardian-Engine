#ifndef _GE_GUARDIANPHYSICSENGINE_H_
#define _GE_GUARDIANPHYSICSENGINE_H_
#include "../Graphics/PickingRay/GuardianPickingRay.h"

namespace GE
{
#define GE_GRAVITY_EARTH GVector3(0.0f, -9.81f, 0.0f)

	class GUARDIAN_API GuardianPhysicsEngine
	{
	public:
		static void InitializePhysicsEngine();
		
		static PxMaterial* CreatePhysicsMaterial(float staticFriction,
			float dynamicFriction, float restitution);

		static PxPhysics* GetPhysicsObject() noexcept;

	private:
		static PxDefaultAllocator PhysicsAllocator;
		static PxDefaultErrorCallback PhysicsErrorCallback;
		static PxFoundation* PhysicsFoundation;
		static PxPvd* PhysicsDebugger;
		static PxPhysics* PhysicsObject;
		static PxDefaultCpuDispatcher* PhysicsCpuDispatcher;

		friend class GuardianScene;
	};
}

#endif