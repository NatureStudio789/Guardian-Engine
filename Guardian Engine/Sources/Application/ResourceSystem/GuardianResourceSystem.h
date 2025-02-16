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

		static GuardianMesh GetMesh(GString meshName);
		static const GString GetMeshName(GuardianMesh mesh);

		static GuardianMaterial GetMaterial(GString materialName);
		static GuardianMaterial GetMaterial(const GuardianUUID& materialId);
		static const GString GetMaterialName(GuardianMaterial material);

		static std::map<GString, GuardianMesh> GetMeshList();
		static std::map<GString, GuardianMaterial> GetMaterialList();

	private:
		static void AddDefaultResource();

		static void IterateDirectory(const std::filesystem::path& path);
		static void LoadResource(const std::filesystem::path& path);
		
		static std::vector<GString> LoadedResourcePath;
		static std::map<GString, GuardianMesh> ResourceMeshList;
		static std::map<GString, GuardianTexture> ResourceTextureList;
		static std::map<GString, GuardianMaterial> ResourceMaterialList;
		static std::map<GString, GuardianPhysicsMaterial> ResourcePhysicsMaterialList;
	};
}

#endif