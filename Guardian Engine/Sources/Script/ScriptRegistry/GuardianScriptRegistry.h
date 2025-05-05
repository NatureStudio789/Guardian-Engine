#ifndef _GE_GUARDIANSCRIPTREGISTRY_H_
#define _GE_GUARDIANSCRIPTREGISTRY_H_
#include "../../Application/Input/GuardianInput.h"

namespace GE
{
	class GuardianEntity;

	class GUARDIAN_API GuardianScriptRegistry
	{
	public:
		static void RegisterFunction(const GString& functionName, const void* functionPtr);
		static void RegisterComponents();

		static std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> GetEntityCreateComponentFunctionList();
		static std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> GetEntityHasComponentFunctionList();
		static std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> GetEntityRemoveComponentFunctionList();

	private:
		friend class GuardianScriptEngine;
	};
}

#endif