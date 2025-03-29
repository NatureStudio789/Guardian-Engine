#ifndef _GE_GUARDIANRESOURCESYSTEM_H_
#define _GE_GUARDIANRESOURCESYSTEM_H_
#include "Asset/GuardianMeshAsset.h"

namespace GE
{
	class GUARDIAN_API GuardianAssetSystem
	{
	public:
		static void InitializeAssetSystem();

		static void UpdateAssetSystem();

		static const GuardianMeshAsset& GetMeshAsset(GString meshName);
		static const GuardianMeshAsset& GetMeshAssetFromPath(GString meshFilePath);
		static std::shared_ptr<GuardianFramebuffer> GetMeshAssetRenderingView(GString meshName);
		static std::shared_ptr<GuardianFramebuffer> GetMeshAssetRenderingViewFromPath(GString meshFilePath);

		static GuardianTexture GetTexture(GString textureName);
		static const GString GetTextureName(GuardianTexture texture);

		static std::map<GString, std::shared_ptr<GuardianMeshAsset>> GetMeshAssetList();
		static std::map<GString, GuardianTexture> GetTextureList();

	private:
		static void AddDefaultAsset();

		static void IterateDirectory(const std::filesystem::path& path);
		static void LoadAsset(const std::filesystem::path& path);
		
		static std::vector<GString> LoadedAssetPath;
		static std::map<GString, std::shared_ptr<GuardianMeshAsset>> MeshAssetsList;
		static std::map<GString, GuardianTexture> TextureAssetList;
		static std::map<GString, GuardianPhysicsMaterial> PhysicsMaterialResourceList;

		static std::map<GString, std::shared_ptr<GuardianMesh>> MeshAssetRenderingList;
	};
}

#endif