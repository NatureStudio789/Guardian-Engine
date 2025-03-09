#ifndef _GE_GUARDIANMATERIALSYSTEM_H_
#define _GE_GUARDIANMATERIALSYSTEM_H_
#include "../Material/GuardianMaterial.h"

namespace guardian
{
	class GUARDIAN_API GuardianMesh;

	class GUARDIAN_API GuardianMaterialSystem
	{
	public:
		static const GuardianUUID& CreateNewMaterial(const GString& name);

		static void UpdateMaterialSystem();

		static std::shared_ptr<GuardianMaterial> GetMaterial(const GString& name);
		static std::shared_ptr<GuardianMaterial> GetMaterial(const GuardianUUID& id);

		static std::shared_ptr<GuardianFramebuffer> GetMaterialRenderingView(const GString& materialName);
		static std::shared_ptr<GuardianFramebuffer> GetMaterialRenderingView(const GuardianUUID& id);

		static const GString& GetMaterialName(const GuardianUUID& id);

		static std::map<GString, std::shared_ptr<GuardianMaterial>> GetMaterialList();

	private:
		static std::map<GString, std::shared_ptr<GuardianMaterial>> MaterialList;
		static std::map<GString, std::shared_ptr<GuardianMesh>> MaterialSphereMeshList;
	};
}

#endif	