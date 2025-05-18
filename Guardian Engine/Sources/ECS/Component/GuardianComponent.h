#ifndef _GE_GUARDIANCOMPONENT_H_
#define _GE_GUARDIANCOMPONENT_H_
#include "../../Graphics/Renderer/GuardianRenderer.h"

namespace GE
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
		std::shared_ptr<GuardianMesh> CameraMesh;
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

	class GUARDIAN_API GuardianPointLightComponent : public GuardianComponent
	{
	public:
		GuardianPointLightComponent()
		{
			this->LightProperties = GuardianPointLightProperties();
			this->LightMesh = null;
		}
		GuardianPointLightComponent(const GuardianPointLightComponent& other)
		{
			this->LightId = other.LightId;
			this->LightProperties = other.LightProperties;
			this->LightMesh = other.LightMesh;
		}

		GuardianUUID LightId = GuardianUUID();
		GuardianPointLightProperties LightProperties;
		std::shared_ptr<GuardianMesh> LightMesh;
	};

	class GUARDIAN_API GuardianMeshComponent : public GuardianComponent
	{
	public:
		GuardianMeshComponent()
		{
			this->Mesh = std::make_shared<GuardianMesh>();
		}
		GuardianMeshComponent(const GuardianMeshComponent& other)
		{
			this->Mesh = other.Mesh;
		}

		std::shared_ptr<GuardianMesh> Mesh;
	};

	class GUARDIAN_API GuardianSphereColliderComponent : public GuardianComponent
	{
	public:
		GuardianSphereColliderComponent()
		{
			this->SphereCollider = std::make_shared<GuardianSphereCollider>();
			this->SphereGeometry = std::make_shared<GuardianGeometry>();
		}
		GuardianSphereColliderComponent(const GuardianSphereColliderComponent& other)
		{
			this->SphereCollider = other.SphereCollider;
			this->SphereGeometry = other.SphereGeometry;
		}

		std::shared_ptr<GuardianSphereCollider> SphereCollider;
		std::shared_ptr<GuardianGeometry> SphereGeometry;
	};

	class GUARDIAN_API GuardianBoxColliderComponent : public GuardianComponent
	{
	public:
		GuardianBoxColliderComponent()
		{
			this->BoxCollider = std::make_shared<GuardianBoxCollider>();
			this->BoxGeometry = std::make_shared<GuardianGeometry>();
		}
		GuardianBoxColliderComponent(const GuardianBoxColliderComponent& other)
		{
			this->BoxCollider = other.BoxCollider;
			this->BoxGeometry = other.BoxGeometry;
		}

		std::shared_ptr<GuardianBoxCollider> BoxCollider;
		std::shared_ptr<GuardianGeometry> BoxGeometry;
	};

	class GUARDIAN_API GuardianCapsuleColliderComponent : public GuardianComponent
	{
	public:
		GuardianCapsuleColliderComponent()
		{
			this->CapsuleCollider = std::make_shared<GuardianCapsuleCollider>();
			this->CapsuleGeometry = std::make_shared<GuardianGeometry>();
		}
		GuardianCapsuleColliderComponent(const GuardianCapsuleColliderComponent& other)
		{
			this->CapsuleCollider = other.CapsuleCollider;
			this->CapsuleGeometry = other.CapsuleGeometry;
		}

		std::shared_ptr<GuardianCapsuleCollider> CapsuleCollider;
		std::shared_ptr<GuardianGeometry> CapsuleGeometry;
	};

	class GUARDIAN_API GuardianMeshColliderComponent : public GuardianComponent
	{
	public:
		GuardianMeshColliderComponent()
		{
			this->MeshCollider = std::make_shared<GuardianMeshCollider>();
		}
		GuardianMeshColliderComponent(const GuardianMeshColliderComponent& other)
		{
			this->MeshCollider = other.MeshCollider;
		}

		std::shared_ptr<GuardianMeshCollider> MeshCollider;
	};

	enum GUARDIAN_API GuardianRigidBodyType
	{
		GE_RIGIDBODY_STATIC,
		GE_RIGIDBODY_DYNAMIC,
	};
	class GUARDIAN_API GuardianRigidBodyComponent : public GuardianComponent
	{
	public:
		GuardianRigidBodyComponent()
		{
			this->RigidBodyType = GE_RIGIDBODY_STATIC;
			this->DynamicRigidBody = std::make_shared<GuardianDynamicRigidBody>();
			this->StaticRigidBody = std::make_shared<GuardianStaticRigidBody>();
		}
		GuardianRigidBodyComponent(const GuardianRigidBodyComponent& other)
		{
			this->RigidBodyType = other.RigidBodyType;
			this->DynamicRigidBody = other.DynamicRigidBody;
			this->StaticRigidBody = other.StaticRigidBody;
		}

		GuardianRigidBodyType RigidBodyType;
		std::shared_ptr<GuardianDynamicRigidBody> DynamicRigidBody;
		std::shared_ptr<GuardianStaticRigidBody> StaticRigidBody;
	};
}

#endif