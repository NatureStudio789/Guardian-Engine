#include "GuardianScriptClass.h"
#include "../GuardianScriptEngine.h"

namespace GE 
{
	GuardianScriptClass::GuardianScriptClass(const GuardianScriptClass& other)
	{
		this->ClassName = other.ClassName;
		this->ClassNamespace = other.ClassNamespace;
		this->ClassInstance = other.ClassInstance;
		this->ClassObject = other.ClassObject;
		this->ClassFieldList = other.ClassFieldList;
	}

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

	bool GuardianScriptClass::HasField(const GString& name) const
	{
		return this->ClassFieldList.count(name) > 0;
	}

	GuardianScriptClass::ScriptField& GuardianScriptClass::GetClassField(const GString& name)
	{
		if (!this->HasField(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No field named : '{}' found in class!", name));
		}

		return this->ClassFieldList[name];
	}

	void GuardianScriptClass::SetFieldValue(const GString& name, void* value)
	{
		auto& field = this->GetClassField(name);

		mono_field_set_value(this->ClassObject, field.ClassField, value);
	}

	void* GuardianScriptClass::GetFieldValue(const GString& name)
	{
		auto& field = this->GetClassField(name);

		void* data = malloc(16);
		mono_field_get_value(this->ClassObject, field.ClassField, data);

		return data;
	}

	std::map<GString, GuardianScriptClass::ScriptField> GuardianScriptClass::GetClassFieldList()
	{
		return this->ClassFieldList;
	}
}
