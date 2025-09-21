#include "GPhysicsEngine.h"

namespace GE
{
	GPhysicsEngine::GPhysicsEngine()
	{
		this->ModuleName = "Physics";
		this->IsModuleLoaded = false;

		this->MainPhysicsContext = null;
	}

	GPhysicsEngine::GPhysicsEngine(const GPhysicsEngine& other) : GModule(other)
	{
		this->MainPhysicsContext = other.MainPhysicsContext;
	}

	GPhysicsEngine::~GPhysicsEngine()
	{

	}

	void GPhysicsEngine::InitializeModule()
	{
		this->MainPhysicsContext = GPhysicsContext::CreateNewPhysicsContext("Main");
		GPhysicsContextRegistry::RegisterPhysicsContext(this->MainPhysicsContext);
		GPhysicsContextRegistry::SetCurrentPhysicsContext(this->MainPhysicsContext->GetContextName());
	}

	void GPhysicsEngine::UpdateModule()
	{
		if (GPhysicsWorldRegistry::IsCurrentPhysicsWorldAvailable())
		{
			GPhysicsWorldRegistry::GetCurrentPhysicsWorld()->Simulate();
		}
	}

	void GPhysicsEngine::ReleaseModule()
	{
		this->MainPhysicsContext.reset();
	}
}
