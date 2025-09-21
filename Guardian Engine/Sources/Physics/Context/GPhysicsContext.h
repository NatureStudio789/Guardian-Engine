#ifndef _GE_GPHYSICSCONTEXT_H_
#define _GE_GPHYSICSCONTEXT_H_
#include "../ErrorCallback/GPhysicsErrorCallback.h"

namespace GE
{
	class GUARDIAN_API GPhysicsContext
	{
	public:
		GPhysicsContext();
		GPhysicsContext(const std::string& name);
		GPhysicsContext(const GPhysicsContext& other);
		~GPhysicsContext();

		void InitializePhysicsContext(const std::string& name);

		const GUUID& GetContextId() const noexcept;
		const std::string& GetContextName() const noexcept;
		PxFoundation* GetPhysicsFoundation();
		PxPhysics* GetPhysicsHandle();
		PxDefaultCpuDispatcher* GetPhysicsCpuDispatcher();
		
		static std::shared_ptr<GPhysicsContext> CreateNewPhysicsContext(const std::string& name)
		{
			return std::make_shared<GPhysicsContext>(name);
		}

	private:
		GUUID ContextId;
		std::string ContextName;

		GPhysicsErrorCallback ErrorCallback;
		PxDefaultAllocator DefaultAllocator;
		PxFoundation* PhysicsFoundation;
		PxPhysics* PhysicsHandle;
		PxPvd* PhysicsDebugger;
		PxDefaultCpuDispatcher* PhysicsCpuDispatcher;
	};
}

#endif