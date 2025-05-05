#include "GuardianScriptEngine.h"
#include "../ECS/Entity/GuardianEntity.h"
#include "../ECS/Scene/GuardianScene.h"

namespace GE
{
	MonoDomain* GuardianScriptEngine::RootDomain = null;
	MonoDomain* GuardianScriptEngine::ApplicationDomain = null;

	MonoAssembly* GuardianScriptEngine::CoreAssembly = null;

	std::unordered_map<GString, std::shared_ptr<GuardianScriptClass>> GuardianScriptEngine::EntityClassList;
	std::shared_ptr<GuardianScene> GuardianScriptEngine::SceneContext;

	static std::map<GString, GuardianScriptClass::ScriptField::Category> ScriptFieldTypeList =
	{
		{"System.Single", GuardianScriptClass::ScriptField::GE_FLOAT}, 
		{"System.Double", GuardianScriptClass::ScriptField::GE_DOUBLE}, 
		{"System.Bool", GuardianScriptClass::ScriptField::GE_BOOL}, 
		{"System.Byte", GuardianScriptClass::ScriptField::GE_BYTE}, 
		{"System.Short", GuardianScriptClass::ScriptField::GE_SHORT}, 
		{"System.Int32", GuardianScriptClass::ScriptField::GE_INT}, 
		{"System.Long", GuardianScriptClass::ScriptField::GE_LONG}, 
		{"System.UByte", GuardianScriptClass::ScriptField::GE_UBYTE}, 
		{"System.UShort", GuardianScriptClass::ScriptField::GE_USHORT}, 
		{"System.UInt", GuardianScriptClass::ScriptField::GE_UINT}, 
		{"System.ULong", GuardianScriptClass::ScriptField::GE_ULONG}, 

		{"GE.GVector2", GuardianScriptClass::ScriptField::GE_GVECTOR2}, 
		{"GE.GVector3", GuardianScriptClass::ScriptField::GE_GVECTOR3}, 
		{"GE.GVector4", GuardianScriptClass::ScriptField::GE_GVECTOR4}, 

		{"GE.Entity", GuardianScriptClass::ScriptField::GE_ENTITY}
	};


	static GuardianScriptClass::ScriptField::Category MonoTypeToScriptFieldType(MonoType* monoType)
	{
		GString typeName = mono_type_get_name(monoType);

		if (ScriptFieldTypeList.count(typeName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No supported type of '{}' to script field type!", typeName));
		}

		return ScriptFieldTypeList[typeName];
	}

	static void EntityCreateComponent(unsigned long long uuid, MonoReflectionType* reflectionType)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);

		MonoType* managedType = mono_reflection_type_get_type(reflectionType);
		const auto& CreateComponentFunctionList = GuardianScriptRegistry::GetEntityCreateComponentFunctionList();
		if (CreateComponentFunctionList.find(managedType) == CreateComponentFunctionList.end())
		{
			GString ManagedTypeName = mono_type_get_name(managedType);
			printf((("Cannot find the type : '" + ManagedTypeName + "' !").c_str()));
			return;
		}

