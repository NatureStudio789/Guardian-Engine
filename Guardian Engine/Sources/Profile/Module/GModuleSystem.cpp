#include "GModuleSystem.h"

namespace GE
{
	std::vector<std::shared_ptr<GModule>> GModuleSystem::ModuleList;


	void GModuleSystem::RegisterModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, false);

		std::shared_ptr<GModule> m;

		if (moduleName == "Render")
		{
			m = std::make_shared<GRenderEngine>();
		}
		else if (moduleName == "Audio")
		{
			m = std::make_shared<GAudioEngine>();
		}
		else if (moduleName == "Asset")
		{
			m = std::make_shared<GAssetEngine>();
		}
		else if (moduleName == "Gameplay")
		{
			m = std::make_shared<GGameplayEngine>();
		}

		ModuleList.push_back(m);
	}

	void GModuleSystem::RegisterMultiModules(const std::vector<std::string>& moduleNameList)
	{
		for (const auto& moduleName : moduleNameList)
		{
			RegisterModule(moduleName);
		}
	}

	void GModuleSystem::LoadModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		if (GetModule(moduleName)->GetModuleLoaded())
		{
			return;
		}

		GetModule(moduleName)->InitializeModule();
		GetModule(moduleName)->IsModuleLoaded = true;
	}

	void GModuleSystem::LoadAllModules()
	{
		for (auto& module : ModuleList)
		{
			LoadModule(module->GetModuleName());
		}
	}

	void GModuleSystem::UpdateModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		if (!GetModule(moduleName)->GetModuleLoaded())
		{
			return;
		}

		GetModule(moduleName)->UpdateModule();
	}

	void GModuleSystem::UpdateAllModules()
	{
		for (auto& module : ModuleList)
		{
			UpdateModule(module->GetModuleName());
		}
	}

	void GModuleSystem::ReleaseModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		if (!GetModule(moduleName)->GetModuleLoaded())
		{
			return;
		}

		GetModule(moduleName)->ReleaseModule();
		GetModule(moduleName)->IsModuleLoaded = false;
	}

	void GModuleSystem::ReleaseAllModules()
	{
		for (auto& module : ModuleList)
		{
			ReleaseModule(module->GetModuleName());
		}
	}

	std::shared_ptr<GModule> GModuleSystem::GetModule(const std::string& moduleName)
	{
		CheckModuleExists(moduleName, true);

		for (auto& module : ModuleList)
		{
			if (module->GetModuleName() == moduleName)
			{
				return module;
			}
		}

		return null;
	}

	void GModuleSystem::CheckModuleExists(const std::string& moduleName, bool wanted)
	{
		if (wanted)
		{
			for (auto& module : ModuleList)
			{
				if (module->GetModuleName() == moduleName)
				{
					return;
				}
			}
			
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No module called '{}' found!", moduleName));
		}
		else
		{
			for (auto& module : ModuleList)
			{
				if (module->GetModuleName() == moduleName)
				{
					throw GUARDIAN_ERROR_EXCEPTION(std::format("The '{}' module has been registerred!", moduleName));
				}
			}
		}
	}
}
