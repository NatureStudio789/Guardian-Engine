#include "GModuleSystem.h"

namespace GE
{
	std::map<std::string, std::shared_ptr<GModule>> GModuleSystem::ModuleList;


	void GModuleSystem::RegisterModule(const std::string& moduleName, std::shared_ptr<GModule> module)
	{
		ModuleList[moduleName] = module;
	}

	void GModuleSystem::LoadModule(const std::string& moduleName)
	{
		//TO DO : Check the module registered.
		/*...*/

		if (ModuleList[moduleName]->GetModuleLoaded())
		{
			return;
		}

		ModuleList[moduleName]->InitializeModule();
		ModuleList[moduleName]->IsModuleLoaded = true;
	}

	void GModuleSystem::ReleaseModule(const std::string& moduleName)
	{
		//TO DO : Check the module registered.
		/*...*/

		if (!ModuleList[moduleName]->GetModuleLoaded())
		{
			return;
		}

		ModuleList[moduleName]->ReleaseModule();
		ModuleList[moduleName]->IsModuleLoaded = false;
	}

	std::shared_ptr<GModule> GModuleSystem::GetModule(const std::string& moduleName)
	{
		//TO DO : Check the module registered.
		/*...*/

		return ModuleList[moduleName];
	}
}
