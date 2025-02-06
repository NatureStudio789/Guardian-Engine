#include "GuardianScriptRegistry.h"
#include "../../ECS/Entity/GuardianEntity.h"

namespace guardian
{
	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::EntityHasComponentFunctionList;


	void GuardianScriptRegistry::RegisterFunction(const GString& functionName, const void* functionPtr)
	{
		mono_add_internal_call(functionName.c_str(), functionPtr);
	}

	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::GetEntityHasComponentFunctionList()
	{
		return EntityHasComponentFunctionList;
	}
}
