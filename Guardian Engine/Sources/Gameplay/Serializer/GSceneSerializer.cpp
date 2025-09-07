#include "GSceneSerializer.h"

namespace GE
{
	void GSceneSerializer::Serialize(std::shared_ptr<GScene> scene, YAML::Emitter& serializingData)
	{
		auto& SerializeData = serializingData;

		SerializeData << YAML::BeginMap;
		SerializeData << YAML::Key << "Scene";
		SerializeData << YAML::Value << (unsigned long long)scene->SceneId;
		SerializeData << YAML::Key << "Name";
		SerializeData << YAML::Value << scene->SceneName;
		SerializeData << YAML::Key << "Entities";
		SerializeData << YAML::Value << YAML::BeginSeq;

		for (auto& entity : scene->SceneEntityList)
		{
			GEntitySerializer::Serialize(entity.second, SerializeData);
		}

		SerializeData << YAML::EndSeq;
		SerializeData << YAML::EndMap;
	}

	void GSceneSerializer::Deserialize(std::shared_ptr<GScene> scene, YAML::Node& deserializingData)
	{
		scene->SceneRootEntity = GEntity::CreateNewEntity("Root", scene.get());
		scene->EditCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, -15.0f), GVector3(), GPerspectiveProjection());
		scene->RuntimeCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, 0.0f), GVector3(), GPerspectiveProjection());
		scene->LightRegistry = std::make_shared<GLightRegistry>();

		scene->SceneId = deserializingData["Scene"].as<unsigned long long>();
		scene->SceneName = deserializingData["Name"].as<std::string>();

		auto EntitiesData = deserializingData["Entities"];
		for (auto& EntityData : EntitiesData)
		{
			std::shared_ptr<GEntity> entity = std::make_shared<GEntity>();
			entity->EntityScene = scene.get();
			entity->EntityHandle = scene->EntityRegistry.create();
			GEntitySerializer::Deserialize(entity, EntityData);

			scene->SceneEntityList[entity->GetEntityName()] = entity;
		}

		scene->BuildEntityTree();
	}

	void GSceneSerializer::Export(const std::string& filePath, std::shared_ptr<GScene> scene)
	{
		YAML::Emitter SerializeData;
		Serialize(scene, SerializeData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializeData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();
		
		OutputFileStream.close();
	}

	void GSceneSerializer::Import(const std::string& filePath, std::shared_ptr<GScene> scene)
	{
		Deserialize(scene, Load(filePath));
	}

	YAML::Node GSceneSerializer::Load(const std::string& filePath)
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
