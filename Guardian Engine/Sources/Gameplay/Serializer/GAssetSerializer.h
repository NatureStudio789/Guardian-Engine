#ifndef _GE_GASSETSERIALIZER_H_
#define _GE_GASSETSERIALIZER_H_
#include "../Entity/GEntity.h"

namespace GE
{
	class GUARDIAN_API GAssetSerializer
	{
	public:
		static void Serialize(std::shared_ptr<GAsset> asset, YAML::Emitter& serializingData);
		static void Serialize(GAsset* asset, YAML::Emitter& serializingData);

		static void Deserialize(std::shared_ptr<GAsset> asset, YAML::Node& deserializingData);
		static void Deserialize(GAsset* asset, YAML::Node& deserializingData);

		static void Export(const std::string& filePath, std::shared_ptr<GAsset> asset);
		static void Export(const std::string& filePath, GAsset* asset);
		static void Import(const std::string& filePath, std::shared_ptr<GAsset> asset);
		static void Import(const std::string& filePath, GAsset* asset);

		static YAML::Node Load(const std::string& filePath);
	};
}

#endif