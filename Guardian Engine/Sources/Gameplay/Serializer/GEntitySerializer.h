#ifndef _GE_GENTITYSERIALIZER_H_
#define _GE_GENTITYSERIALIZER_H_
#include "../Entity/GEntity.h"

namespace GE
{
	class GUARDIAN_API GEntitySerializer
	{
	public:
		static void Serialize(std::shared_ptr<GEntity> entity, YAML::Emitter& serializingData);

		static void Deserialize(std::shared_ptr<GEntity> entity, YAML::Node& deserializingData);

		static void Export(const std::string& filePath, YAML::Emitter& serializingData);
		static void Import(const std::string& filePath, std::shared_ptr<GEntity> entity);
	};
}

#endif