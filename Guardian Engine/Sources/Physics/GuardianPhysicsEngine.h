#ifndef _GE_GUARDIANPHYSICSENGINE_H_
#define _GE_GUARDIANPHYSICSENGINE_H_
#include "../Graphics/Renderer/GuardianRenderer.h"

namespace guardian
{
#define GE_GRAVITY_EARTH GVector3(0.0f, 9.81f, 0.0f)

	class GUARDIAN_API GuardianPhysicsEngine
	{
	public:
		static void InitializePhysicsEngine(const GVector3& gravity);

		static void UpdatePhysicsSimulation(GuardianTimestep deltaTime);

	private:
		static PxDefaultAllocator PhysicsAllocator;
		static PxDefaultErrorCallback PhysicsErrorCallback;
		static PxFoundation* PhysicsFoundation;
		static PxPvd* PhysicsDebugger;
		static PxPhysics* PhysicsObject;
		static PxDefaultCpuDispatcher* PhysicsCpuDispatcher;
		static PxScene* PhysicsScene;
	};
}

#endif