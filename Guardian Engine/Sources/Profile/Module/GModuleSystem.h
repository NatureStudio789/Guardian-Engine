#ifndef _GE_GMODULESYSTEM_H_
#define _GE_GMODULESYSTEM_H_
#include "../../Gameplay/GGameplayEngine.h"

namespace GE
{
	class GUARDIAN_API GModuleSystem
	{
	public:
		static void RegisterModule(const std::string& moduleName);
		static void RegisterMultiModules(const std::vector<std::string>& moduleNameList);

		static void LoadModule(const std::string& moduleName);
		static void LoadAllModules();
		static void UpdateModule(const std::string& moduleName);
		static void UpdateAllModules();
		static void ReleaseModule(const std::string& moduleName);
		static void ReleaseAllModules();

		static std::shared_ptr<GModule> GetModule(const std::string& moduleName);
		
	private:
		static void CheckModuleExists(const std::string& moduleName, bool wanted);

		static std::vector<std::shared_ptr<GModule>> ModuleList;
	};
}

#endif