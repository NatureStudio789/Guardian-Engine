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
