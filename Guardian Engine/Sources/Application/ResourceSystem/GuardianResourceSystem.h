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

		static const GuardianMesh& GetMesh(GString meshName);
		static const GString GetMeshName(GuardianMesh mesh);

		static GuardianTexture GetTexture(GString textureName);
		static const GString GetTextureName(GuardianTexture texture);

		static const GuardianMaterial& GetMaterial(GString materialName);
		static const GuardianMaterial& GetMaterial(const GuardianUUID& materialId);
		static const GString GetMaterialName(GuardianMaterial material);

		static std::map<GString, GuardianMesh> GetMeshList();
		static std::map<GString, GuardianTexture> GetTextureList();
		static std::map<GString, GuardianMaterial> GetMaterialList();

	private:
		static void AddDefaultResource();

		static void IterateDirectory(const std::filesystem::path& path);
		static void LoadResource(const std::filesystem::path& path);
		
		static std::vector<GString> LoadedResourcePath;
		static std::map<GString, GuardianMesh> MeshResourceList;
		static std::map<GString, GuardianTexture> TextureResourceList;
		static std::map<GString, GuardianMaterial> MaterialResourceList;
		static std::map<GString, GuardianPhysicsMaterial> PhysicsMaterialResourceList;
	};
}

#endif