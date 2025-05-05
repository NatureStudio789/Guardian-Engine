#include "GuardianScriptRegistry.h"
#include "../GuardianScriptEngine.h"
#include "../../ECS/Entity/GuardianEntity.h"

namespace GE
{
	std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> EntityCreateComponentFunctionList;
	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> EntityHasComponentFunctionList;
	std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> EntityRemoveComponentFunctionList;


	static void EntityCreateComponent(unsigned long long uuid, MonoReflectionType* reflectionType)
	{
		auto entity = GuardianScriptEngine::GetSceneContext()->GetEntity(uuid);

		MonoType* managedType = mono_reflection_type_get_type(reflectionType);
		if (EntityCreateComponentFunctionList.find(managedType) == EntityCreateComponentFunctionList.end())
		{
			GString ManagedTypeName = mono_type_get_name(managedType);
			printf((("Cannot find the type : '" + ManagedTypeName + "' !").c_str()));
			return;
		}

		EntityCreateComponentFunctionList.at(managedType)(entity);
	}

	void GuardianScriptRegistry::RegisterFunction(const GString& functionName, const void* functionPtr)
	{
		mono_add_internal_call(functionName.c_str(), functionPtr);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			GString typeName = typeid(Component).name();
			GString StructName = typeName.substr(typeName.find_last_of(':') + 1);
			GString ManagedTypename = "GE." + StructName;

			MonoType* ManagedType = mono_reflection_type_from_name(ManagedTypename.data(),
				mono_assembly_get_image(GuardianScriptEngine::GetCoreAssembly()));
			if (!ManagedType)
			{
				printf((("Cannot find the type : '" + ManagedTypename + "' !").c_str()));
				return;
			}

			EntityCreateComponentFunctionList[ManagedType] = [](std::shared_ptr<GuardianEntity> entity) { entity->AddComponent<Component>(); };
			EntityHasComponentFunctionList[ManagedType] = [](std::shared_ptr<GuardianEntity> entity) { return entity->HasComponent<Component>(); };
			EntityRemoveComponentFunctionList[ManagedType] = [](std::shared_ptr<GuardianEntity> entity) { entity->RemoveComponent<Component>(); };
		}(), ...);
	}

	void GuardianScriptRegistry::RegisterComponents()
	{
		RegisterComponent<GuardianTransformComponent, GuardianCameraComponent, GuardianPointLightComponent, 
			GuardianSphereColliderComponent, GuardianBoxColliderComponent, GuardianCapsuleColliderComponent, 
			GuardianMeshColliderComponent, GuardianRigidBodyComponent>();
	}

	std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::GetEntityCreateComponentFunctionList()
	{
		return EntityCreateComponentFunctionList;
	}

	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::GetEntityHasComponentFunctionList()
	{
		return EntityHasComponentFunctionList;
	}

	std::unordered_map<MonoType*, std::function<void(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::GetEntityRemoveComponentFunctionList()
	{
		return EntityRemoveComponentFunctionList;
	}
}
