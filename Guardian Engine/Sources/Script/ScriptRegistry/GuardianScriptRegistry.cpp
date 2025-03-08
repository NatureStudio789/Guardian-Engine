#include "GuardianScriptRegistry.h"
#include "../GuardianScriptEngine.h"
#include "../../ECS/Entity/GuardianEntity.h"

namespace guardian
{
	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> EntityHasComponentFunctionList;


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
			GString ManagedTypename = "Guardian." + StructName;

			MonoType* ManagedType = mono_reflection_type_from_name(ManagedTypename.data(),
				mono_assembly_get_image(GuardianScriptEngine::GetCoreAssembly()));
			if (!ManagedType)
			{
				printf((("Cannot find the type : '" + ManagedTypename + "' !").c_str()));
				return;
			}

			EntityHasComponentFunctionList[ManagedType] = [](std::shared_ptr<GuardianEntity> entity) { return entity->HasComponent<Component>(); };
		}(), ...);
	}

	void GuardianScriptRegistry::RegisterComponents()
	{
		RegisterComponent<GuardianTransformComponent, GuardianCameraComponent, GuardianPointLightComponent, 
			GuardianSphereColliderComponent, GuardianBoxColliderComponent, GuardianCapsuleColliderComponent, 
			GuardianMeshColliderComponent, GuardianRigidBodyComponent>();
	}

	std::unordered_map<MonoType*, std::function<bool(std::shared_ptr<GuardianEntity>)>> GuardianScriptRegistry::GetEntityHasComponentFunctionList()
	{
		return EntityHasComponentFunctionList;
	}
}
