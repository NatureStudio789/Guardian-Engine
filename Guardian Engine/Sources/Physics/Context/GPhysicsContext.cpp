#include "GPhysicsContext.h"

namespace GE
{
	GPhysicsContext::GPhysicsContext()
	{
		this->ContextId = 0;
		this->ContextName = "";
		this->PhysicsDebugger = null;
		this->PhysicsHandle = null;
		this->PhysicsFoundation = null;
	}

	GPhysicsContext::GPhysicsContext(const std::string& name)
	{
		this->InitializePhysicsContext(name);
	}

	GPhysicsContext::GPhysicsContext(const GPhysicsContext& other)
	{
		this->ContextId = other.ContextId;
		this->ContextName = other.ContextName;
		this->DefaultAllocator = other.DefaultAllocator;
		this->ErrorCallback = other.ErrorCallback;
		this->PhysicsDebugger = other.PhysicsDebugger;
		this->PhysicsFoundation = other.PhysicsFoundation;
		this->PhysicsHandle = other.PhysicsHandle;
	}

	GPhysicsContext::~GPhysicsContext()
	{
		this->ContextId = 0;
		this->ContextName.clear();

		this->PhysicsDebugger->release();
		this->PhysicsHandle->release();
		this->PhysicsFoundation->release();
	}

	void GPhysicsContext::InitializePhysicsContext(const std::string& name)
	{
		this->ContextId = GUUID();
		this->ContextName = name;

		this->PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->DefaultAllocator, this->ErrorCallback);
		if (!this->PhysicsFoundation)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics foundation!");
		}

		this->PhysicsDebugger = PxCreatePvd(*this->PhysicsFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		this->PhysicsDebugger->connect(*transport, PxPvdInstrumentationFlag::eALL);

		this->PhysicsHandle = PxCreatePhysics(PX_PHYSICS_VERSION, *this->PhysicsFoundation, PxTolerancesScale(), true, this->PhysicsDebugger);
		if (!this->PhysicsHandle)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics handle!");
		}
	}

	const GUUID& GPhysicsContext::GetContextId() const noexcept
	{
		return this->ContextId;
	}

	const std::string& GPhysicsContext::GetContextName() const noexcept
	{
		return this->ContextName;
	}

	PxFoundation* GPhysicsContext::GetPhysicsFoundation()
	{
		return this->PhysicsFoundation;
	}

	PxPhysics* GPhysicsContext::GetPhysicsHandle()
	{
		return this->PhysicsHandle;
	}
}
