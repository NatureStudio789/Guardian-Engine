#include "GuardianPhysicsEngine.h"

namespace GE
{
	PxDefaultAllocator GuardianPhysicsEngine::PhysicsAllocator;
	PxDefaultErrorCallback GuardianPhysicsEngine::PhysicsErrorCallback;
	PxFoundation* GuardianPhysicsEngine::PhysicsFoundation = null;
	PxPvd* GuardianPhysicsEngine::PhysicsDebugger = null;
	PxPhysics* GuardianPhysicsEngine::PhysicsObject = null;
	PxDefaultCpuDispatcher* GuardianPhysicsEngine::PhysicsCpuDispatcher = null;
	PxCudaContextManager* GuardianPhysicsEngine::PhysicsCudaManager = null;


	void GuardianPhysicsEngine::InitializePhysicsEngine()
	{
		PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, PhysicsAllocator, PhysicsErrorCallback);
		if (!PhysicsFoundation)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics foundation!");
		}
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Created engine physics foundation.");

		PhysicsDebugger = PxCreatePvd(*PhysicsFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		PhysicsDebugger->connect(*transport, PxPvdInstrumentationFlag::eALL);

		PhysicsObject = PxCreatePhysics(PX_PHYSICS_VERSION, *PhysicsFoundation,
			PxTolerancesScale(), true, PhysicsDebugger);
		if (!PhysicsObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics object!");
		}
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Created engine physics object.");

		PhysicsCpuDispatcher = PxDefaultCpuDispatcherCreate(16);

		PxCudaContextManagerDesc CudaManagerDesc;
		PhysicsCudaManager = PxCreateCudaContextManager(*PhysicsFoundation, CudaManagerDesc, 
			PxGetProfilerCallback());
		if (!PhysicsCudaManager)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics CUDA manager!\nYour GPU may not support the CUDA module!!!");
		}

		if (!PxInitExtensions(*PhysicsObject, PhysicsDebugger))
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to initialize physics engine extentions!");
		}
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Initialized physics extentions.");

		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to initialize physics engine.");
	}

	PxMaterial* GuardianPhysicsEngine::CreatePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		return PhysicsObject->createMaterial(staticFriction, dynamicFriction, restitution);
	}

	PxPhysics* GuardianPhysicsEngine::GetPhysicsObject() noexcept
	{
		return PhysicsObject;
	}

	PxCudaContextManager* GuardianPhysicsEngine::GetPhysicsCudaManager()
	{
		if (PhysicsCudaManager)
		{
			return PhysicsCudaManager;
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Your GPU may not support the CUDA module!");
		}
	}
}
