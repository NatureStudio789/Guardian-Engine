#ifndef _GE_GUARDIANRESOURCESYSTEM_H_
#define _GE_GUARDIANRESOURCESYSTEM_H_
#include "../../Profile/Program/GuardianProgram.h"

namespace guardian
{
	class GUARDIAN_API GuardianResourceSystem
	{
	public:
		static void InitializeResourceSystem();

		static void UpdateResourceSystem();

		static const std::vector<GuardianMeshInstance::Data>& GetMeshData(GString meshName);

		static GuardianTexture GetTexture(GString textureName);
		static const GString GetTextureName(GuardianTexture texture);

		static std::map<GString, std::vector<GuardianMeshInstance::Data>> GetMeshDataList();
		static std::map<GString, GuardianTexture> GetTextureList();

	private:
		static void AddDefaultResource();

		static void IterateDirectory(const std::filesystem::path& path);
		static void LoadResource(const std::filesystem::path& path);
		
		static std::vector<GString> LoadedResourcePath;
		static std::map<GString, std::vector<GuardianMeshInstance::Data>> MeshDataResourceList;
		static std::map<GString, GuardianTexture> TextureResourceList;
		static std::map<GString, GuardianPhysicsMaterial> PhysicsMaterialResourceList;
	};
}

#endif