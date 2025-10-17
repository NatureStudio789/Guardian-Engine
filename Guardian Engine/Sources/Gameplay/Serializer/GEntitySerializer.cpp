#include "GEntitySerializer.h"

namespace GE
{
	void GEntitySerializer::Serialize(std::shared_ptr<GEntity> entity, YAML::Emitter& serializingData)
	{
		auto& SerializeData = serializingData;

		SerializeData << YAML::BeginMap;
		SerializeData << YAML::Key << "Entity";
		SerializeData << YAML::Value << (unsigned long long)entity->GetEntityId();
		SerializeData << YAML::Key << "Name";
		SerializeData << YAML::Value << entity->GetEntityName();
		SerializeData << YAML::Key << "Parent";
		SerializeData << YAML::Value << entity->ParentEntityName;

		if (entity->HasComponent<GTagComponent>())
		{
			SerializeData << YAML::Key << "Tag Component";
			SerializeData << YAML::BeginMap;

			auto& tag = entity->GetComponent<GTagComponent>().Tag;
			SerializeData << YAML::Key << "Tag" << YAML::Value << tag;

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GTransformComponent>())
		{
			SerializeData << YAML::Key << "Transform Component";
			SerializeData << YAML::BeginMap;

			auto& Position = entity->GetComponent<GTransformComponent>().Transform.Position;
			SerializeData << YAML::Key << "Position" << YAML::Value << Position;

			auto& Rotation = entity->GetComponent<GTransformComponent>().Transform.Rotation;
			SerializeData << YAML::Key << "Rotation" << YAML::Value << Rotation;

			auto& Scale = entity->GetComponent<GTransformComponent>().Transform.Scale;
			SerializeData << YAML::Key << "Scale" << YAML::Value << Scale;

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GCameraComponent>())
		{
			SerializeData << YAML::Key << "Camera Component";
			SerializeData << YAML::BeginMap;

			auto& IsFreeLook = entity->GetComponent<GCameraComponent>().Camera->IsFreeLook;
			SerializeData << YAML::Key << "IsFreeLook" << YAML::Value << IsFreeLook;

			SerializeData << YAML::Key << "Projection";
			SerializeData << YAML::BeginMap;

			auto& FOV = entity->GetComponent<GCameraComponent>().Camera->Projection.FOV;
			SerializeData << YAML::Key << "FOV" << YAML::Value << FOV;

			auto& NearZ = entity->GetComponent<GCameraComponent>().Camera->Projection.NearZ;
			SerializeData << YAML::Key << "NearZ" << YAML::Value << NearZ;

			auto& FarZ = entity->GetComponent<GCameraComponent>().Camera->Projection.FarZ;
			SerializeData << YAML::Key << "FarZ" << YAML::Value << FarZ;

			SerializeData << YAML::EndMap;

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GPointLightComponent>())
		{
			SerializeData << YAML::Key << "Point Light Component";
			SerializeData << YAML::BeginMap;

			auto& Color = entity->GetComponent<GPointLightComponent>().PointLight->Color;
			SerializeData << YAML::Key << "Color" << YAML::Value << Color;

			auto& Strength = entity->GetComponent<GPointLightComponent>().PointLight->Strength;
			SerializeData << YAML::Key << "Strength" << YAML::Value << Strength;

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GColliderComponent>())
		{
			SerializeData << YAML::Key << "Collider Component";
			SerializeData << YAML::BeginMap;

			SerializeData << YAML::Key << "Shapes";
			SerializeData << YAML::Value << YAML::BeginSeq;

			for (auto& shape : entity->GetComponent<GColliderComponent>().Collider->GetColliderShapeList())
			{
				SerializeData << YAML::BeginMap;

				SerializeData << YAML::Key << "Local Transform";
				SerializeData << YAML::BeginMap;

				auto& LocalTransform = shape->GetLocalTransform();
				auto& Position = LocalTransform.Position;
				SerializeData << YAML::Key << "Position" << YAML::Value << Position;

				auto& Rotation = LocalTransform.Rotation;
				SerializeData << YAML::Key << "Rotation" << YAML::Value << Rotation;

				SerializeData << YAML::EndMap;

				SerializeData << YAML::Key << "Physics Material";
				SerializeData << YAML::BeginMap;

				auto& physicsMaterial = shape->GetShapeMaterial();

				SerializeData << YAML::Key << "Static Friction" << YAML::Value << physicsMaterial->GetStaticFriction();
				SerializeData << YAML::Key << "Dynamic Friction" << YAML::Value << physicsMaterial->GetDynamicFriction();
				SerializeData << YAML::Key << "Restitution" << YAML::Value << physicsMaterial->GetRestitution();

				SerializeData << YAML::EndMap;

				SerializeData << YAML::Key << "Category";
				
				const auto& Category = shape->GetShapeCategory();
				switch (Category)
				{
					case GShape::GE_SHAPE_BOX:
					{
						SerializeData << YAML::Value << "Box";

						auto& boxShape = std::dynamic_pointer_cast<GBoxShape>(shape);

						const auto& edgeLength = boxShape->GetEdgeLength();
						SerializeData << YAML::Key << "Edge Length" << YAML::Value << edgeLength;

						break;
					}

					case GShape::GE_SHAPE_SPHERE:
					{
						SerializeData << YAML::Value << "Sphere";

						auto& sphereShape = std::dynamic_pointer_cast<GSphereShape>(shape);

						const auto& radius = sphereShape->GetRadius();
						SerializeData << YAML::Key << "Radius" << YAML::Value << radius;

						break;
					}

					case GShape::GE_SHAPE_CAPSULE:
					{
						SerializeData << YAML::Value << "Capsule";

						auto& capsuleShape = std::dynamic_pointer_cast<GCapsuleShape>(shape);

						const auto& radius = capsuleShape->GetHalfSphereRadius();
						SerializeData << YAML::Key << "Half Sphere Radius" << YAML::Value << radius;

						const auto& height = capsuleShape->GetHeight();
						SerializeData << YAML::Key << "Height" << YAML::Value << height;

						break;
					}

					default:
					{
						throw GUARDIAN_ERROR_EXCEPTION("Unknown shape category!");

						break;
					}
				}

				SerializeData << YAML::EndMap;
			}

			SerializeData << YAML::EndSeq;

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GRigidBodyComponent>())
		{
			SerializeData << YAML::Key << "Rigid Body Component";
			SerializeData << YAML::BeginMap;

			auto& RigidBodyComponent = entity->GetComponent<GRigidBodyComponent>();

			SerializeData << YAML::Key << "Mass" << YAML::Value << RigidBodyComponent.RigidBody->GetRigidBodyMass();

			SerializeData << YAML::Key << "Is Kinematic" << YAML::Value << RigidBodyComponent.RigidBody->GetKinematic();

			SerializeData << YAML::EndMap;
		}

		if (entity->HasComponent<GModelComponent>())
		{
			SerializeData << YAML::Key << "Model Component";
			SerializeData << YAML::BeginMap;

			auto& ModelAssetName = entity->GetComponent<GModelComponent>().ModelAssetName;
			SerializeData << YAML::Key << "Asset Name" << YAML::Value << ModelAssetName;

			SerializeData << YAML::EndMap;
		}

		SerializeData << YAML::EndMap;
	}

