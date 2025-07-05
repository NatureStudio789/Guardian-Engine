#include "GModuleSystem.h"

namespace GE
{
	std::map<std::string, std::shared_ptr<GModule>> GModuleSystem::ModuleList;


	void GModuleSystem::RegisterModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, false);

		std::shared_ptr<GModule> m;

		//TO DO : Get module class from module name.

		ModuleList[moduleName] = m;
	}

	void GModuleSystem::LoadModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		if (ModuleList[moduleName]->GetModuleLoaded())
		{
			return;
		}

		ModuleList[moduleName]->InitializeModule();
		ModuleList[moduleName]->IsModuleLoaded = true;
	}

	void GModuleSystem::ReleaseModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		if (!ModuleList[moduleName]->GetModuleLoaded())
		{
			return;
		}

		ModuleList[moduleName]->ReleaseModule();
		ModuleList[moduleName]->IsModuleLoaded = false;
	}

	std::shared_ptr<GModule> GModuleSystem::GetModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		return ModuleList[moduleName];
	}

	void GModuleSystem::CheckModuleExists(const std::string& moduleName, bool wanted)
	{
		if (wanted)
		{
			if (ModuleList.count(moduleName) <= 0)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("No module called '{}' found!", moduleName));
			}
		}
		else
		{
			if (ModuleList.count(moduleName) > 0)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("The '{}' module has been registerred!", moduleName));
			}
		}
	}
}
