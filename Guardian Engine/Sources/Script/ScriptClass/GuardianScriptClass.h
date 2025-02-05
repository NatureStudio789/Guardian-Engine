#ifndef _GE_GUARDIANSCRIPTCLASS_H_
#define _GE_GUARDIANSCRIPTCLASS_H_
#include "../ScriptRegistry/GuardianScriptRegistry.h"

namespace guardian
{
	class GUARDIAN_API GuardianScriptClass
	{
	public:
		GuardianScriptClass() = default;
		GuardianScriptClass(const GString& classNamespace, const GString& className);

		void InitializeScriptClass(const GString classNamespace, const GString& className);
		void InstantiateObject();

		void InvokeMethod(MonoMethod* method, void** parameters);
		void InvokeMethod(const GString& name, int parameterCount, void** parameters);

		MonoObject* GetClassObject();
		MonoMethod* GetClassMethod(const GString& name, int parameterCount);

	private:
		GString ClassNamespace;
		GString ClassName;

		MonoClass* ClassInstance = null;
		MonoObject* ClassObject = null;

		friend class GuardianScriptEngine;
	};
}

#endif