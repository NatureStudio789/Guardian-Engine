#ifndef _GE_GMATERIALREGISTRY_H_
#define _GE_GMATERIALREGISTRY_H_
#include "GMaterial.h"

namespace GE
{
	class GUARDIAN_API GMaterialRegistry
	{
	public:
		static void RegistryMaterial(std::shared_ptr<GMaterial> material);

		static bool HasMaterial(const GUUID& id);
		static bool HasMaterial(const std::string& name);
		static std::shared_ptr<GMaterial> GetMaterial(const GUUID& id);
		static std::shared_ptr<GMaterial> GetMaterial(const std::string& name);

	private:
		static std::map<std::string, std::shared_ptr<GMaterial>> MaterialList;
	};
}

#endif