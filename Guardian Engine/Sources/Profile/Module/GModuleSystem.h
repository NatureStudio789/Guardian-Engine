#ifndef _GE_GMODULESYSTEM_H_
#define _GE_GMODULESYSTEM_H_
#include "GModule.h"

namespace GE
{
	class GUARDIAN_API GModuleSystem
	{
	public:
		static void RegisterModule(const std::string& moduleName);

		static void LoadModule(const std::string& moduleName);
		static void ReleaseModule(const std::string& moduleName);

		static std::shared_ptr<GModule> GetModule(const std::string& moduleName);
		
	private:
		static void CheckModuleExists(const std::string& moduleName, bool wanted);

		static std::map<std::string, std::shared_ptr<GModule>> ModuleList;
	};
}

#endif