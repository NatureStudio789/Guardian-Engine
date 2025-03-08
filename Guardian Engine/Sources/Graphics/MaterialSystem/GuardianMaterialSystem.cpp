#include "GuardianMaterialSystem.h"

namespace guardian
{
    std::map<GString, std::shared_ptr<GuardianMaterial>> GuardianMaterialSystem::MaterialList;


    const GuardianUUID& GuardianMaterialSystem::CreateNewMaterial(const GString& name)
    {
        if (MaterialList.count(name) > 0)
        {
            throw GUARDIAN_ERROR_EXCEPTION("The material called '" + name + "' has already been exists!");
        }

        MaterialList[name] = std::make_shared<GuardianMaterial>();
        return MaterialList[name]->GetMaterialId();
    }

    std::shared_ptr<GuardianMaterial> GuardianMaterialSystem::GetMaterial(const GString& name)
    {
        if (MaterialList.count(name) <= 0)
        {
            throw GUARDIAN_ERROR_EXCEPTION("No materials called '" + name + "' are exists!");
        }

        return MaterialList[name];
    }

    std::shared_ptr<GuardianMaterial> GuardianMaterialSystem::GetMaterial(const GuardianUUID& id)
    {
        for (auto& iterator : MaterialList)
        {
            if (iterator.second->GetMaterialId() == id)
            {
                return iterator.second;
            }
        }

        throw GUARDIAN_ERROR_EXCEPTION("No materials with id : '" + std::to_string(id) + "' are exists!");
    }

    const GString& GuardianMaterialSystem::GetMaterialName(const GuardianUUID& id)
    {
        for (auto& iterator : MaterialList)
        {
            if (iterator.second->GetMaterialId() == id)
            {
                return iterator.first;
            }
        }

        throw GUARDIAN_ERROR_EXCEPTION("No materials with id : '" + std::to_string(id) + "' are exists!");
    }

    std::map<GString, std::shared_ptr<GuardianMaterial>> GuardianMaterialSystem::GetMaterialList()
    {
        return MaterialList;
    }
}
