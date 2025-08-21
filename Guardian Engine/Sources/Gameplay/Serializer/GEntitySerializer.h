#ifndef _GE_GENTITYSERIALIZER_H_
#define _GE_GENTITYSERIALIZER_H_
#include "GAssetSerializer.h"

namespace GE
{
	class GUARDIAN_API GEntitySerializer
	{
	public:
		static void Serialize(std::shared_ptr<GEntity> entity, YAML::Emitter& serializingData);

		static void Deserialize(std::shared_ptr<GEntity> entity, YAML::Node& deserializingData);

		static void Export(const std::string& filePath, std::shared_ptr<GEntity> entity);
		static void Import(const std::string& filePath, std::shared_ptr<GEntity> entity);

		static YAML::Node Load(const std::string& filePath);
	};
}

#endif