#include "GProjectSerializer.h"

namespace GE
{
	void GProjectSerializer::Serialize(std::shared_ptr<GProject> project, YAML::Emitter& serializingData)
	{
		auto& SerializingData = serializingData;

		SerializingData << YAML::BeginMap;
		SerializingData << YAML::Key << "Project";
		SerializingData << YAML::Value << (unsigned long long)project->GetProjectId();
		SerializingData << YAML::Key << "Name";
		SerializingData << YAML::Value << project->GetProjectName();
		
		SerializingData << YAML::Key << "Asset Directory";
		SerializingData << YAML::Value << project->GetProjectAssetDirectory();

		SerializingData << YAML::Key << "Startup Scene";
		SerializingData << YAML::Value << project->GetStartupSceneAssetId();

		SerializingData << YAML::EndMap;
	}

	void GProjectSerializer::Serialize(GProject* project, YAML::Emitter& serializingData)
	{
		auto& SerializingData = serializingData;

		SerializingData << YAML::BeginMap;
		SerializingData << YAML::Key << "Project";
		SerializingData << YAML::Value << (unsigned long long)project->GetProjectId();
		SerializingData << YAML::Key << "Name";
		SerializingData << YAML::Value << project->GetProjectName();

		SerializingData << YAML::Key << "Asset Directory";
		SerializingData << YAML::Value << project->GetProjectAssetDirectory();

		SerializingData << YAML::Key << "Startup Scene";
		SerializingData << YAML::Value << project->GetStartupSceneAssetId();

		SerializingData << YAML::EndMap;
	}

	void GProjectSerializer::Deserialize(std::shared_ptr<GProject> project, YAML::Node& deserializingData)
	{
		project->ProjectId = deserializingData["Project"].as<unsigned long long>();
		project->ProjectName = deserializingData["Name"].as<std::string>();

		project->ProjectAssetDirectory = deserializingData["Asset Directory"].as<std::string>();

		project->StartupSceneAssetId = deserializingData["Active Scene"].as<unsigned long long>();
	}

	void GProjectSerializer::Deserialize(GProject* project, YAML::Node& deserializingData)
	{
		project->ProjectId = deserializingData["Project"].as<unsigned long long>();
		project->ProjectName = deserializingData["Name"].as<std::string>();

		project->ProjectAssetDirectory = deserializingData["Asset Directory"].as<std::string>();

		project->StartupSceneAssetId = deserializingData["Startup Scene"].as<unsigned long long>();
	}

	void GProjectSerializer::Export(const std::string& filePath, std::shared_ptr<GProject> project)
	{
		YAML::Emitter SerializingData;
		Serialize(project, SerializingData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializingData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	void GProjectSerializer::Export(const std::string& filePath, GProject* project)
	{
		YAML::Emitter SerializingData;
		Serialize(project, SerializingData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializingData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	void GProjectSerializer::Import(const std::string& filePath, std::shared_ptr<GProject> project)
	{
		Deserialize(project, Load(filePath));
	}

	void GProjectSerializer::Import(const std::string& filePath, GProject* project)
	{
		Deserialize(project, Load(filePath));
	}

	YAML::Node GProjectSerializer::Load(const std::string& filePath)
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
