#ifndef _GE_GUARDIANSCRIPTENGINE_H_
#define _GE_GUARDIANSCRIPTENGINE_H_
#include "ScriptClass/GuardianScriptClass.h"

namespace guardian
{
	class GuardianEntity;
	class GuardianScene;
	class GUARDIAN_API GuardianScriptEngine
	{
	public:
		static void InitializeScriptEngine();

		static void LoadAssembly(const GString& filePath);

		static void StartRuntime(GuardianScene* scene);
		static void OnInitializeEntity(std::shared_ptr<GuardianEntity> entity);
		static void OnUpdateEntity(std::shared_ptr<GuardianEntity> entity);
		static void OnReleaseEntity(std::shared_ptr<GuardianEntity> entity);
		static void StopRuntime();

		static const bool IsEntityClassExists(const GString& fullName);
		static std::shared_ptr<GuardianScene> GetSceneContext();
		static std::unordered_map<GString, std::shared_ptr<GuardianScriptClass>> GetEntityClassList();

	private:
		static char* ReadFileBytes(const GString& filePath, unsigned int* outSize);
		static MonoAssembly* LoadCSharpAssembly(const GString& assemblyPath);

		static void InitializeMono();

		static MonoDomain* RootDomain;
		static MonoDomain* ApplicationDomain;

		static MonoAssembly* CoreAssembly;

		static std::unordered_map<GString, std::shared_ptr<GuardianScriptClass>> EntityClassList;
		static std::shared_ptr<GuardianScene> SceneContext;

		friend class GuardianScriptClass;
	};
}

#endif