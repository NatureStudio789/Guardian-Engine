#include "GuardianPhysicsEngine.h"

namespace guardian
{
	PxDefaultAllocator GuardianPhysicsEngine::PhysicsAllocator;
	PxDefaultErrorCallback GuardianPhysicsEngine::PhysicsErrorCallback;
	PxFoundation* GuardianPhysicsEngine::PhysicsFoundation = null;
	PxPvd* GuardianPhysicsEngine::PhysicsDebugger = null;
	PxPhysics* GuardianPhysicsEngine::PhysicsObject = null;
	PxDefaultCpuDispatcher* GuardianPhysicsEngine::PhysicsCpuDispatcher = null;
	PxScene* GuardianPhysicsEngine::PhysicsScene = null;


	void GuardianPhysicsEngine::InitializePhysicsEngine(const GVector3& gravity)
	{
		PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, PhysicsAllocator, PhysicsErrorCallback);
		if (!PhysicsFoundation)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics foundation!");
		}

		PhysicsDebugger = PxCreatePvd(*PhysicsFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		PhysicsDebugger->connect(*transport, PxPvdInstrumentationFlag::eALL);

		PhysicsObject = PxCreatePhysics(PX_PHYSICS_VERSION, *PhysicsFoundation,
			PxTolerancesScale(), true, PhysicsDebugger);
		if (!PhysicsObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics object!");
		}

		PhysicsCpuDispatcher = PxDefaultCpuDispatcherCreate(2);

		PxSceneDesc SceneDesc(PhysicsObject->getTolerancesScale());
		SceneDesc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
		SceneDesc.cpuDispatcher = PhysicsCpuDispatcher;
		SceneDesc.filterShader = PxDefaultSimulationFilterShader;
		PhysicsScene = PhysicsObject->createScene(SceneDesc);
		if (!PhysicsScene)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics scene!");
		}

		PxPvdSceneClient* DebuggerClient = PhysicsScene->getScenePvdClient();
		if (DebuggerClient)
		{
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void GuardianPhysicsEngine::UpdatePhysicsSimulation(GuardianTimestep deltaTime)
	{
		PhysicsScene->fetchResults();
		PhysicsScene->fetchResultsParticleSystem();

		PhysicsScene->simulate(deltaTime.GetSecond());
	}
}
