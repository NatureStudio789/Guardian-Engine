#ifndef _GE_GPHYSICSWORLD_H_
#define _GE_GPHYSICSWORLD_H_
#include "../Collider/GCollider.h"

namespace GE
{
	class GUARDIAN_API GPhysicsWorld
	{
	public:
		GPhysicsWorld();
		GPhysicsWorld(const std::string& name, const GVector3& gravity);
		~GPhysicsWorld();

		void InitializePhysicsWorld(const std::string& name, const GVector3& gravity);

		void Simulate();

		const GUUID& GetWorldId() const noexcept;
		const std::string& GetWorldName() const noexcept;
		PxScene* GetWorldScene();

		static std::shared_ptr<GPhysicsWorld> CreateNewPhysicsWorld(const std::string& name, const GVector3& gravity)
		{
			return std::make_shared<GPhysicsWorld>(name, gravity);
		}

	private:
		GUUID WorldId;
		std::string WorldName;

		PxScene* WorldScene;
	};
}

#endif