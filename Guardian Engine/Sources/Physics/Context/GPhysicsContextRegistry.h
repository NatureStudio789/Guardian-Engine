#ifndef _GE_GPHYSICSCONTEXTREGISTRY_H_
#define _GE_GPHYSICSCONTEXTREGISTRY_H_
#include "GPhysicsContext.h"

namespace GE
{
	class GUARDIAN_API GPhysicsContextRegistry
	{
	public:
		static void RegisterPhysicsContext(std::shared_ptr<GPhysicsContext> context);

		static void SetCurrentPhysicsContext(const std::string& name);
		static void SetCurrentPhysicsContext(const GUUID& id);

		static std::shared_ptr<GPhysicsContext> GetCurrentPhysicsContext();

		static std::shared_ptr<GPhysicsContext> GetPhysicsContext(const std::string& name);
		static std::shared_ptr<GPhysicsContext> GetPhysicsContext(const GUUID& id);

	private:
		static std::string CurrentPhysicsContext;
		static std::map<std::string, std::shared_ptr<GPhysicsContext>> PhysicsContextList;
	};
}

#endif