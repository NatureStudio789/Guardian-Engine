#include "GuardianScriptRegistry.h"

namespace guardian
{
	void GuardianScriptRegistry::RegisterFunction(const GString& functionName, const void* functionPtr)
	{
		mono_add_internal_call(functionName.c_str(), functionPtr);
	}
}
