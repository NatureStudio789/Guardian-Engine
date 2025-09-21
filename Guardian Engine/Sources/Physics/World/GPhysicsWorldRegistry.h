#ifndef _GE_GPHYSICSWORLDREGISTRY_H_
#define _GE_GPHYSICSWORLDREGISTRY_H_
#include "GPhysicsWorld.h"

namespace GE
{
	class GUARDIAN_API GPhysicsWorldRegistry
	{
	public:
		static void RegistryPhysicsWorld(std::shared_ptr<GPhysicsWorld> world);
		static void RemovePhysicsWorld(const GUUID& id);
		static void RemovePhysicsWorld(const std::string& name);

		static void SetCurrentPhysicsWorld(const GUUID& id);
		static void SetCurrentPhysicsWorld(const std::string& name);

		static std::shared_ptr<GPhysicsWorld> GetCurrentPhysicsWorld();
		static bool IsCurrentPhysicsWorldAvailable();

		static std::shared_ptr<GPhysicsWorld> GetPhysicsWorld(const GUUID& id);
		static std::shared_ptr<GPhysicsWorld> GetPhysicsWorld(const std::string& name);

	private:
		static std::string CurrentPhysicsWorld;
		static std::map<std::string, std::shared_ptr<GPhysicsWorld>> PhysicsWorldList;
	};
}

#endif