#ifndef _GE_GUARDIANSCRIPTREGISTRY_H_
#define _GE_GUARDIANSCRIPTREGISTRY_H_
#include "../../Application/Input/GuardianInput.h"

namespace guardian
{
	class GUARDIAN_API GuardianScriptRegistry
	{
	public:
		static void RegisterFunction(const GString& functionName, const void* functionPtr);

	};
}

#endif