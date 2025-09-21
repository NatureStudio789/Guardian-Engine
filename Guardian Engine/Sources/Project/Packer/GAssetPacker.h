#ifndef _GE_GASSETPACKER_H_
#define _GE_GASSETPACKER_H_
#include "../../Physics/World/GPhysicsWorldRegistry.h"

namespace GE
{
	struct GUARDIAN_API GAssetPackHeader
	{
	public:
		char Identifier[6] = "GEPAK";
		unsigned int Version = 1;
		unsigned int FileCount = 0;
		unsigned long long EntriesOffset;
	};

	struct GAssetPackEntry
	{
	public:
		unsigned int PathLength;
		std::string FilePath;
		unsigned long long FileSize;
		unsigned long long DataOffset;
	};

	class GUARDIAN_API GAssetLoader;

	class GUARDIAN_API GAssetPacker
	{
	public:
		static void PackAsset(std::shared_ptr<GAssetLoader> assetLoader, const std::string& packageFilePath);

		static void LoadAssetPackage(std::shared_ptr<GAssetLoader> assetLoader, const std::string& packageFilePath);
		static void UnpackAsset(const std::string& packageFilePath, const std::string& unpackDirectory);
	};
}

#endif