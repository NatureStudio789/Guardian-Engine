#ifndef _GE_GUARDIANMATERIALSYSTEM_H_
#define _GE_GUARDIANMATERIALSYSTEM_H_
#include "../Material/GuardianMaterial.h"

namespace guardian
{
	class GUARDIAN_API GuardianMaterialSystem
	{
	public:
		static const GuardianUUID& CreateNewMaterial(const GString& name);

		static std::shared_ptr<GuardianMaterial> GetMaterial(const GString& name);
		static std::shared_ptr<GuardianMaterial> GetMaterial(const GuardianUUID& id);

		static const GString& GetMaterialName(const GuardianUUID& id);

		static std::map<GString, std::shared_ptr<GuardianMaterial>> GetMaterialList();

	private:
		static std::map<GString, std::shared_ptr<GuardianMaterial>> MaterialList;
	};
}

#endif	