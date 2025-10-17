#ifndef _GE_GCOMPONENT_H_
#define _GE_GCOMPONENT_H_
#include "../../Asset/Loader/GAssetLoaderRegistry.h"

namespace GE
{
	struct GUARDIAN_API GTagComponent
	{
	public:
		GTagComponent()
		{
			this->Tag = "";
		}
		GTagComponent(const std::string& tag)
		{
			this->Tag = tag;
		}
		GTagComponent(const GTagComponent& other) = default;

		std::string Tag;
	};

	struct GUARDIAN_API GTransformComponent
	{
	public:
		GTransformComponent()
		{
			this->Transform = GTransform();
			this->AccumulatedMatrix = GMatrix::IdentityMatrix();
		}
		GTransformComponent(const GTransform& transform)
		{
			this->Transform = transform;
			this->AccumulatedMatrix = GMatrix::IdentityMatrix();
		}
		GTransformComponent(const GTransformComponent& ) = default;

		GTransform Transform;

	private:
		GMatrix AccumulatedMatrix;

		friend class GScene;
	};

	struct GUARDIAN_API GCameraComponent
	{
	public:
		GCameraComponent()
		{
			this->Camera = std::make_shared<GCamera>();
		}
		GCameraComponent(std::shared_ptr<GCamera> camera)
		{
			this->Camera = camera;
		}
		GCameraComponent(const GCameraComponent&) = default;

		std::shared_ptr<GCamera> Camera;
	};

	struct GUARDIAN_API GPointLightComponent
	{
	public:
		GPointLightComponent()
		{
			this->PointLight = std::make_shared<GPointLight>();
		}
		GPointLightComponent(std::shared_ptr<GPointLight> pointLight)
		{
			this->PointLight = pointLight;
		}
		GPointLightComponent(const GPointLightComponent&) = default;

		std::shared_ptr<GPointLight> PointLight;
	};

	struct GUARDIAN_API GColliderComponent
	{
	public:
		GColliderComponent()
		{
			this->Collider = std::make_shared<GCollider>();
			this->ColliderWireframeList.clear();
			this->StaticRigidBody = null;
		}
		GColliderComponent(std::shared_ptr<GCollider> collider)
		{
			this->Collider = collider;
		}
		GColliderComponent(const GColliderComponent&) = default;

		void AddColliderShape(std::shared_ptr<GShape> shape, bool initializeWireframe = true)
		{
			this->Collider->AddShape(shape);

			std::shared_ptr<GGeometry> geometry;
			switch (shape->GetShapeCategory())
			{
				case GShape::GE_SHAPE_BOX:
				{
					geometry = std::make_shared<GBoxGeometry>(std::dynamic_pointer_cast<GBoxShape>(shape)->EdgeLength);

					break;
				}

				case GShape::GE_SHAPE_SPHERE:
				{
					geometry = std::make_shared<GSphereGeometry>(std::dynamic_pointer_cast<GSphereShape>(shape)->Radius);

					break;
				}

				case GShape::GE_SHAPE_CAPSULE:
				{
					geometry = std::make_shared<GCapsuleGeometry>(std::dynamic_pointer_cast<GCapsuleShape>(shape)->Height, 
						std::dynamic_pointer_cast<GCapsuleShape>(shape)->HalfSphereRadius);

					break;
				}

				default:
				{
					throw GUARDIAN_ERROR_EXCEPTION("Unknown collider shape category!");
					break;
				}
			}
			this->ColliderGeometryList.push_back(geometry);
			if (initializeWireframe)
			{
				this->ColliderWireframeList.push_back(GGeometryWireframe::CreateNewGeometryWireframe(geometry));
			}
			else
			{
				this->ToInitializeGeometryList.push_back(geometry);
			}
		}

		std::shared_ptr<GCollider> Collider;

	private:
		std::vector<std::shared_ptr<GGeometry>> ColliderGeometryList;
		std::vector<std::shared_ptr<GGeometry>> ToInitializeGeometryList;
		std::vector<std::shared_ptr<GGeometryWireframe>> ColliderWireframeList;
		std::shared_ptr<GStaticRigidBody> StaticRigidBody;

		friend class GScene;
	};

	struct GUARDIAN_API GRigidBodyComponent
	{
	public:
		GRigidBodyComponent()
		{
			this->RigidBody = std::make_shared<GDynamicRigidBody>();
		}
		GRigidBodyComponent(std::shared_ptr<GDynamicRigidBody> rigidBody)
		{
			this->RigidBody = rigidBody;
		}
		GRigidBodyComponent(const GRigidBodyComponent&) = default;

		std::shared_ptr<GDynamicRigidBody> RigidBody;
	};

	struct GUARDIAN_API GModelComponent
	{
	public:
		GModelComponent()
		{
			this->Model = null;
		}
		GModelComponent(const std::string& modelAssetName)
		{
			this->Model = null;
			this->ModelAssetName = modelAssetName;
		}
		GModelComponent(const GModelComponent&) = default;

		std::string ModelAssetName;

	private:
		std::shared_ptr<GModel> Model;

		friend class GScene;
		friend class GSceneEditor;
	};
}

#endif