		CreateComponentFunctionList.at(managedType)(entity);
	}

	static bool EntityHasComponent(unsigned long long uuid, MonoReflectionType* componentType)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);
		if (!entity)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find the entity with this UUID!");
		}

		MonoType* monoComponentType = mono_reflection_type_get_type(componentType);
		const auto& HasComponentFunctionList = GuardianScriptRegistry::GetEntityHasComponentFunctionList();
		if (HasComponentFunctionList.find(monoComponentType) == HasComponentFunctionList.end())
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot find this type of component in engine!");
		}

		return HasComponentFunctionList.at(monoComponentType)(entity);
	}

	static void EntityRemoveComponent(unsigned long long uuid, MonoReflectionType* reflectionType)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);

		MonoType* managedType = mono_reflection_type_get_type(reflectionType);
		const auto& RemoveComponentFunctionList = GuardianScriptRegistry::GetEntityRemoveComponentFunctionList();
		if (RemoveComponentFunctionList.find(managedType) == RemoveComponentFunctionList.end())
		{
			GString ManagedTypeName = mono_type_get_name(managedType);
			printf((("Cannot find the type : '" + ManagedTypeName + "' !\n").c_str()));
			return;
		}

		RemoveComponentFunctionList.at(managedType)(entity);
	}

	static void EntityGetTranslation(unsigned long long uuid, GVector3* translation)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);

		(*translation) = entity->GetComponent<GuardianTransformComponent>().Position;
	}

	static void EntitySetTranslation(unsigned long long uuid, GVector3* translation)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);

		entity->GetComponent<GuardianTransformComponent>().Position = *translation;
	}

	static bool InputIsKeyPressed(unsigned int keyCode)
	{
		return GuardianKeyboard::IsKeyPress(keyCode);
	}

	void GuardianScriptEngine::InitializeScriptEngine()
	{
		InitializeMono();
		LoadAssembly("Assets/Scripts/Guardian-ScriptCore.dll");

		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::EntityCreateComponent", EntityCreateComponent);
		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::EntityHasComponent", EntityHasComponent);
		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::EntityRemoveComponent", EntityRemoveComponent);
		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::TransformComponentGetTranslation", EntityGetTranslation);
		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::TransformComponentSetTranslation", EntitySetTranslation);
		GuardianScriptRegistry::RegisterFunction("GE.InternalMethods::InputIsKeyPressed", InputIsKeyPressed);

		GuardianScriptRegistry::RegisterComponents();

		EntityClassList.clear();

		MonoImage* image = mono_assembly_get_image(CoreAssembly);
		const MonoTableInfo* TypeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int NumTypes = mono_table_info_get_rows(TypeDefinitionsTable);
		GuardianScriptClass EntityClass = { "GE", "Entity" };

		for (int i = 0; i < NumTypes; i++)
		{
			unsigned int cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(TypeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			GString nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			GString name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			GString fullName;
			if (nameSpace.size() != 0)
			{
				fullName = nameSpace + "." + name;
			}
			else
			{
				fullName = name;
			}

			GuardianScriptClass Class = { nameSpace, name };

			if (Class.ClassInstance == EntityClass.ClassInstance)
			{
				continue;
			}

			bool isEntity = mono_class_is_subclass_of(Class.ClassInstance, EntityClass.ClassInstance, false);
			if (!isEntity)
			{
				continue;
			}

			std::shared_ptr<GuardianScriptClass> ScriptClass = std::make_shared<GuardianScriptClass>(Class);
			EntityClassList[fullName] = ScriptClass;

			MonoClass* monoClass = mono_class_from_name(mono_assembly_get_image(CoreAssembly), nameSpace.c_str(), name.c_str());

			void* iterator = null;
			printf("%s has %d fields.\n", name.c_str(), mono_class_num_fields(monoClass));
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				GString FieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					printf("    %s\n", FieldName.c_str());

					ScriptClass->ClassFieldList[FieldName] = {};
					ScriptClass->ClassFieldList[FieldName].Name = FieldName;
					ScriptClass->ClassFieldList[FieldName].ClassField = field;
					ScriptClass->ClassFieldList[FieldName].Type = MonoTypeToScriptFieldType(mono_field_get_type(field));
				}
			}
		}
	}

	void GuardianScriptEngine::StartRuntime(GuardianScene* scene)
	{
		SceneContext = std::make_shared<GuardianScene>(*scene);
	}

	void GuardianScriptEngine::OnInitializeEntity(std::shared_ptr<GuardianEntity> entity)
	{
		const auto& SComponent = entity->GetComponent<GuardianScriptComponent>();
		if (IsEntityClassExists(SComponent.ClassName))
		{
			if (!EntityClassList[SComponent.ClassName]->GetClassObject())
			{
				EntityClassList[SComponent.ClassName]->InstantiateObject();
			}

			for (auto& field : EntityClassList[SComponent.ClassName]->ClassFieldList)
			{
				EntityClassList[SComponent.ClassName]->SetFieldValue(field.first, (void*)field.second.GetValueBuffer());
			}

			void* parameter = (unsigned long long*)&entity->GetEntityId();
			EntityClassList[SComponent.ClassName]->InvokeMethod(GuardianScriptClass{ "GE", "Entity"}.GetClassMethod(".ctor", 1), &parameter);
			EntityClassList[SComponent.ClassName]->InvokeMethod("Initialize", 0, null);
		}
	}

	void GuardianScriptEngine::OnUpdateEntity(std::shared_ptr<GuardianEntity> entity)
	{
		const auto& SComponent = entity->GetComponent<GuardianScriptComponent>();
		if (IsEntityClassExists(SComponent.ClassName))
		{
			float deltaTime = GuardianTime::GetDeltaTime();
			void* parameter = &deltaTime;

			EntityClassList[SComponent.ClassName]->InvokeMethod("Update", 1, &parameter);
		}
	}

	void GuardianScriptEngine::OnReleaseEntity(std::shared_ptr<GuardianEntity> entity)
	{
		const auto& SComponent = entity->GetComponent<GuardianScriptComponent>();
		if (IsEntityClassExists(SComponent.ClassName))
		{
			EntityClassList[SComponent.ClassName]->InvokeMethod("Release", 0, null);
		}
	}

	void GuardianScriptEngine::StopRuntime()
	{
		SceneContext = null;
	}

	std::shared_ptr<GuardianScriptClass> GuardianScriptEngine::GetEntityClass(const GString& name)
	{
		if (EntityClassList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No script entity class named : '{}' found!", name));
		}

		return EntityClassList[name];
	}

	void GuardianScriptEngine::LoadAssembly(const GString& filePath)
	{
		CoreAssembly = LoadCSharpAssembly(filePath);
	}

	const bool GuardianScriptEngine::IsEntityClassExists(const GString& fullName)
	{
		return EntityClassList.find(fullName) != EntityClassList.end();
	}

	std::shared_ptr<GuardianScene> GuardianScriptEngine::GetSceneContext()
	{
		return SceneContext;
	}

	std::unordered_map<GString, std::shared_ptr<GuardianScriptClass>> GuardianScriptEngine::GetEntityClassList()
	{
		return EntityClassList;
	}

	MonoAssembly* GuardianScriptEngine::GetCoreAssembly()
	{
		return CoreAssembly;
	}

	char* GuardianScriptEngine::ReadFileBytes(const GString& filePath, unsigned int* outSize)
	{
		std::ifstream FileStream(filePath, std::ios::binary | std::ios::ate);
		if (!FileStream)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The file stream is invalid!");
		}

		std::streampos end = FileStream.tellg();
		FileStream.seekg(0, std::ios::beg);
		unsigned int size = (unsigned int)(end - FileStream.tellg());
		if (size == 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The file is empty!");
		}

		char* buffer = new char[size];
		FileStream.read((char*)buffer, size);
		FileStream.close();

		(*outSize) = size;
		return buffer;
	}

	MonoAssembly* GuardianScriptEngine::LoadCSharpAssembly(const GString& assemblyPath)
	{
		unsigned int fileSize = 0;
		char* fileData = ReadFileBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			throw GUARDIAN_ERROR_EXCEPTION(errorMessage);
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	void GuardianScriptEngine::InitializeMono()
	{
		mono_set_assemblies_path("mono/lib");

		RootDomain = mono_jit_init("GuardianJITRuntime");
		if (!RootDomain)
		{
			throw GUARDIAN_FAILURE_EXCEPTION("mono_jit_init()");
		}

		ApplicationDomain = mono_domain_create_appdomain("GuardianScriptRuntime", null);
		mono_domain_set(ApplicationDomain, true);
	}
}
