#include "GPhysicsWorld.h"

namespace GE
{
	GPhysicsWorld::GPhysicsWorld()
	{
		this->WorldId = 0;
		this->WorldName = "";
		this->WorldScene = null;
	}

	GPhysicsWorld::GPhysicsWorld(const std::string& name, const GVector3& gravity)
	{
		this->InitializePhysicsWorld(name, gravity);
	}

	GPhysicsWorld::~GPhysicsWorld()
	{
		this->WorldId = 0;
		this->WorldName.clear();

		this->WorldScene->release();
	}

	void GPhysicsWorld::InitializePhysicsWorld(const std::string& name, const GVector3& gravity)
	{
		this->WorldId = GUUID();
		this->WorldName = name;

		PxSceneDesc WorldSceneDesc(GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->getTolerancesScale());
		WorldSceneDesc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
		WorldSceneDesc.cpuDispatcher = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsCpuDispatcher();
		WorldSceneDesc.filterShader = PxDefaultSimulationFilterShader;
		this->WorldScene = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createScene(WorldSceneDesc);
		if (!this->WorldScene)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics world scene!");
		}

		PxPvdSceneClient* SceneClientDebugger = this->WorldScene->getScenePvdClient();
		if (!SceneClientDebugger)
		{
			SceneClientDebugger->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			SceneClientDebugger->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			SceneClientDebugger->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void GPhysicsWorld::AttatchRigidBody(std::shared_ptr<GDynamicRigidBody> dynamicRigidBody)
	{
		this->WorldScene->addActor(*dynamicRigidBody->GetRigidBodyObject());
	}

	void GPhysicsWorld::AttatchRigidBody(std::shared_ptr<GStaticRigidBody> staticRigidBody)
	{
		this->WorldScene->addActor(*staticRigidBody->GetRigidBodyObject());
	}

	void GPhysicsWorld::Simulate()
	{
		this->WorldScene->simulate(0.01f);

		this->WorldScene->fetchResults(true);
	}

	const GUUID& GPhysicsWorld::GetWorldId() const noexcept
	{
		return this->WorldId;
	}

	const std::string& GPhysicsWorld::GetWorldName() const noexcept
	{
		return this->WorldName;
	}

	PxScene* GPhysicsWorld::GetWorldScene()
	{
		return this->WorldScene;
	}
}
