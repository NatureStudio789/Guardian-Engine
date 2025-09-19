#ifndef _GE_GPHYSICSENGINE_H_
#define _GE_GPHYSICSENGINE_H_
#include "../Audio/GAudioEngine.h"

namespace GE
{
	class GUARDIAN_API GPhysicsEngine : public GModule
	{
	public:
		GPhysicsEngine();
		GPhysicsEngine(const GPhysicsEngine& other);
		~GPhysicsEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:
		std::shared_ptr<GPhysicsContext> MainPhysicsContext;
	};
}

#endif