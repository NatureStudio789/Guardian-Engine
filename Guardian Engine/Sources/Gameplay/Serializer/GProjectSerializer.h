#ifndef _GE_GPROJECTSERIALIZER_H_
#define _GE_GPROJECTSERIALIZER_H_
#include "../Entity/GEntity.h"

namespace GE
{
	class GUARDIAN_API GProjectSerializer
	{
	public:
		static void Serialize(std::shared_ptr<GProject> project, YAML::Emitter& serializingData);
		static void Serialize(GProject* project, YAML::Emitter& serializingData);

		static void Deserialize(std::shared_ptr<GProject> project, YAML::Node& deserializingData);
		static void Deserialize(GProject* project, YAML::Node& deserializingData);

		static void Export(const std::string& filePath, std::shared_ptr<GProject> project);
		static void Export(const std::string& filePath, GProject* project);
		static void Import(const std::string& filePath, std::shared_ptr<GProject> project);
		static void Import(const std::string& filePath, GProject* project);

		static YAML::Node Load(const std::string& filePath);
	};
}

#endif