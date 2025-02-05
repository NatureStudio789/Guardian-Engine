#include "GuardianScriptEngine.h"
#include "../ECS/Entity/GuardianEntity.h"
#include "../ECS/Scene/GuardianScene.h"

namespace guardian
{
	MonoDomain* GuardianScriptEngine::RootDomain = null;
	MonoDomain* GuardianScriptEngine::ApplicationDomain = null;

	MonoAssembly* GuardianScriptEngine::CoreAssembly = null;

	std::unordered_map<GString, std::shared_ptr<GuardianScriptClass>> GuardianScriptEngine::EntityClassList;
	std::shared_ptr<GuardianScene> GuardianScriptEngine::SceneContext;


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
		LoadAssembly("Resources/Scripts/Guardian-ScriptCore.dll");

		GuardianScriptRegistry::RegisterFunction("Guardian.InternalMethods::EntityGetTranslation", EntityGetTranslation);
		GuardianScriptRegistry::RegisterFunction("Guardian.InternalMethods::EntitySetTranslation", EntitySetTranslation);
		GuardianScriptRegistry::RegisterFunction("Guardian.InternalMethods::InputIsKeyPressed", InputIsKeyPressed);

		EntityClassList.clear();

		MonoImage* image = mono_assembly_get_image(CoreAssembly);
		const MonoTableInfo* TypeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int NumTypes = mono_table_info_get_rows(TypeDefinitionsTable);
		GuardianScriptClass EntityClass = { "Guardian", "Entity" };

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
			if (isEntity)
			{
				EntityClassList[fullName] = std::make_shared<GuardianScriptClass>(Class);
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

			void* parameter = (unsigned long long*)&entity->GetEntityId();
			EntityClassList[SComponent.ClassName]->InvokeMethod(GuardianScriptClass{ "Guardian", "Entity"}.GetClassMethod(".ctor", 1), &parameter);
			EntityClassList[SComponent.ClassName]->InvokeMethod("Initialize", 0, null);
		}
	}

	void GuardianScriptEngine::OnUpdateEntity(std::shared_ptr<GuardianEntity> entity)
	{
		const auto& SComponent = entity->GetComponent<GuardianScriptComponent>();
		if (IsEntityClassExists(SComponent.ClassName))
		{
			float deltaTime = 16.6667f;
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
