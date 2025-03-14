#include "GuardianPhysicsEngine.h"

namespace guardian
{
	PxDefaultAllocator GuardianPhysicsEngine::PhysicsAllocator;
	PxDefaultErrorCallback GuardianPhysicsEngine::PhysicsErrorCallback;
	PxFoundation* GuardianPhysicsEngine::PhysicsFoundation = null;
	PxPvd* GuardianPhysicsEngine::PhysicsDebugger = null;
	PxPhysics* GuardianPhysicsEngine::PhysicsObject = null;
	PxDefaultCpuDispatcher* GuardianPhysicsEngine::PhysicsCpuDispatcher = null;


	void GuardianPhysicsEngine::InitializePhysicsEngine()
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

		PhysicsCpuDispatcher = PxDefaultCpuDispatcherCreate(16);

		if (!PxInitExtensions(*PhysicsObject, PhysicsDebugger))
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to initialize physics engine extentions!");
		}
	}

	PxMaterial* GuardianPhysicsEngine::CreatePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		return PhysicsObject->createMaterial(staticFriction, dynamicFriction, restitution);
	}

	PxPhysics* GuardianPhysicsEngine::GetPhysicsObject() noexcept
	{
		return PhysicsObject;
	}
}
