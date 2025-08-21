#ifndef _GE_GSCENESERIALIZER_H_
#define _GE_GSCENESERIALIZER_H_
#include "GEntitySerializer.h"

namespace GE
{
	class GUARDIAN_API GSceneSerializer
	{
	public:
		static void Serialize(std::shared_ptr<GScene> scene, YAML::Emitter& serializingData);

		static void Deserialize(std::shared_ptr<GScene> scene, YAML::Node& deserializingData);

		static void Export(const std::string& filePath, std::shared_ptr<GScene> scene);
		static void Import(const std::string& filePath, std::shared_ptr<GScene> scene);

		static YAML::Node Load(const std::string& filePath);
	};
}

#endif