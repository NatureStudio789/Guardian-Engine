#include "GPhysicsContextRegistry.h"

namespace GE
{
	std::string GPhysicsContextRegistry::CurrentPhysicsContext;
	std::map<std::string, std::shared_ptr<GPhysicsContext>> GPhysicsContextRegistry::PhysicsContextList;


	void GPhysicsContextRegistry::RegisterPhysicsContext(std::shared_ptr<GPhysicsContext> context)
	{
		if (PhysicsContextList.count(context->GetContextName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The physics context named '{}' has already been in the registry!", context->GetContextName()));
		}

		PhysicsContextList[context->GetContextName()] = context;
	}

	void GPhysicsContextRegistry::SetCurrentPhysicsContext(const std::string& name)
	{
		if (!PhysicsContextList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics context named '{}' found in registry!", name));
		}

		CurrentPhysicsContext = name;
	}

	void GPhysicsContextRegistry::SetCurrentPhysicsContext(const GUUID& id)
	{
		for (auto& [name, context] : PhysicsContextList)
		{
			if (context->GetContextId() == id)
			{
				CurrentPhysicsContext = name;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics context with id :  '{}' found in registry!", (unsigned long long)id));
	}

	std::shared_ptr<GPhysicsContext> GPhysicsContextRegistry::GetCurrentPhysicsContext()
	{
		if (!PhysicsContextList.count(CurrentPhysicsContext))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Current physics context named '{}' is invalid!", CurrentPhysicsContext));
		}

		return PhysicsContextList[CurrentPhysicsContext];
	}

	std::shared_ptr<GPhysicsContext> GPhysicsContextRegistry::GetPhysicsContext(const std::string& name)
	{
		if (!PhysicsContextList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics context named '{}' found in registry!", name));
		}

		return PhysicsContextList[name];
	}

	std::shared_ptr<GPhysicsContext> GPhysicsContextRegistry::GetPhysicsContext(const GUUID& id)
	{
		for (auto& [name, context] : PhysicsContextList)
		{
			if (context->GetContextId() == id)
			{
				return context;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics context with id :  '{}' found in registry!", (unsigned long long)id));
	}
}