	void GEntitySerializer::Deserialize(std::shared_ptr<GEntity> entity, YAML::Node& deserializingData)
	{
		entity->EntityId = deserializingData["Entity"].as<unsigned long long>();
		entity->EntityName = deserializingData["Name"].as<std::string>();
		entity->ParentEntityName = deserializingData["Parent"].as<std::string>();

		auto TagComponent = deserializingData["Tag Component"];
		if (TagComponent)
		{
			auto& Tag = entity->AddComponent<GTagComponent>();

			Tag.Tag = TagComponent["Tag"].as<std::string>();
		}

		auto TransformComponent = deserializingData["Transform Component"];
		if (TransformComponent)
		{
			auto& Transform = entity->AddComponent<GTransformComponent>();

			Transform.Transform.Position = TransformComponent["Position"].as<GVector3>();
			Transform.Transform.Rotation = TransformComponent["Rotation"].as<GVector3>();
			Transform.Transform.Scale = TransformComponent["Scale"].as<GVector3>();
		}

		auto CameraComponent = deserializingData["Camera Component"];
		if (CameraComponent)
		{
			auto& Camera = entity->AddComponent<GCameraComponent>();

			Camera.Camera->IsFreeLook = CameraComponent["IsFreeLook"].as<bool>();
			
			auto& Projection = CameraComponent["Projection"];
			Camera.Camera->Projection.FOV = Projection["FOV"].as<float>();
			Camera.Camera->Projection.NearZ = Projection["NearZ"].as<float>();
			Camera.Camera->Projection.FarZ = Projection["FarZ"].as<float>();
		}

		auto PointLightComponent = deserializingData["Point Light Component"];
		if (PointLightComponent)
		{
			auto& PointLight = entity->AddComponent<GPointLightComponent>();

			PointLight.PointLight->Color = PointLightComponent["Color"].as<GVector3>();
			PointLight.PointLight->Strength = PointLightComponent["Strength"].as<float>();
		}

		auto ColliderComponent = deserializingData["Collider Component"];
		if (ColliderComponent)
		{
			auto& Collider = entity->AddComponent<GColliderComponent>();

			auto& ShapeList = ColliderComponent["Shapes"];
			for (auto& ShapeData : ShapeList)
			{
				auto& LocalTransform = ShapeData["Local Transform"];

				std::shared_ptr<GShape> shape;
				auto& ShapeCategory = ShapeData["Category"].as<std::string>();
				if (ShapeCategory == "Box")
				{
					shape = GBoxShape::CreateNewBoxShape(ShapeData["Edge Length"].as<GVector3>());
				}
				else if (ShapeCategory == "Sphere")
				{
					shape = GSphereShape::CreateNewSphereShape(ShapeData["Radius"].as<float>());
				}
				else if (ShapeCategory == "Capsule")
				{
					shape = GCapsuleShape::CreateNewCapsuleShape(
						ShapeData["Half Sphere Radius"].as<float>(), ShapeData["Height"].as<float>());
				}
			
				shape->SetLocalPosition(LocalTransform["Position"].as<GVector3>());
				shape->SetLocalRotation(LocalTransform["Rotation"].as<GVector3>());

				auto& PhysicsMaterial = ShapeData["Physics Material"];
				shape->GetShapeMaterial()->SetStaticFriction(PhysicsMaterial["Static Friction"].as<float>());
				shape->GetShapeMaterial()->SetDynamicFriction(PhysicsMaterial["Dynamic Friction"].as<float>());
				shape->GetShapeMaterial()->SetRestitution(PhysicsMaterial["Restitution"].as<float>());

				if (entity->EntityScene->GetRenderingInitialized())
				{
					Collider.AddColliderShape(shape);
				}
				else
				{
					Collider.AddColliderShape(shape, false);
				}
			}
		}

		auto RigidBodyComponent = deserializingData["Rigid Body Component"];
		if (RigidBodyComponent)
		{
			auto& RigidBody = entity->AddComponent<GRigidBodyComponent>();

			RigidBody.RigidBody->SetMass(RigidBodyComponent["Mass"].as<float>());
			RigidBody.RigidBody->SetKinematic(RigidBodyComponent["Is Kinematic"].as<bool>());
		}

		auto ModelComponent = deserializingData["Model Component"];
		if (ModelComponent)
		{
			auto& Model = entity->AddComponent<GModelComponent>();

			Model.ModelAssetName = ModelComponent["Asset Name"].as<std::string>();
		}
	}

	void GEntitySerializer::Export(const std::string& filePath, std::shared_ptr<GEntity> entity)
	{
		YAML::Emitter SerializingData;
		Serialize(entity, SerializingData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializingData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	void GEntitySerializer::Import(const std::string& filePath, std::shared_ptr<GEntity> entity)
	{
		Deserialize(entity, Load(filePath));
	}

	YAML::Node GEntitySerializer::Load(const std::string& filePath)
	{
		std::ifstream InputFileStream(filePath);
		if (!InputFileStream.is_open())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The scene file is invalid!");
		}

		std::stringstream InputStringStream;
		InputStringStream << InputFileStream.rdbuf();

		std::string SceneFileString = InputStringStream.str();

		YAML::Node SceneData = YAML::Load(SceneFileString);

		return SceneData;
	}
}
