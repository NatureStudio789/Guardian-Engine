#include "GMaterialRegistry.h"

namespace GE
{
	std::map<std::string, std::shared_ptr<GMaterial>> GMaterialRegistry::MaterialList;


	void GMaterialRegistry::RegistryMaterial(std::shared_ptr<GMaterial> material)
	{
		if (MaterialList.count(material->GetMaterialName()) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The material named '{}' already exists in the registry!", material->GetMaterialName()));
		}

		MaterialList[material->GetMaterialName()] = material;
	}

	bool GMaterialRegistry::HasMaterial(const GUUID& id)
	{
		for (auto& material : MaterialList)
		{
			if (material.second->GetMaterialId() == id)
			{
				return true;
			}
		}

		return false;
	}

	bool GMaterialRegistry::HasMaterial(const std::string& name)
	{
		return MaterialList.count(name);
	}

	std::shared_ptr<GMaterial> GMaterialRegistry::GetMaterial(const GUUID& id)
	{
		for (auto& material : MaterialList)
		{
			if (material.second->GetMaterialId() == id)
			{
				return material.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No material with id : '{}' found in registry!", (unsigned long long)id));
	}

	std::shared_ptr<GMaterial> GMaterialRegistry::GetMaterial(const std::string& name)
	{
		if (MaterialList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No material named '{}' found in registry!", name));
		}

		return MaterialList[name];
	}
}
