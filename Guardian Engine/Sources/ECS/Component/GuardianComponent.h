#ifndef _GE_GUARDIANCOMPONENT_H_
#define _GE_GUARDIANCOMPONENT_H_
#include "../../Graphics/Renderer/GuardianRenderer.h"

namespace guardian
{
	class GUARDIAN_API GuardianComponent
	{
	public:

	};

	class GUARDIAN_API GuardianTagComponent : public GuardianComponent
	{
	public:
		GuardianTagComponent()
		{
			this->Tag = "Blank Tag";
		}
		GuardianTagComponent(const GString& tag)
		{
			this->Tag = tag;
		}
		GuardianTagComponent(const GuardianTagComponent& other)
		{
			this->Tag = other.Tag;
		}
		
		GString Tag;
	};

	class GUARDIAN_API GuardianTransformComponent : public GuardianComponent, public GuardianTransform
	{
	public:
	};

	class GUARDIAN_API GuardianCameraComponent : public GuardianComponent, public GuardianCamera
	{
	public:
		std::shared_ptr<GuardianModel> CameraModel;
	};

	class GuardianScriptableEntity;
	class GUARDIAN_API GuardianNativeScriptComponent : public GuardianComponent
	{
	public:
		GuardianScriptableEntity* EntityInstance = null;

		GuardianScriptableEntity* (*InstantiateScript)();
		void (*ReleaseScript)(GuardianNativeScriptComponent*);

		template<typename T>
		void Apply()
		{
			this->InstantiateScript = []() { return (GuardianScriptableEntity*)(new T()); };
			this->ReleaseScript = [](GuardianNativeScriptComponent* component) 
			{ 
				delete component->EntityInstance;
				component->EntityInstance = null;
			};
		}
	};

	class GUARDIAN_API GuardianScriptComponent : public GuardianComponent
	{
	public:
		GuardianScriptComponent()
		{
			this->ClassName = "";
		}
		GuardianScriptComponent(const GuardianScriptComponent& other)
		{
			this->ClassName = other.ClassName;
		}

		GString ClassName;
	};

	class GUARDIAN_API GuardianModelComponent : public GuardianComponent, public GuardianModel
	{
	public:
	};

	class GUARDIAN_API GuardianBoxColliderComponent : public GuardianComponent
	{
	public:
		GuardianBoxColliderComponent()
		{
			this->BoxCollider = std::make_shared<GuardianBoxCollider>();
		}
		GuardianBoxColliderComponent(const GuardianBoxColliderComponent& other)
		{
			this->BoxCollider = other.BoxCollider;
		}

		std::shared_ptr<GuardianBoxCollider> BoxCollider;
	};

	class GUARDIAN_API GuardianDynamicRigidBodyComponent : public GuardianComponent
	{
	public:
		GuardianDynamicRigidBodyComponent()
		{
			this->DynamicRigidBody = std::make_shared<GuardianDynamicRigidBody>();
		}
		GuardianDynamicRigidBodyComponent(const GuardianDynamicRigidBodyComponent& other)
		{
			this->DynamicRigidBody = other.DynamicRigidBody;
		}

		std::shared_ptr<GuardianDynamicRigidBody> DynamicRigidBody;
	};

	class GUARDIAN_API GuardianStaticRigidBodyComponent : public GuardianComponent
	{
	public:
		GuardianStaticRigidBodyComponent()
		{
			this->StaticRigidBody = std::make_shared<GuardianStaticRigidBody>();
		}
		GuardianStaticRigidBodyComponent(const GuardianStaticRigidBodyComponent& other)
		{
			this->StaticRigidBody = other.StaticRigidBody;
		}

		std::shared_ptr<GuardianStaticRigidBody> StaticRigidBody;
	};
}

#endif