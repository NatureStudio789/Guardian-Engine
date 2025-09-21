#include "GPhysicsWorldRegistry.h"

namespace GE
{
	std::string GPhysicsWorldRegistry::CurrentPhysicsWorld;
	std::map<std::string, std::shared_ptr<GPhysicsWorld>> GPhysicsWorldRegistry::PhysicsWorldList;


	void GPhysicsWorldRegistry::RegistryPhysicsWorld(std::shared_ptr<GPhysicsWorld> world)
	{
		if (PhysicsWorldList.count(world->GetWorldName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The physics world named '{}' has already been in registry!", world->GetWorldName()));
		}

		PhysicsWorldList[world->GetWorldName()] = world;
	}

	void GPhysicsWorldRegistry::RemovePhysicsWorld(const GUUID& id)
	{
		for (auto& iterator = PhysicsWorldList.begin(); iterator != PhysicsWorldList.end(); iterator++)
		{
			if ((*iterator).second->GetWorldId() == id)
			{
				if ((*iterator).first == CurrentPhysicsWorld)
				{
					CurrentPhysicsWorld.clear();
				}

				PhysicsWorldList.erase(iterator);
				break;
			}
		}
	}

	void GPhysicsWorldRegistry::RemovePhysicsWorld(const std::string& name)
	{
		for (auto& iterator = PhysicsWorldList.begin(); iterator != PhysicsWorldList.end(); iterator++)
		{
			if ((*iterator).first == name)
			{
				if (name == CurrentPhysicsWorld)
				{
					CurrentPhysicsWorld.clear();
				}

				PhysicsWorldList.erase(iterator);
				break;
			}
		}
	}

	void GPhysicsWorldRegistry::SetCurrentPhysicsWorld(const GUUID& id)
	{
		for (auto& [name, world] : PhysicsWorldList)
		{
			if (world->GetWorldId() == id)
			{
				CurrentPhysicsWorld = name;
				return;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics world with id '{}' found in registry!", (unsigned long long)id));
	}

	void GPhysicsWorldRegistry::SetCurrentPhysicsWorld(const std::string& name)
	{
		if (!PhysicsWorldList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics world named '{}' found in registry!", name));
		}

		CurrentPhysicsWorld = name;
	}

	std::shared_ptr<GPhysicsWorld> GPhysicsWorldRegistry::GetCurrentPhysicsWorld()
	{
		if (!PhysicsWorldList.count(CurrentPhysicsWorld))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Current physics world named '{}' is invalid!", CurrentPhysicsWorld));
		}

		return PhysicsWorldList[CurrentPhysicsWorld];
	}

	bool GPhysicsWorldRegistry::IsCurrentPhysicsWorldAvailable()
	{
		return PhysicsWorldList.count(CurrentPhysicsWorld) > 0;
	}

	std::shared_ptr<GPhysicsWorld> GPhysicsWorldRegistry::GetPhysicsWorld(const GUUID& id)
	{
		for (auto& [name, world] : PhysicsWorldList)
		{
			if (world->GetWorldId() == id)
			{
				return PhysicsWorldList[name];
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics world with id '{}' found in registry!", (unsigned long long)id));
	}

	std::shared_ptr<GPhysicsWorld> GPhysicsWorldRegistry::GetPhysicsWorld(const std::string& name)
	{
		if (!PhysicsWorldList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No physics world named '{}' found in registry!", name));
		}

		return PhysicsWorldList[name];
	}
}
