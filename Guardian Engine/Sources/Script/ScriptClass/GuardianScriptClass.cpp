#include "GuardianScriptClass.h"
#include "../GuardianScriptEngine.h"

namespace GE 
{
	GuardianScriptClass::GuardianScriptClass(const GString& classNamespace, const GString& className)
	{
		this->InitializeScriptClass(classNamespace, className);
	}

	void GuardianScriptClass::InitializeScriptClass(const GString classNamespace, const GString& className)
	{
		this->ClassNamespace = classNamespace;
		this->ClassName = className;

		this->ClassInstance = mono_class_from_name(mono_assembly_get_image(GuardianScriptEngine::CoreAssembly),
			this->ClassNamespace.c_str(), this->ClassName.c_str());
	}

	void GuardianScriptClass::InvokeMethod(MonoMethod* method, void** parameters)
	{
		mono_runtime_invoke(method, this->ClassObject, parameters, null);
	}

	void GuardianScriptClass::InvokeMethod(const GString& name, int parameterCount, void** parameters)
	{
		this->InvokeMethod(this->GetClassMethod(name, parameterCount), parameters);
	}

	void GuardianScriptClass::InstantiateObject()
	{
		if (this->ClassObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The object has been instantiated!");
		}

		this->ClassObject = mono_object_new(GuardianScriptEngine::ApplicationDomain, this->ClassInstance);
		mono_runtime_object_init(this->ClassObject);
	}

	MonoObject* GuardianScriptClass::GetClassObject()
	{
		return this->ClassObject;
	}

	MonoMethod* GuardianScriptClass::GetClassMethod(const GString& name, int parameterCount)
	{
		return mono_class_get_method_from_name(this->ClassInstance, name.c_str(), parameterCount);
	}
}
